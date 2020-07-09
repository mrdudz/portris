
/***********************************************************************************
                     _        _
     _ __   ___  _ __| |_ _ __(_)___
    | '_ \ / _ \| '__| __| '__| / __|
    | |_) | (_) | |  | |_| |  | \__ \
    | .__/ \___/|_|   \__|_|  |_|___/
    |_|

    version 0.5, 13.12.2017

    a portable Tetris implementation targetted on cc65 (https://cc65.github.io/cc65/)

    (w)2002-2017 Groepaz/Solution (groepaz@gmx.net)

***********************************************************************************/

//#define DEBUG

// required libs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <errno.h>
#include <time.h>
#include <joystick.h>

// define screensize and some other capabilities of the target
#include "portris.h"
const char *VERSION="0.5";

// include other headers
#include "input.h"
#include "output.h"
#include "title.h"

#if !defined(NOHISCORES)
#include "hiscore.h"
#endif
#include "effects.h"

typedef struct
{
    // playfield position on screen
    unsigned char px;
    // position,rotation,color of current block
    unsigned char bx,by;
    unsigned char br,bc,bn;
    // next block color,number
    unsigned char nc,nn;
    unsigned char gameover;     // game over flag
    unsigned char key;          // last input "key"
    clock_t lasttick;    // used for syncing
    clock_t delayticks;    // used for syncing
    unsigned char lines,stage;
    unsigned short score;
    // playfield grid data
    #if !defined(NO2DIMARRAYS)
    unsigned char blk [PF_VY][PF_VX];
    #else
    unsigned char blk [PF_VY*PF_VX];
    #endif
} PLAYFIELD;

PLAYFIELD pf[MAX_PLR];

#define NO_BLOCK    0xff

typedef struct
{
    unsigned char size;
    #if !defined(NO2DIMARRAYS)
    unsigned char map [4][4*4];
    #else
    unsigned char map [4*4*4];
    #endif
//    unsigned char map [4][];
} BLOCK;

#define MAX_BLOCKS 4

// blocks data
#include "blocks.c"

// we don't need the table if no colors available
#if !(defined(NOCOLORS) && defined(NOBGCOLORS))

const unsigned char colortable[0x10]=
{
    COLOR_BROWN,
    COLOR_RED,
    COLOR_ORANGE,
    COLOR_GRAY2,

    COLOR_LIGHTRED,
    COLOR_GRAY3,
    COLOR_YELLOW,
    COLOR_WHITE,

    COLOR_WHITE,
    COLOR_LIGHTGREEN,
    COLOR_CYAN,
    COLOR_GREEN,

    COLOR_LIGHTBLUE,
    COLOR_VIOLET,
    COLOR_GRAY1,
    COLOR_BLUE,
};

#endif

// prototypes...

void init_pf(unsigned char n);

/***********************************************************************************
    ticks root counter

    this should be driven by some (more accurate?) generic timer api!
***********************************************************************************/

static clock_t ticks;

void ticks_reset(void)
{
    ticks=0;
}

void ticks_do(void)
{
    // add elapsed time from timer api here
    #ifdef NOCLOCK
    ticks+=300;
    #else
static clock_t clk,lclk,clocks;

    clk=clock();
    clocks=clk-lclk;
    lclk=clk;

    ticks+=((clocks/(1+(CLOCKS_PER_SEC/50)))*256);
    #endif
}

clock_t ticks_get(void)
{
    return(ticks);
}

/***********************************************************************************
    Playfield Subroutines
***********************************************************************************/

void block_set(PLAYFIELD *pf, register unsigned char col)
{
const BLOCK *blk;
register unsigned char *blocks;
register unsigned char *block;
unsigned char x,y,size;
unsigned char bx,by;

    //blk=myblocks[pf->bn];
    blk=GET1DIM(myblocks,pf->bn);

    size=blk->size;


// #if !defined (NO2DIMARRAYS)
//     block=&blk->map[pf->br][0];
//     #else
//     block=&blk->map[(pf->br*16)+0];
//     #endif

    //block=(unsigned char*)&(blk->map[pf->br][0]);
    //test=blk->map[pf->br][0];
    block=(unsigned char*)&GET2DIM(blk->map,pf->br,0,16);

    bx=pf->bx+3;by=pf->by;

//     #if defined(__SDCC__)
//     /* neither lcc nor sdcc would compile the latter */
//     {
//     //int i=(by*PF_VX)+bx;
//     //blocks=pf->blk+i;
//     blocks=&pf->blk[(int)by][(int)bx];
//     }
//     #else
//         #if !defined (NO2DIMARRAYS)
//         blocks=&pf->blk[by][bx];
//         #else
//         blocks=&pf->blk[((int)by*(int)PF_VX)+(int)bx];
//         #endif
//     #endif

    blocks=&GET2DIM(pf->blk,by,bx,PF_VX);

    for(y=0;y<size;++y)
    {
                        for(x=0;x<size;++x)
                        {
                                if(*block)
                                {
                                    *blocks=col;
                                }
                            ++block;
                            ++blocks;
                        }
                        blocks+=(PF_VX-size);
    }

}

