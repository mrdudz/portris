
#ifndef NOHISCORES

/***********************************************************************************
    Hiscore-List
***********************************************************************************/

HISCOREENTRY hiscorelist[10];

char checkifhiscore(unsigned short score)
{
    if(score > hiscorelist[9].score) {
        return(1);
    } else {
        return(0);
    }
}

void inithiscore(void)
{
char i;

    for(i = 0; i < 10; ++i) {
        hiscorelist[i].lastplr = 0xff;
        hiscorelist[i].score = 1;
        hiscorelist[i].stage = 1;
        hiscorelist[i].lines = 0;
        memset(hiscorelist[i].name, ' ', HS_NAMELEN);
        hiscorelist[i].name[HS_NAMELEN] = 0;
    }
}

void updatehiscore(void)
{
char p, i, ii;
unsigned short score;

    for(p = 0; p < MAX_PLR; ++p) {
        score=pf[p].score;
        if(checkifhiscore(score) == 1) {
            i = 9; while((score > hiscorelist[i - 1].score) && (i > 0)) {
                i--;
            }

            for(ii = 9; ii > i; --ii) {
#ifdef NOSTRUCTASSIGN
                memcpy(&hiscorelist[ii], &hiscorelist[ii - 1], sizeof(HISCOREENTRY));
#else
                hiscorelist[ii] = hiscorelist[ii - 1];
#endif
            }

            hiscorelist[i].score = score;
            hiscorelist[i].lastplr = p;
            memset(hiscorelist[i].name, ' ', HS_NAMELEN);
            hiscorelist[i].name[HS_NAMELEN] = 0;
            hiscorelist[i].lines = pf[p].lines;
            hiscorelist[i].stage = pf[p].stage;
        }
    }
}

void enterhiscore(void)
{
char p, i, ii, c, blink;
    for(p = 0; p < MAX_PLR; ++p) {
        flushkeys();

        for(i = 0; i < 10; ++i) {
            if(hiscorelist[i].lastplr == p) {

                gotoxy(((SCREENX - 21) / 2), 18);
                       //12345678901234567890123
                cprintf("Player %d - enter name", (p + 1));

                memset(hiscorelist[i].name, ' ', HS_NAMELEN);
                c = ii = 0;
                while(c != CH_ENTER) {
                    textcolor(COLOR_WHITE);
                    cclearxy(((SCREENX-22)/2)+6,6+i,HS_NAMELEN+1);
                    cputsxy(((SCREENX-22)/2)+6,6+i,hiscorelist[i].name);

                    do {
                        waitvsync();
                        flasher();
                        dostars();
                        gotoxy(((SCREENX - 22) / 2) + 6 + ii, 6 + i);
                        if(blink & 0x20) {
                            textcolor(COLOR_WHITE);
                            cputc(hiscorelist[i].name[ii]);
                        } else {
                            textcolor(COLOR_YELLOW);
#ifdef NOREVERS
                            cputc('*');
#else
                            revers(1);
                            cputc(hiscorelist[i].name[ii]);
                            revers(0);
#endif
                        }
                        ++blink;
#if defined(CONIOUPDATE)
                        conio_update();
#endif
                    } while((!keypressed()) || (blink & 0x07));

#if !defined(NOKEYBOARD)
                    if(kbhit()) {
                        c = cgetc();
                        switch(c) {
/* if CH_DEL does not exist for the target, use cursore left instead */
#ifdef CH_DEL
                            case CH_DEL:
#else
                            case CH_CURS_LEFT:
#endif
                                    if(ii > 0) {
                                        ii--;
                                    }
                                    hiscorelist[i].name[ii] = ' ';
                                break;
                            case CH_ENTER:
                                break;
                            default:
                                    if(ii < HS_NAMELEN) {
                                        hiscorelist[i].name[ii] = c;
                                        ii++;
                                    }
                                break;
                        }
                    }
#elif !defined(NOJOYSTICKS)
                    {
                    unsigned char ch;
                        switch(pf[p].key) {
                            case TKEY_UP:
                                    ch = hiscorelist[i].name[ii] + 1;
                                    if(ch > 0x7e) {
                                        ch = 0x20;
                                    }
                                    hiscorelist[i].name[ii] = ch;
                                    blink = 0x20;
                                break;
                            case TKEY_DOWN:
                                    ch = hiscorelist[i].name[ii] - 1;
                                    if(ch < 0x20) {
                                        ch = 0x7e;
                                    }
                                    hiscorelist[i].name[ii] = ch;
                                    blink = 0x20;
                                break;
                            case TKEY_LEFT:
                                    if(ii > 0) {
                                        ii--;
                                    }
                                    blink = 0;
                                break;
                            case TKEY_RIGHT:
                                    if(ii < (HS_NAMELEN - 1)) {
                                        ++ii;
                                    }
                                    blink=0;
                                break;
                            case TKEY_BUTTON:
                                    c = CH_ENTER;
                                break;
                        }
                    }
#endif
                }
                hiscorelist[i].lastplr = 0xff;
                pf[p].score = 0;
                pf[p].stage = 1;
                pf[p].lines = 0;

            }
        }
        gotoxy(((SCREENX - 22) / 2), 18);
        cclear(SCREENX - ((SCREENX - 22) / 2));
        printhiscores();
    }

}

void printhiscores(void)
{
unsigned char i;
    textcolor(COLOR_YELLOW);
              /*123456789012345678901234567890*/
    cputsy_c(2,"portris Hall of Fame");

    textcolor(COLOR_RED);

#ifdef CONIODYNSIZE
              /*123456789012345678901234567890*/
    cputsy_c(4,"score name stage+lines");
#else
# if SCREENX > 21
              /*123456789012345678901234567890*/
    cputsy_c(4,"score name stage+lines");
# else
              /*123456789012345678901234567890*/
    cputsy_c(4,"score name level");
# endif
#endif

    textcolor(COLOR_WHITE);
    for(i = 0; i < 10; i++) {
        gotoxy(((SCREENX - 22) / 2), 6 + i);
        cprintf("%5d %10s %2d+%2d",
            hiscorelist[i].score,
            hiscorelist[i].name,
            hiscorelist[i].stage,
            hiscorelist[i].lines
            );
    }

#if defined(CONIOUPDATE)
    conio_update();
#endif
}

void showhiscores(void)
{
    clrscr();
    initstars();

    updatehiscore();

    printhiscores();

    enterhiscore();

    flushkeys();

    while(!keypressed()) {
#if !defined(NOWAITVBLANK)
        waitvsync();
#endif
        flasher();
        dostars();
#if defined(CONIOUPDATE)
        conio_update();
#endif
    }

    flushkeys();

    domeltdown();
}

#endif
