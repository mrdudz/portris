
/***********************************************************************************
    cozy background flash effect :=P
***********************************************************************************/

#if !defined(NOBGCOLOR)
static unsigned char flashing = 0xff;

void flashit(unsigned char n) 
{
    flashing = n;
}

void flasher(void) 
{
    if(flashing == 0xff) {
        bgcolor(COL_BG); bordercolor(COL_BG);
    } else {
        bgcolor(colortable[flashing]);
        bordercolor(colortable[flashing]);
        --flashing;
    }
}
#endif

/***********************************************************************************
    "Melt Down" Fade-Out Fx
***********************************************************************************/

#if !defined(NOMELTDOWNFX)

#if defined(MELTDOWNNOBUFFER)
/* this is a slightly slower implementation of the effect, which does not use
   any large RAM buffers */
void domeltdown(void)
{
unsigned char i, x, y, y2;
unsigned char ch, co, rv;
unsigned char offs;
unsigned char yo[SCREENX];

    // first check how many spaces we have at the top
    for(x = 0; x < SCREENX; ++x) {
        // skip blanks
        i = 0;
        for(y = 0; y < SCREENY; ++y) {
            gotoxy(x, y);
            if((cpeekc() == ' ') && (cpeekrevers() == 0)) {
                ++i;
            } else {
                break;
            }
        }
        yo[x] = i;
    }

    for(i = 0; i < (SCREENY + (SCREENY / 2)); ++i) {
        waitvsync();
        flasher();
#if !defined(NOKEYBOARD)
        if(kbhit()) {
            cgetc();
            break;
        }
#endif
        for(x = 0; x < SCREENX; ++x) {
            offs = rand() % 3;
            y2 = (SCREENY - 1) - offs;
            for(y = (SCREENY - 1); (y > yo[x]) && (y2 >= 0); --y, --y2) {
                gotoxy(x, y2);
                ch = cpeekc();
#ifndef NOCOLORS
                co = cpeekcolor();
#endif
#ifndef NOREVERS
                rv = cpeekrevers();
#endif
                gotoy(y);
#ifndef NOCOLORS
                textcolor(co);
#endif
#ifndef NOREVERS
                revers(rv);
#endif
                cputc(ch);
            }
            if (y < SCREENY) {
                gotoxy(x, y);
                revers(0);
                cputc(' ');
            }
            yo[x] += offs;
        }
    }
}

#else
void domeltdown(void)
{
unsigned char *buf;
unsigned char *b;
unsigned char *ys[SCREENX];
#ifndef NOCOLORS
unsigned char *cb;
unsigned char *ycs[SCREENX];
#endif
unsigned char yc[SCREENX];
unsigned char yo[SCREENX];

unsigned char x, y;
unsigned char i;

#ifdef NOCOLORS
    b = buf = (unsigned char*)calloc(SCREENX * SCREENY, 1);
#else
    b = buf = (unsigned char*)calloc(2 * SCREENX * SCREENY, 1);
    cb = &buf[SCREENX * SCREENY];
#endif

    /* abort if we couldnt allocated memory */
    if(b == NULL) {
        clrscr();
#ifdef DEBUG
        textcolor(COLOR_WHITE);
        bgcolor(COLOR_BLACK);
        cputs("out of memory!\n\r(domeltdown)");

        while(1) {
            conio_update();
        };
#endif
        return;
    }

    // first prepare the buffers
    for(x = 0; x < SCREENX; ++x) {
        ys[x] = b;
#ifndef NOCOLORS
        ycs[x] = cb;
#endif

        yc[x] = x & 5;
        yo[x] = 0;

        // skip blanks
        for(y = 0; y < SCREENY; ++y) {
            gotoxy(x, y);
            if((*b = cpeekc()) == ' ') {
                ++b;
#ifndef NOCOLORS
                ++cb;
#endif
                ++yo[x];
                ++ys[x];
#ifndef NOCOLORS
                ++ycs[x];
#endif
            } else {
                break;
            }
        }
        // copy rest of column
        for(; y < SCREENY; ++y) {
            gotoxy(x, y);
            *b = cpeekc();
            ++b;
#ifndef NOCOLORS
            gotoxy(x, y);
            *cb = cpeekcolor();
            ++cb;
#endif
        }

    }

    for(i = 0; i < (SCREENY * 5); ++i) {
        waitvsync();
        flasher();
#if !defined(NOKEYBOARD)
        if(kbhit()) {
            cgetc();
            break;
        }
#endif
        for(x = 0; x < SCREENX; ++x) {
            if(yo[x] < SCREENY) {
                if(yc[x] == 0) {
                    b = ys[x];
#ifndef NOCOLORS
                    cb = ycs[x];
#endif
                    cputcxy(x, yo[x], ' ');
                    ++yo[x];
#ifdef CONIOSCROLLS
                    if(x == (SCREENX - 1)) {
                        for(y = yo[x]; y < (SCREENY - 1); ++y) {
#ifndef NOCOLORS
                            textcolor(*cb);
                            ++cb;
#endif
                            cputcxy(x, y , *b);
                            ++b;
                        }
                    } else {
#endif

                        for(y = yo[x]; y < SCREENY; ++y) {
#ifndef NOCOLORS
                            textcolor(*cb);
                            ++cb;
#endif
                            cputcxy(x, y, *b);
                            ++b;
                        }

#ifdef CONIOSCROLLS
                    }
#endif
                    yc[x] = 1 + (rand()%3);
                } else {
                    --yc[x];
                }
            }
        }
#if defined(CONIOUPDATE)
    conio_update();
#endif

    }

    free(buf);
}
#endif

#endif // NOMELTDOWNFX


/***********************************************************************************
    Blinking Stars Background Fx
***********************************************************************************/

#if !defined(NOSTARSFX)

#define NUMSTARS        16
#define STARSPERFRAME   4

                                      //01234567
const unsigned char starchars[8 + 1] = ",-+*+-' ";
#ifndef NOCOLORS
const unsigned char starcolors[8] = {
    COLOR_GRAY1,
    COLOR_GRAY2,
    COLOR_GRAY3,
    COLOR_WHITE,
    COLOR_GRAY3,
    COLOR_GRAY2,
    COLOR_GRAY1
};
#endif
unsigned char starx[NUMSTARS];
unsigned char stary[NUMSTARS];
unsigned char starc[NUMSTARS];
unsigned char stard[NUMSTARS];

void initstars(void)
{
unsigned char i;

    for(i = 0;i < NUMSTARS; ++i) {
        starx[i] = rand() % SCREENX;
        stary[i] = rand() % SCREENY;
        stard[i] = 10 + (rand() % 200);
        starc[i] = 8 + (rand() % 50);
    }
}

void dostars(void)
{
unsigned char c, n;
static int i;

    for (n = 0; n < STARSPERFRAME; ++n) {
        ++i;
        if (i == NUMSTARS) {
            i = 0;
        }

        c = starc[i];
        if (c < 8) {
            gotoxy(starx[i], stary[i]);
            if(cpeekc() == starchars[(c - 1) & 7]) {
                gotoxy(starx[i], stary[i]);
                textcolor(starcolors[c]);
                cputc(starchars[c]);
                ++starc[i];
            } else {
                c = 8;
            }
        } else {
            if(c < stard[i]) {
                ++starc[i];
            } else {
                starx[i] = rand() % SCREENX;
                stary[i] = rand() % SCREENY;
                stard[i] = rand() % 100;
                starc[i] = 0;
            }
        }
    }
}
#endif // NOSTARSFX
