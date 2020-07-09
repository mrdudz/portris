
/***********************************************************************************

    Controller related

    please notice that using conio for input (in a game) is weak and will only work
    halfway decent for one player.

    please also notice that due to the nature of tetris, a rather simple way
    of handling the controllers is still sufficient here, in most other games you
    would probably want something that can handle diagonal movement for example :)

***********************************************************************************/

#ifdef NOJOYSTICKS
unsigned char usejoysticks = 0;
#else
unsigned char usejoysticks = 1;
#endif

unsigned char numjoysticks = 0;
unsigned char lastkey = 0;

#ifdef NOJOYSELECT
const unsigned char *drivernames[5] =
{
// WTF?!?
//(unsigned char *)joy_stddrv,
NULL
};
#else

#if !defined (NOJOYSTICKS)
// FIXME: make a generic version using opendir/readdir
unsigned char *drivernames[5]=
{
"none (keyboard)",
(unsigned char *)joy_stddrv,
//"c64-stdjoy.joy",
"c64-ptvjoy.joy",
"c64-hitjoy.joy",
NULL
};
#ifdef __C64__
char numdrivers=(1+1+2);
#else
char numdrivers=(1+1);
#endif
#endif

#endif // NOJOYSELECT

// load and init joydriver
void init_joy(void)
{
    #if !defined (NOJOYSTICKS)
    unsigned char Res,x;
    #ifndef NOJOYSELECT
    unsigned char i;
    #endif

    numjoysticks=0; usejoysticks=0;
    clrscr();

    #ifndef NOJOYSELECT
    textcolor(COLOR_WHITE);
    cputsxy(0,0,"select input driver\r\n\n");

    for(i=0;i<numdrivers;++i)
    {
        cprintf("%1d) %s\n\r",i,drivernames[i]);
    }

    cursor(1);cputs("\n>");
    while((x<'0')|(x>('9'+numdrivers)))
    {
        x=cgetc();
    }
    cursor(0);

    x=(x-'0');
    if(x==0) return;    // keyboard/no driver

    cprintf("\n\rloading %d:%s...\n\r",x,drivernames[x]);
    #else
    x=1;    // select default driver
    #endif

    #ifdef STATICJOYDRV
        numjoysticks=joy_count();
        usejoysticks=1;
    #else
    Res = joy_load_driver (drivernames[x]);

    if (Res != JOY_ERR_OK) 
    {
           cprintf ("Error in joy_load_driver: %u\r\n", Res);
        cprintf ("os: %u, %s\r\n", _oserror, _stroserror (_oserror));

        #if 0
        while(!kbhit())
        {
            /* wait */
        } cgetc();
        #endif

        // move into variable to get rid of compiler warning
        // "statement has no effect"
        x=cgetc();
           //exit (EXIT_FAILURE);
        }
    else
    {
        numjoysticks=joy_count();
        usejoysticks=1;
    }
    #endif

    #endif
}

#if !defined (NOKEYBOARD)
#    if !defined (NO2DIMARRAYS)
const unsigned char joykeys[8][4]=
{
    {'a','s','l',' '},
    {'7','8','9','0'},
    {'q','w','e','r'},
    {'u','i','o','p'},
    {'1','2','3','4'},
    {'g','h','j','k'},
    {'5','6','t','z'},
    {'v','b','n','m'},
};
#    else
const unsigned char joykeys[8*4]=
{
    'a','s','l',' ',
    '7','8','9','0',
    'q','w','e','r',
    'u','i','o','p',
    '1','2','3','4',
    'g','h','j','k',
    '5','6','t','z',
    'v','b','n','m',
};
#    endif
#endif

unsigned char poll_key(unsigned char n)
{
#if !defined (NOKEYBOARD)
const unsigned char *keys;

    keys=&GET2DIM(joykeys,n,0,4);

    if(lastkey==keys[0]) {
        lastkey=0;
         return TKEY_LEFT;
    }
    else if(lastkey==keys[1])
    {
        lastkey=0;
        return TKEY_RIGHT;
    }
    else if(lastkey==keys[2])
    {
        lastkey=0;
        return TKEY_ROTATE;
    }
    else if(lastkey==keys[3])
    {
        lastkey=0;
        return TKEY_DROP;
    }
    else
    {
        return(0);
    }
#else
        return(0);
#endif
}

#if !defined (NOJOYSTICKS)
// read state of a controller
unsigned char poll_joy(unsigned char n)
{
int j; // should be int so it works with >8 button devices

    j = joy_read (n);

    if (JOY_DOWN(j)) return TKEY_DROP;
    else if (JOY_UP(j)) return TKEY_UP;
    else if (JOY_LEFT(j)) return TKEY_LEFT;
    else if (JOY_RIGHT(j)) return TKEY_RIGHT;
    else if (JOY_BTN_1(j)) return TKEY_ROTATE;

    else return(0);
}
#endif

// read state of all controllers in use
void poll_controller(void)
{
unsigned char j;

    #if !defined (NOKEYBOARD)
    // read eventually pressed key
    if(kbhit())
    {
        lastkey=cgetc();
    }
    #endif

    for(j=0;j<MAX_PLR;++j)
    {
        #if defined (NOJOYSTICKS)
            // use conio for input if no joysticks available
                pf[j].key=poll_key(j);
        #else

        // use conio for input if joysticks disabled by user
        if(!usejoysticks)
        {
                pf[j].key=poll_key(j);
        }
        else
        {
            // use joysticks for as many player as the driver supports
            if(j<numjoysticks)
            {
                pf[j].key=poll_joy(j);
            }
            // use keyboard for players that dont have
            // a joystick
            else
            {
                pf[j].key=poll_key(j);
            }

        }

        #endif

    }

}

void flushkeys(void)
{
unsigned char i,ii;

    #if !defined (NOKEYBOARD)
    // eat any remaining chars in keyboard buffer
    while(kbhit())
    {
        cgetc();
    }
    #endif // NOKEYBOARD

    #if !defined (NOJOYSTICKS)
    do
    {
        //poll_controller();
        for(i=ii=0;i<numjoysticks;++i)
        {
            ii|=poll_joy(i);
        }
    } while (ii!=0);
    #endif // NOJOYSTICKS

}

unsigned char keypressed(void)
{
#if !defined (NOJOYSTICKS)
unsigned char i,ii;
#endif
#if !defined (NOKEYBOARD)
    if(kbhit()) return(1);
#endif
#if !defined (NOJOYSTICKS)
    poll_controller();
    for(i=ii=0;i<numjoysticks;++i)
    {
        if((pf[i].key=poll_joy(i))!=0)
        {
            ii=1;
        }
    }
    return (ii);
#else
    return (0);
#endif // NOJOYSTICKS
}