void block_unset(PLAYFIELD *pf)
{
const BLOCK *blk;
register unsigned char *blocks;
register unsigned char *block;
unsigned char x,y,size;
unsigned char bx,by;

    //blk=myblocks[pf->bn];
    blk=GET1DIM(myblocks,pf->bn);


    size=blk->size;

//     #if !defined (NO2DIMARRAYS)
//     block=&blk->map[pf->br][0];
//     #else
//     block=&blk->map[(pf->br*16)+0];
//     #endif

    block=(unsigned char*)&GET2DIM(blk->map,pf->br,0,16);

    bx=pf->bx+3;by=pf->by;

//     #if defined(__SDCC__)
//     /* neither lcc nor sdcc would compile the latter */
//     {

//    int i=(by*PF_VX)+bx;
//    blocks=pf->blk+i;

//     blocks=&pf->blk[(int)by][(int)bx];
//     }
//     #else
//         #if !defined (NO2DIMARRAYS)
//         blocks=&pf->blk[by][bx];
//         #else
//         blocks=&pf->blk[((int)by*(int)PF_VX)+(int)bx];
//         #endif
//     #endif

    blocks=&GET2DIM(pf->blk,by,bx,PF_VX);

    for(y=0;y<size;++y)
    {
                        for(x=0;x<size;++x)
                        {
                                if(*block)
                                {
                                    *blocks=NO_BLOCK;
                                }
                            ++block;
                            ++blocks;
                        }
                        blocks+=(PF_VX-size);
    }

}

// check if block hits any other blocks
unsigned char block_checkhit_x(PLAYFIELD *pf,unsigned char ox)
{
const BLOCK *blk;
unsigned char *blocks;
unsigned char *block;
unsigned char x,y,bx,by,size;

    //blk=myblocks[pf->bn];
    blk=GET1DIM(myblocks,pf->bn);

    size=blk->size;

//     #if !defined (NO2DIMARRAYS)
//     block=&blk->map[pf->br][0];
//     #else
//     block=&blk->map[(pf->br*16)+0];
//     #endif

    block=(unsigned char*)&GET2DIM(blk->map,pf->br,0,16);

    bx=pf->bx+ox;by=pf->by;
//     #if defined(__SDCC__)
//     /* neither lcc nor sdcc would compile the latter */
//     {
//     int i=(by*PF_VX)+bx;
//     blocks=pf->blk+i;
//     blocks=&pf->blk[(int)by][(int)bx];
//     }
//     #else
//         #if !defined (NO2DIMARRAYS)
//         blocks=&pf->blk[by][bx];
//         #else
//         blocks=&pf->blk[((int)by*(int)PF_VX)+(int)bx];
//         #endif
//     #endif
    blocks=&GET2DIM(pf->blk,by,bx,PF_VX);


    for(y=0;y<size;++y)
    {
             for(x=0;x<size;++x)
             {
                 if(*block++)
                 {
                     if(*blocks!=NO_BLOCK) return(1);
                 }
                 ++blocks;
             }
             blocks+=(PF_VX-size);
    }
    return(0);
}
unsigned char block_checkhit_y(PLAYFIELD *pf,unsigned char oy)
{
const BLOCK *blk;
unsigned char *blocks;
unsigned char *block;
unsigned char x,y,bx,by,size;

    //blk=myblocks[pf->bn];
    blk=GET1DIM(myblocks,pf->bn);

    size=blk->size;


//     #if !defined (NO2DIMARRAYS)
//     block=&blk->map[pf->br][0];
//     #else
//     block=&blk->map[(pf->br*16)+0];
//     #endif

    block=(unsigned char*)&GET2DIM(blk->map,pf->br,0,16);

    bx=pf->bx+3;
    by=pf->by+oy;

// /*    #if defined(__SDCC__)
//     /* neither lcc nor sdcc would compile the latter */
//     {
// //    int i=(by*PF_VX)+bx;
// //    blocks=pf->blk+i;
//     blocks=&pf->blk[(int)by][(int)bx];
//     }
//     #else
//         #if !defined (NO2DIMARRAYS)
//         blocks=&pf->blk[by][bx];
//         #else
//         blocks=&pf->blk[((int)by*(int)PF_VX)+(int)bx];
//         #endif
//     #endif*/
    blocks=&GET2DIM(pf->blk,by,bx,PF_VX);


    for(y=0;y<size;++y)
    {
             for(x=0;x<size;++x)
             {
                 if(*block++)
                 {
                     if(*blocks!=NO_BLOCK) return(1);
                 }
                 ++blocks;
             }
             blocks+=(PF_VX-size);
    }
    return(0);
}

