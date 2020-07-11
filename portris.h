
#if defined(__CC65__)
// use static locals for speed
#pragma static-locals(1)
#endif    // compilers

#include "target.h"

//#define CHARINDEX(_a)    (int)(_a)
#define CHARINDEX(_a)    (_a)

#define GET1DIM(_a,_x) _a[CHARINDEX(_x)]

#if !defined (NO2DIMARRAYS)
#define GET2DIM(_a,_y,_x,_nx) _a[CHARINDEX(_y)][CHARINDEX(_x)]
#else
#define GET2DIM(_a,_y,_x,_nx) _a[((int)_y*(int)_nx)+(int)_x]
#endif


///////////////////////////////////////////////////////////////////////////
// nullify some functions that would otherwhise just waste memory

#ifdef NOCOLORS
#ifdef textcolor
    #undef textcolor
#endif
#ifndef textcolor
    #define textcolor(_x)
#endif
#endif

#ifdef NOBGCOLORS

#ifdef bgcolor
    #undef bgcolor
#endif
#ifndef bgcolor
    #define bgcolor(_x)
#endif

#ifdef bordercolor
    #undef bordercolor
#endif
#ifndef bordercolor
    #define bordercolor(_x)
#endif

#elif defined(NOBORDER)
#ifdef bordercolor
    #undef bordercolor
#endif
    #define bordercolor(_x)
#endif

#ifdef NOKEYBOARD
    #define cgetc() 0
    #define kbhit() 0
    #ifndef NOKBREPEAT
    #    define NOKBREPEAT
    #endif
#endif

#ifdef NOKBREPEAT
#ifdef kbrepeat
    #undef kbrepeat
#endif
#ifdef kbrepeatrate
    #undef kbrepeatrate
#endif
#ifdef kbrepeatdelay
    #undef kbrepeatdelay
#endif
    #define kbrepeat(_x)
    #define kbrepeatrate(_x)
    #define kbrepeatdelay(_x)
#endif

#ifdef NOJOYSTICKS
#  define NOJOYSELECT
#endif

#ifdef NOREVERS
#ifdef revers
    #undef revers
#endif
    #define revers(_x)
#endif

#ifdef NOWAITVBLANK
#ifdef waitvsync
    #undef waitvsync
#endif
    #define waitvsync()
#endif

#if !defined(CONIOINIT)
    #define conio_init()
#endif

#if !defined(CONIOUPDATE)
    #define conio_update()
#endif

///////////////////////////////////////////////////////////////////////////
// define some stuff to avoid absolute values in the code

// size of playfield in characters
#define PF_X     9
#define PF_Y    ((SCREENY)-4)
// size of virtual playfield in characters
#define PF_VX    ((PF_X)+3+3)
#define PF_VY    ((PF_Y)+3+3)

// adjust number of players to screen width
#define MAX_PLR      ((SCREENX)/(PF_X+1))

// define the shape of the blocks
#ifdef NOREVERS
//    #define BLK_SET  '#'
    #define BLK_SET  'O'
#else
    #define BLK_SET  ' '
#endif

#define BLK_NONE ' '

// lines needed to complete a stage, usually 25
//#  define LINES_PER_STAGE 1
#  define LINES_PER_STAGE 25

// color to use for background, ie usually black
#define COL_BG    COLOR_BLACK

// define some values used for screen-layout

#ifdef CONIODYNSIZE

// logo
#define TOFFS1    1
// version string
#define TOFFS2    9
// menu strings
#define TOFFS3    12
// copyright
#define TOFFS5    19
// controller
#define TOFFS6    22

#else

#if SCREENY < 20

// logo
#define TOFFS1    0
// version string
#define TOFFS2    8
// menu strings
#define TOFFS3    10
// copyright
#define TOFFS5    13
// controller
#define TOFFS6    16

#elif SCREENY < 25

// logo
#define TOFFS1    0
// version string
#define TOFFS2    8
// menu strings
#define TOFFS3    12
// copyright
#define TOFFS5    18
// controller
#define TOFFS6    21

#else

// logo
#define TOFFS1    1
// version string
#define TOFFS2    9
// menu strings
#define TOFFS3    12
// copyright
#define TOFFS5    19
// controller
#define TOFFS6    22

#endif

#endif
