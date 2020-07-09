
/***********************************************************************************
    Title Screen
***********************************************************************************/

const unsigned char *titlestrings[]=
{
   //12345678901234567890123456789012345678901234567890
    "  #####                   ##            ##      ",
    "  ##  ##                  ##            ##      ",
    " ##  ##  ######  ##  ##  #####  ##  ##     #####",
    " #####   ##  ##  #####   ##     #####  ## ##    ",
    " ##      ##  ##  ##      ##     ##     ## ######",
    "##      ##  ##  ##      ##  ## ##     ##     ## ",
    "##      ######  ##       ##### ##     ## #####  ",
};

   //12345678901234567890
const unsigned char *menustrings[]=
{
#if !defined(NOKEYBOARD)
    "H show HiScores",
# if !defined(NOJOYSELECT)
    "C toggles Controls",
# endif
# if !defined(NOQUIT)
    "Q Quit Program",
# endif
    "Press SPACE to start",
#else
    "show HiScores",
# if !defined(NOJOYSELECT)
    "toggle Controls",
# endif
# if !defined(NOQUIT)
    "Quit Program",
# endif
    "start new Game",
#endif
};

#define MENUITEMS (sizeof(menustrings)/sizeof(unsigned char*))

const unsigned char menukeys[MENUITEMS]=
{
    'h',
#if !defined(NOJOYSELECT)
    'c',
#endif
#if !defined(NOQUIT)
    'q',
#endif
    ' '
};
unsigned char menuitem=(MENUITEMS-1);

void title_printstring (unsigned char aaline, unsigned char nr, signed char off)
{
    register unsigned char x;
    register unsigned char c;
    register unsigned char *ptr;

    gotoxy (0, aaline);
    ptr=(unsigned char *) titlestrings[nr];
    if (off > 0) {
        ptr += off;
    }
    x=0;

    while (x < (SCREENX)) {
        if (off < 0) {
            ++off;
            cputc (' ');
        } else if ((c = *ptr) != '\0') {
              cputc (c);
            ++ptr;
        } else {
            cputc (' ');
        }
        ++x;
    }
}

void title_printcontroller(unsigned char type)
{
unsigned char i,ii,l,c,j;

    gotoxy(0,TOFFS6);cclear(SCREENX);
    gotoxy(0,TOFFS6+1);cclear(SCREENX);

    // controller info
    for(i=0,l=0;i<MAX_PLR;++l)
    {
        for(ii=0;ii<(SCREENX/20);++ii,++i)
        {
            #ifdef CONIODYNSIZE

            gotoxy((ii*20)+2,TOFFS6+l);

            #else

            // VOOODOOOO!!! :o)
            #if SCREENX < 40
            gotoxy(((SCREENX/2)-10)+(((SCREENX/(SCREENX/(SCREENX/2)))/8)),TOFFS6+l);
            //gotoxy((ii*20),TOFFS6+l);
            #elif SCREENX < 80
            gotoxy((ii*20)+(((SCREENX/(SCREENX/(SCREENX/2)))/8)),TOFFS6+l);
            //gotoxy((ii*20),TOFFS6+l);
            #else
            gotoxy((ii*20)+2,TOFFS6+l);
            //gotoxy((ii*20),TOFFS6+l);
            #endif

            #endif

            cprintf("plr %d:",i+1);
            if((type==1)&(i<numjoysticks))
            {
                cprintf("joystick %d",i+1);
            }
            else
            {
            #if !defined (NOKEYBOARD)
                for(j=0;j<4;++j)
                {
                    #if !defined (NO2DIMARRAYS)
                    c=joykeys[i][j];
                    #else
                    c=joykeys[(i*4)+j];
                    #endif
                    if(c==' ')
                    {
                        cputs("space");
                    }
                    else
                    {
                        cputc(c);
                    }
                    cputc(' ');
                }
            #else
                cputs("n/a");
            #endif
            }
        }
    }

}