// print next block
unsigned char block_printnext(PLAYFIELD *pf)
{
#ifdef __SDCC__
BLOCK *blk;
#else
const BLOCK *blk;
#endif
register unsigned char *block;
unsigned char x,y,bx,by,size;

    #if !defined(NOCOLORS)
unsigned char col;
        //col=colortable[pf->nc];
        col=GET1DIM(colortable,pf->nc);
    #endif

    //blk=myblocks[pf->nn];
    //blk=myblocks[(int)pf->nn];
    blk=GET1DIM(myblocks,pf->nn);
    size=blk->size;
    //size=myblocks[(int)pf->nn]->size;
    {
    //blk=myblocks[0];
    //size=4;
    }

    //cprintf("n:%d\n\r",blk->size);
    //cprintf("s:%d\n\r",size);


//     #if !defined (NO2DIMARRAYS)
//     //block=&blk->map[0][0];
//     block=&blk->map[(int)0][(int)0];
//     #else
//     block=&blk->map[0+0];
//     #endif

    block=(unsigned char*)&GET2DIM(blk->map,0,0,16);

    {
    //block=blk->map;
    }

    bx=pf->px;
    by=PF_Y+1;

//size=4;

    //for(y=0;y<size;++y)
    for(y=0;y<2;++y)
    {
             gotoxy(bx,by);
             for(x=0;x<size;++x)
             {
                        if (*block)
                 {
                           textcolor(col);
                              revers(1);cputc(BLK_SET);revers(0);
                           //cputc('+');
                 }
                 else
                 {
                           cputc(BLK_NONE);
                           //cputc('-');
                 }
                        ++block;
             }
             while(x<4)
             {
                     cputc(BLK_NONE);
                    ++x;
             }
             ++by;
    }
    return(0);
}

// set active block to next block
void block_next(PLAYFIELD *pf)
{
    pf->bn=pf->nn;    // block number
    pf->bc=pf->nc;    // color
    // position new block
    pf->bx=(PF_X/2);pf->by=0;
}

// randomize next block
void block_randomize(PLAYFIELD *pf)
{
unsigned char c;
    c=rand();
    pf->nn=c&0x3;     // block number
    pf->nc=c&0xf;     // color
}

// clears playfield and counts free blocks
int clear_pf(PLAYFIELD *pf)
{
unsigned char x,y;
register unsigned char *blocks;
unsigned char cnt=0;


//     #if !defined (NO2DIMARRAYS)
//     blocks=&pf->blk[0][0];
//     #else
//     blocks=&pf->blk[0+0];
//     #endif

    blocks=&GET2DIM(pf->blk,0,0,PF_VX);

    for(y=0;y<PF_Y+3;++y)
    {
         for(x=0;x<3;++x)
         {
                 *blocks++=1;
         }
         for(x=0;x<PF_X;++x)
         {
                 if(*blocks==NO_BLOCK)
                    {
                           cnt++;
                 }
                else
                {
                     *blocks=NO_BLOCK;
                }
                    ++blocks;
         }
         for(x=0;x<3;++x)
         {
                 *blocks++=1;
         }
    }
    for(;y<PF_VY;++y)
    {
         for(x=0;x<PF_VX;++x)
         {
                 *blocks++=1;
        }
    }

    pf->lines=0;

    return(cnt);
}

