
#ifndef NOHISCORES

#ifndef HISCORE_H_
#define HISCORE_H_

// 10 is the maximum that will still display right on vic20 (23 chars/line)
#define HS_NAMELEN 10

typedef struct
{
    char name[HS_NAMELEN + 1];
    unsigned int score;
    unsigned int lines;
    unsigned int stage;
    char lastplr;
}
HISCOREENTRY;

void inithiscore(void);
void showhiscores(void);
void printhiscores(void);
void enterhiscore(void);
char checkifhiscore(unsigned short score);
void updatehiscore(void);

#endif

#endif