// print the title screen
unsigned char title (void)
{
unsigned char i;
static unsigned char cnt;
static unsigned char ccnt;
signed char loff=0;
#if !defined(NOKEYBOARD)
unsigned char c;
#endif

    clrscr();

    textcolor(COLOR_LIGHTRED);
    // version

#ifdef CONIODYNSIZE

    gotoxy(0,TOFFS2);

    cprintf("v%s ",VERSION);
#if !defined(NOTVMODE)
    if(get_tv()==TV_NTSC) {
        cputs("(ntsc)");
    } else {
        cputs("(pal)");
    }
#endif
    cprintf(", %d players",MAX_PLR);

    textcolor(COLOR_WHITE);
    // copyright
    gotoxy(0,TOFFS5);
    cputs_c("(w) 2002-2020 by Groepaz/Solution");

#else

    #if SCREENX > 23
    gotoxy((SCREENX-24)/2,TOFFS2);
    #elif SCREENX > 17
    gotoxy((SCREENX-17)/2,TOFFS2);
    #else
    gotoxy(0,TOFFS2);
    #endif

    #if SCREENX > 23
    cprintf("v%s ",VERSION);
#if !defined(NOTVMODE)
    if(get_tv()==TV_NTSC) {
        cputs("(ntsc)");
    } else {
        cputs("(pal)");
    }
#endif
    cprintf(", %d players",MAX_PLR);
    #elif SCREENX > 17

    cprintf("v%s",VERSION);
#if !defined(NOTVMODE)
    if(get_tv()==TV_NTSC) {
        cputs("(ntsc)");
    } else {
        cputs("(pal)");
    }
#endif
    cprintf(",%dplr",MAX_PLR);

    #endif


//conio_update();
//return;    


    textcolor(COLOR_WHITE);
    // copyright
     gotoxy(0,TOFFS5);
    #if SCREENX < 26
           /*1234567890123456789012345*/
    cputs_c("(w)'03-'20 by GPZ/TSN");
    #else
           /*123456789012345678901234567890*/
    cputs_c("(w) 2003-2020 by Groepaz/Solution");
    #endif

    #endif

     gotoxy(0,TOFFS5+1);
           /*123456789012345678901234567890*/
    cputs_c("all rights reversed");

    textcolor(COLOR_YELLOW);
    title_printcontroller(usejoysticks);

    while (1)
    {
            cnt = ccnt;
            ++ccnt;
            waitvsync();
            flasher();
            // portris logo
               for(i=0;i<7;++i)
               {
                          textcolor(colortable[cnt&0xf]);
                          title_printstring(TOFFS1+i,i,loff);
                    ++cnt;
             }

               if (--loff == -SCREENX)
            {
                 // must match length of one line in the logo
                 loff = 48;
            }

            // print menu items

            textcolor(COLOR_WHITE);

            for(i=0;i<MENUITEMS;++i)
            {
                gotoxy(0,TOFFS3+i);
                if(i==menuitem)
                {
                    textcolor(COLOR_YELLOW);

                    if((ccnt&0x08)==0x08)
                    {
                            revers(1);
                            cputs_c(menustrings[i]);
                            revers(0);
                    }
                    else
                    {
                            #ifdef NOREVERS
                            cclear(SCREENX);
                            #else
                            cputs_c(menustrings[i]);
                            #endif
                    }
                    textcolor(COLOR_WHITE);

                }
                else
                {
                    cputs_c(menustrings[i]);
                }
            }

            // do background flashing

               if((ccnt&0x7f)==0)
            {
                flashit(15);
            }

#if !defined(NOKEYBOARD)
            if(kbhit())
            {
                c=cgetc();
                switch(c)
                {

#if !defined(NOJOYSELECT)
                    case 'c':
#endif
                    case ' ':
                    case 'q':
                    case 'h':
                        domeltdown();
                        return(c);
                    default:
                        break;
                }
            }
            else
#endif // NOKEYBOARD
            {
#if !defined (NOJOYSTICKS)

                poll_controller();
                for(i=0;i<MAX_PLR;++i)
                {
                    switch(pf[i].key)
                    {
                        case TKEY_DROP:
                            if(menuitem<(MENUITEMS-1))
                            {
                                ++menuitem;
                            }
                            break;
                        case TKEY_UP:
                            if(menuitem>0)
                            {
                                --menuitem;
                            }
                            break;
                        case TKEY_ROTATE:
                                domeltdown();
                                return(menukeys[menuitem]);
                            break;
                    }

                }
#endif // NOJOYSTICKS
            }

    #if defined(CONIOUPDATE)
    conio_update();
    #endif


    }

}