// collapse playfield (remove full lines)
int collapse_pf(PLAYFIELD *pf)
{
unsigned char x,y;
unsigned char *blocks;
unsigned char cnt=0,lines=0;

//     #if !defined (NO2DIMARRAYS)
//     blocks=&(pf->blk[(PF_Y+2)][PF_X+2]);
//     #else
//     blocks=&(pf->blk[((int)(PF_Y+2)*(int)PF_VX)+(int)PF_X+2]);
//     #endif

    blocks=&GET2DIM(pf->blk,(PF_Y+2),(PF_X+2),PF_VX);

    for(y=(PF_Y+2);y>1;)
    {
        cnt=0;
        for(x=0;x<PF_X;++x)
        {
             if(*blocks!=NO_BLOCK)
            {
                    ++cnt;
            }
            --blocks;
        }

          if(cnt==0) return(lines);

        if(cnt==PF_X)
        {
            // remove line

//             #if !defined (NO2DIMARRAYS)
//             memmove(&pf->blk[1][0],&pf->blk[0][0],y*PF_VX);
//             memset(&pf->blk[0][0],1,3);
//             memset(&pf->blk[0][3],NO_BLOCK,PF_X);
//             memset(&pf->blk[0][3+PF_X],1,3);
//             #else
//             memmove(&pf->blk[((int)1*(int)PF_VX)+0],&pf->blk[0+0],y*PF_VX);
//             memset (&pf->blk[0+0],1,3);
//             memset (&pf->blk[0+3],NO_BLOCK,PF_X);
//             memset (&pf->blk[0+3+PF_X],1,3);
//             #endif

             memmove(&GET2DIM(pf->blk,1,0,PF_VX),&GET2DIM(pf->blk,0,0,PF_VX),y*PF_VX);
             memset (&GET2DIM(pf->blk,0,0,PF_VX),1,3);
             memset (&GET2DIM(pf->blk,0,3,PF_VX),NO_BLOCK,PF_X);
             memset (&GET2DIM(pf->blk,0,3+PF_X,PF_VX),1,3);

            ++lines;
            // go back one line
//             blocks+=(PF_X);

            // advance to next line
            blocks-=6;
            --y;
        }
        else
        {
            // advance to next line
            blocks-=6;
            --y;
        }
    }
    pf->lines+=lines;
    return(lines);
}

// update one playfield
void update_pf_block(PLAYFIELD *pf)
{
unsigned char x,y,px,py,size;
register unsigned char blk;
register unsigned char *blocks;

    if(pf->gameover)
    {
        return;
    }

    //size=myblocks[pf->bn]->size;
    size=GET1DIM(myblocks,pf->bn)->size;
    py=pf->by;

    if(!((size+py)>3))
    {
        return;
    }
    else
    {

        if(py>3)
        {
            py--;
            size++;
//             #if defined(__GAMEBOY__)
//             /* neither lcc nor sdcc would compile the latter */
//             {
//     //        int i=(py*PF_VX)+3;
// //            blocks=pf->blk+i;
//             blocks=&pf->blk[(int)py][(int)3];
//             }
//             #else
//
//                 #if !defined (NO2DIMARRAYS)
//                 blocks=&pf->blk[py][3];
//                 #else
//                 blocks=&pf->blk[((int)py*(int)PF_VX)+(int)3];
//                 #endif
//
//             #endif


            blocks=&GET2DIM(pf->blk,py,3,PF_VX);

            py-=3;
        }
        else
        {
//             #if defined(__GAMEBOY__)
//             /* neither lcc nor sdcc would compile the latter */
//             {
//             //int i=(3*PF_VX)+3;
//             //blocks=pf->blk+i;
//             blocks=&pf->blk[(int)3][(int)3];
//             }
//             #else
//                 #if !defined (NO2DIMARRAYS)
//                 blocks=&pf->blk[3][3];
//                 #else
//                 blocks=&pf->blk[((int)3*(int)PF_VX)+(int)3];
//                 #endif
//             #endif

            blocks=&GET2DIM(pf->blk,3,3,PF_VX);

            py=0;

        }

        px=pf->px;

        for(y=0;(y<size)&(py<PF_Y);++y)
        {
            gotoxy(px,py);

            for(x=0;x<PF_X;++x)
            {
                    blk=*blocks++;
                    if(blk==NO_BLOCK)
                    {
                            cputc(BLK_NONE);
                    }
                    else
                    {
                            //textcolor(colortable[blk]);
                            textcolor(GET1DIM(colortable,blk));
                            revers(1);cputc(BLK_SET);revers(0);
                    }
            }
            blocks+=6;
                py++;
        }
    }

}

// update one playfield
void update_pf_all(PLAYFIELD *pf)
{
unsigned char x,y,px,py;
register unsigned char blk;
register unsigned char *blocks;

    if(pf->gameover)
    {
        return;
    }

    px=pf->px;py=0;

//     #if !defined (NO2DIMARRAYS)
//     blocks=&pf->blk[3][3];
//     #else
//     blocks=&pf->blk[((int)3*(int)PF_VX)+(int)3];
//     #endif

    blocks=&GET2DIM(pf->blk,3,3,PF_VX);

    for(y=0;y<PF_Y;++y)
    {
        gotoxy(px,py);

        for(x=0;x<PF_X;++x)
        {
                   blk=*blocks++;
                   if(blk==NO_BLOCK)
                   {
                              cputc(BLK_NONE);
                   }
                   else
                   {
                              //textcolor(colortable[blk]);
                              textcolor(GET1DIM(colortable,blk));
                              revers(1);cputc(BLK_SET);revers(0);
                   }
        }
        blocks+=6;
         py++;
    }

}

void update_score(PLAYFIELD *pf,unsigned char score)
{
    pf->score+=score;
    gotoxy(pf->px+4,PF_Y+1);
    textcolor(COLOR_YELLOW);
    cprintf("%05d",pf->score);
}

void update_stage(PLAYFIELD *pf,unsigned char stage)
{
    pf->stage=stage;
    if(stage<16)
    {
        pf->delayticks=256*(17-stage);
    }
    else
    {
        pf->delayticks=1;
    }

    gotoxy(pf->px+7,PF_Y+2);
    textcolor(COLOR_YELLOW);
    cprintf("%2d",stage);

    if(stage>1)
    {
           // a stage gives 200 points, plus one
        // point for each free block
        update_score(pf,200+clear_pf(pf));
    }

}

void update_lines(PLAYFIELD *pf,unsigned char lines)
{
    pf->lines+=lines;
    gotoxy(pf->px+4,PF_Y+3);
    textcolor(COLOR_YELLOW);
    cprintf("%2d/%2d",LINES_PER_STAGE-pf->lines,LINES_PER_STAGE);
       // each line gives 10 points, plus 1 for the block
    update_score(pf,(10*lines)+1);

}

/***********************************************************************************
    Ingame
***********************************************************************************/

void ingame_init(PLAYFIELD *pf)
{

#if defined (NOGLOBALPTRINIT)
    myblocks[0]=&blk2_1;
    myblocks[1]=&blk3_1;
    myblocks[2]=&blk3_2;
    myblocks[3]=&blk4_1;
#endif

    clear_pf(pf);
    // randomize two blocks in advance
    block_randomize(pf);
    block_next(pf);
    block_randomize(pf);
    block_printnext(pf);

    // position new block
    pf->bx=(PF_X/2);pf->by=0;

    pf->score=0;
    pf->stage=0;
    pf->lines=0;

    pf->gameover=0;
       update_pf_all(pf);

    update_score(pf,0);
    update_stage(pf,1);
    update_lines(pf,0);

}

void ingame_gameover(PLAYFIELD *pf)
{
        textcolor(COLOR_WHITE);
        cputsxy(pf->px+2,2,"game");
        cputsxy(pf->px+2,4,"over");
        cputsxy(pf->px+2,7,"press");
        cputsxy(pf->px+2,9,"button");
        pf->gameover=1;
}

unsigned char dokeys(PLAYFIELD *pf)
{
    if(pf->gameover)
    {
        // start a new game
        if(pf->key==TKEY_ROTATE)
        {
            ingame_init(pf);
            return(0);
        }
    }
    else
    {
        switch(pf->key)
        {
            // move left
            case TKEY_LEFT:
                    if(!block_checkhit_x(pf,3-1)) --pf->bx;
                       break;
            // move right
            case TKEY_RIGHT:
                    if(!block_checkhit_x(pf,3+1)) ++pf->bx;
                       break;
            // rotate
                 case TKEY_ROTATE:
                       pf->br = (pf->br + 1) & 3;
                    if(block_checkhit_x(pf,3))
                    {
                           pf->br = (pf->br - 1) & 3;
                    }
                       break;
            // drop
            case TKEY_DROP:
                    if(!block_checkhit_y(pf,1)) ++pf->by;
                       break;
        }
    }
    return(pf->key);
}


// move block down in constant speed according to current stage
void ingame_dropblock(PLAYFIELD *pf)
{
clock_t ticks;

        // get current tick count
        ticks=ticks_get();

        if((ticks-pf->lasttick)>(pf->delayticks))
        {
            pf->lasttick=ticks;
            if(!block_checkhit_y(pf,1)) ++pf->by;
        }
}


// ingame loop for one player, returns
// 1 on game over, otherwhise 0
unsigned char ingame_doplr(PLAYFIELD *pf)
{
unsigned char lines;

         if(pf->gameover)
        {
            dokeys(pf);
            if(pf->gameover)
            {
                       return(pf->gameover);
            }
        }

        // remove block from playfield
        block_unset(pf);

        // move block position according to controller
        if(dokeys(pf)!=TKEY_DROP)
        {
            // move block down in constant speed
            ingame_dropblock(pf);
        }

        // clear keys
        pf->key=0;

        // check if we did hit ground
        if(block_checkhit_y(pf,1))
        {
            // check if we are in visible area
            if(pf->by<2)
            {
                // set game over
                ingame_gameover(pf);
            }
            else
            {

                // put block on playfield
                block_set(pf,pf->bc);

                // remove complete lines
                if((lines=collapse_pf(pf))!=0)
                {
                    // each line gives 10 points, and each block 1 point
                    update_lines(pf,lines);

                    // check if stage completed
                    if(pf->lines>=LINES_PER_STAGE)
                    {
                            update_stage(pf,pf->stage+1);
                    }
                    // update complete playfield
                    update_pf_all(pf);
                    flashit(7);
                }
                // no lines removed
                else
                {
                        update_pf_block(pf);
                        // each block 1 point
                        update_score(pf,1);
                }

                // this block=next block
                block_next(pf);
                // next block=random
                block_randomize(pf);

                block_printnext(pf);
            }
        }
        //  we didnt hit ground
        else
        {

            // put block on playfield
            block_set(pf,pf->bc);

            // update part of playfield with block
            update_pf_block(pf);

        }
         return(pf->gameover);
}

void ingame(void)
{
unsigned char playing=0;
unsigned char plr=0;
unsigned char i;

    // enable keyboard repeat mode with smallest delay and fastest repeatrate
    kbrepeat(KBREPEAT_ALL);
//    kbrepeatrate(KB_REPEATRATE_0);
//    kbrepeatdelay(KB_REPEATDELAY_0);

    // ingame init
    clrscr();
    for(i=0;i<MAX_PLR;++i)
    {
        // position for the playfields on screen
        // each playfield is PF_X*PF_Y characters large on screen
        //pf[i].px=((SCREENX-(MAX_PLR*(PF_X+1)))/2)+(i*(PF_X+1)) ;
        GET1DIM(pf,i).px=((SCREENX-(MAX_PLR*(PF_X+1)))/2)+(i*(PF_X+1)) ;
        init_pf(i);
    }

//while(1){}

    while(!playing)
    {
             poll_controller();
            plr=0;
             for(i=0;i<MAX_PLR;++i)
             {
                 //plr+=ingame_doplr(&pf[i]);
                 plr+=ingame_doplr(&GET1DIM(pf,i));
//pf[(int)i].nn=i;block_printnext(&pf[(int)i]);
             }
             if(plr!=MAX_PLR) playing=1;

            ticks_do();
            waitvsync();
            flasher();

            conio_update();

    }

    while(playing)
    {
             poll_controller();
            plr=0;
             for(i=0;i<MAX_PLR;++i)
             {
//                 plr+=ingame_doplr(&pf[i]);
                 plr+=ingame_doplr(&GET1DIM(pf,i));
             }
             if(plr==MAX_PLR) playing=0;

            ticks_do();
            waitvsync();
            flasher();

            conio_update();

    }

    kbrepeat(KBREPEAT_ALL);

    domeltdown();
#if !defined(NOHISCORES)
    showhiscores();
#endif
}

void init_pf(unsigned char n)
{
PLAYFIELD *p;

    //p=&pf[n];
    p=&GET1DIM(pf,n);

    ingame_init(p);

//return;

    textcolor(COLOR_WHITE);
    gotoxy(p->px,PF_Y);chline(PF_X);

    #ifdef CONIOSCROLLS
    if((p->px+PF_X)==(SCREENX-1))
    {
    gotoxy(p->px+PF_X,0);cvline(SCREENY-1);
    }
    else
    {
    #endif
    gotoxy(p->px+PF_X,0);cvline(SCREENY);
    #ifdef CONIOSCROLLS
    }
    #endif

    if((n+1)==MAX_PLR)
    {
        cputcxy(p->px+PF_X,PF_Y,CH_RTEE);
    }
    else
    {
        cputcxy(p->px+PF_X,PF_Y,CH_CROSS);
    }

    // draw line at left side of first playfield
    // if odd screensize allows this
    if((n==0)&&(p->px>0))
    {
        cvlinexy(p->px-1,0,SCREENY);
        cputcxy(p->px-1,PF_Y,CH_LTEE);
    }

    cputsxy(p->px,PF_Y+1+2,"next");
      cputsxy(p->px+4,PF_Y+2,"lvl");

    ingame_gameover(p);

}

/***********************************************************************************
    Main Loop
***********************************************************************************/

//#include <heap.h>


//void cprintf(char *format,...);
//extern void cprintf(char *,...);

#ifdef __SDL__
int main(int argc, char *argv[])
#else
int main(void)
#endif
{
unsigned char finished=0;
//unsigned char i;

    // KLUDGES: switch to VDC screen
    #if defined (__VDC__)
unsigned char oldvideomode;
//         puts("switching to VDC...\n\r");
//         puts("\x1bx"); // send ESC-x to stdout (switch to VDC screen)
//         puts("\x0e"); // send asc 14 to stdout (toggle lowercase, workaround for buggy conio)
//         *((unsigned char*)0xf8)=0xff; // disable all scrolling (workaround for buggy conio)
        oldvideomode = videomode(VIDEOMODE_80COL);
    #endif

    #if defined(GFXCONIOINIT)
        gfx_conio_init();
    #endif

    #if defined(__GAMECUBE__)
    VIInit();
    PADInit();
    #endif

    #if defined(CONIOINIT)
    conio_init();
    #endif

    #if defined(__GAMECUBE__) | defined(__PSP__)
    conio_foreground();
    conio_disable_backscroll();
    #endif

//     while(1)
// {
// }
#if 0
     clrscr();
//    printk("123456789012345678901234567890123456789012345678901234567890");
  //printf ("%c[1mBold Text\n",27);
  //printf ("%c[2mDim text\n",27);
  for(i=0;i<15;i++)
  {
      //gotoxy(i,i);printf("%d",i);
   //  printk("%c%c%c%d",22,32+(23-i),32+(36-i),i);
     gotoxy(1,i);textcolor(i);cprintf("color %d",i);
 }
while(1)
{
}
  //title();
while(1)
{
}
#endif
    #if 0
    {
    conio_init();
    clrscr();

while(1)
{
    extern unsigned  _BSS_LOAD__,_BSS_RUN__,_BSS_SIZE__;
    extern unsigned  _DATA_LOAD__,_DATA_RUN__,_DATA_SIZE__;
    extern unsigned  _RODATA_LOAD__,_RODATA_RUN__,_RODATA_SIZE__;
    extern unsigned  _CODE_LOAD__,_CODE_RUN__,_CODE_SIZE__;
    extern unsigned  _STARTUP_LOAD__,_STARTUP_RUN__,_STARTUP_SIZE__;
    extern unsigned  _VECTORS_LOAD__,_VECTORS_RUN__,_VECTORS_SIZE__;
    extern unsigned  _ZEROPAGE_LOAD__,_ZEROPAGE_RUN__,_ZEROPAGE_SIZE__;

    //int a=0x1234;

    gotoxy(0,0);
    textcolor(1);bgcolor(9);
//    cputs("does it work?");
    //cprintf("trallalala");

//    gotoxy(3,3);cputs("1la");
//    gotoxy(4,6);cputs("2la");
//    gotoxy(5,7);cputs("3la");


    cprintf("code     load:%04x run:%04x size:%04x\n\r",&_CODE_LOAD__,&_CODE_RUN__,&_CODE_SIZE__);
    cprintf("startup  load:%04x run:%04x size:%04x\n\r",&_STARTUP_LOAD__,&_STARTUP_RUN__,&_STARTUP_SIZE__);
    cprintf("vectors  load:%04x run:%04x size:%04x\n\r",&_VECTORS_LOAD__,&_VECTORS_RUN__,&_VECTORS_SIZE__);
    cprintf("zeropage load:%04x run:%04x size:%04x\n\r",&_ZEROPAGE_LOAD__,&_ZEROPAGE_RUN__,&_ZEROPAGE_SIZE__);
    cprintf("rodata   load:%04x run:%04x size:%04x\n\r",&_RODATA_LOAD__,&_RODATA_RUN__,&_RODATA_SIZE__);
    cprintf("data     load:%04x run:%04x size:%04x\n\r",&_DATA_LOAD__,&_DATA_RUN__,&_DATA_SIZE__);
    cprintf("bss      load:%04x run:%04x size:%04x\n\r",&_BSS_LOAD__,&_BSS_RUN__,&_BSS_SIZE__);
    cprintf("heap     maxavail:%04x avail:%04x\n\r",_heapmaxavail(),_heapmemavail());
    //cprintf("%04x\n\r",a);
    //cprintf("heapavail:%04x\n\r",_heapmemavail());
    //conio_update();
    //while(1){}
    }
    }
    #endif

    #if 0
    {
    extern unsigned int _CONSTRUCTOR_TABLE__;
    extern unsigned char _CONSTRUCTOR_COUNT__;
    extern unsigned int _DESTRUCTOR_TABLE__;
    extern unsigned char _DESTRUCTOR_COUNT__;
    int i;
    clrscr();
    // select driver + init joystick api
    gotoxy(0,0);

    cputhex16(&_CONSTRUCTOR_COUNT__);
    cputs(" ");
    cputhex16((unsigned char)_CONSTRUCTOR_COUNT__);
    for(i=0;i<4;i++)
    {
//    cprintf(">%04x\n\r",(int)_CONSTRUCTOR_TABLE__[i]);
gotoxy(1,1+i);
    cputhex16((unsigned int)((&_CONSTRUCTOR_TABLE__)+i));
    cputs(" ");
    cputhex16(*(unsigned int*)((&_CONSTRUCTOR_TABLE__)+i));
    cputs(" ");
    cputhex16(*(unsigned int*)&_CONSTRUCTOR_TABLE__);
    //t++;
    //cputs(" ");
    //cputhex16((unsigned int)&_CONSTRUCTOR_TABLE__[i]);
    //cputs(" ");
    //cputhex16((unsigned int)_CONSTRUCTOR_TABLE__[i]);

    }

    for(i=0;i<4;i++)
    {
//    cprintf(">%04x\n\r",(int)_CONSTRUCTOR_TABLE__[i]);
gotoxy(1,5+1+i);
    cputhex16((unsigned int)((&_DESTRUCTOR_TABLE__)+i));
    cputs(" ");
    cputhex16(*(unsigned int*)((&_DESTRUCTOR_TABLE__)+i));
    cputs(" ");
    cputhex16(*(unsigned int*)&_DESTRUCTOR_TABLE__);
    //t++;
    //cputs(" ");
    //cputhex16((unsigned int)&_CONSTRUCTOR_TABLE__[i]);
    //cputs(" ");
    //cputhex16((unsigned int)_CONSTRUCTOR_TABLE__[i]);

    }

    while(1)
    {
    }
    }
    #endif

    #if 0
    {
    static char mystring[0x10]="abcdef";

    clrscr();
    // select driver + init joystick api
    gotoxy(0,0);
    cputs("str1:");cputs(VERSION);cputs("\n\r");
    cputs("str2:");cputs(mystring);cputs("\n\r");
//    cputs("str2:");cputhex16(mystring);cputs("\n\r");

        cprintf("str1:%s\n\r",VERSION);
        cprintf("str2:%s\n\r",mystring);
        cprintf("str2:%x\n\r",mystring);

//    init_joy();
    while(1)
    {

    //bgcolor(i&0xf);
    //textcolor(1);textbackground(-1);

    //gotoxy(0,0);
    //cputs("does it work?\n\r");
//    char c;
//    c=cgetc();
//    gotoxy(0,4);cprintf("><<%02x<\n",c);
        //gotoxy(0,0);textcolor(1);bgcolor(0);
        //textcolor((i++)&0xff);
    gotoxy(1,5);
    cprintf("%d\n",i++);
    cprintf("%d  %x\n",i++,joy_read(0));
    //cprintf("%d\n",MAX_PLR+7);
    //cprintf("%d\n",MAX_PLR);
//     cputs("does it work?\n");
    //textcolor(1);textbackground(3);
     //gotoxy(2,5);
     //cputs("abcdefghijklmnopqrstuvwxyz1234567890\n");
    //textcolor(5);textbackground(9);
     //gotoxy(2,8);
     //cputs("abcdefghijklmnopqrstuvwxyz1234567890\n");
//     gotoxy(wherex(),wherey());
//     cputs("AAAA\n");

    #if defined(CONIOUPDATE)
    conio_update();
    #endif
    }
    }
    #endif

    // make the screen all black (huu :=P)
    bgcolor(COL_BG); bordercolor(COL_BG); textcolor(COL_BG);clrscr();

    // select driver + init joystick api
    init_joy();

    #if !defined(NOHISCORES)
    inithiscore();
    #endif

    // reset timers
    ticks_reset();

    // enter the main loop
    while(!finished) {
        // the title screen
        switch(title()) {
            case 'q':
                    // quit
                    finished=1;
                break;
#if !defined(NOHISCORES)
            case 'h':
                    // display the hiscores
                    showhiscores();
                break;
#endif
            case 'c':
                    // configure controller
                    init_joy();
                break;
            case ' ':
                    // do ingame
                    ingame();
                break;
        }
    }

#ifndef CONIORESTORES
    clrscr();
#endif

    // KLUDGES: switch back to VIC screen
#if defined (__VDC__)
    cputs("switching to old video mode...\n\r");
    //puts("\x1bx"); // send ESC-x to stdout
    videomode(oldvideomode);
#endif

    // KLUDGES: enter geos desktop
#if defined(__GEOS__)
    EnterDeskTop();
#endif

    return (EXIT_SUCCESS);
}

#include "input.c"
#include "output.c"
#include "title.c"

#if !defined(NOHISCORES)
#include "hiscore.c"
#endif

#include "effects.c"
