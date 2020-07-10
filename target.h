

///////////////////////////////////////////////////////////////////////////
// define screensize and some other capabilities of the target
// include target specific headers
///////////////////////////////////////////////////////////////////////////

/*

** library options

conio features

#define SCREENX	  80
#define SCREENY	  25
- size of the conio screen in characters (not pixels)
#define NOCOLORS
- conio has no color support
#define NOBGCOLORS
- bgcolor() is not available
#define NOREVERS
- revers() is not available
#define CONIOINIT
- conio must be initialized before it can be used
  (calls conio_init())
#define CONIOUPDATE
- conio must be updated to make changes visible
  (calls conio_update())

input

#define NOJOYSTICKS
- target has no joystick support
#define NOKEYBOARD
- target has no keyboard support

misc

#define NOWAITVBLANK
- waitvblank() is not available
#define NOCLOCK
- clock() is not available
#define NOTVMODE
- get_tv() is not available

** portris options

#define NOMELTDOWNFX
- omit the "melt" effect (needs cpeekc(),cpeekcolor())

title screen

#define NOQUIT
- omit the "quit program" option from main menu

hiscore screen

#define NOHISCORES
- omit the hiscores alltogether
#define NOSTARSFX
- omit the "stars" effect (needs cpeekc())


*/

/* CBM targets (cc65) */

#if defined(__PET__)
#  define SCREENX	  80
#  define SCREENY	  25
#  define NOCOLORS
#  define NOBGCOLORS
#  define NOWAITVBLANK          // FIXME: missing in cc65 lib

#elif defined(__CBM510__)
#  define SCREENX	  40
#  define SCREENY	  25
#  define NOTVMODE              // FIXME: missing in cc65 lib
#  define NOKBREPEAT            // FIXME: missing in cc65 lib

#elif defined(__CBM610__)
#  define SCREENX	  80
#  define SCREENY	  25
#  define NOCOLORS
#  define NOBGCOLORS
#  define NOJOYSTICKS           // FIXME: missing in cc65 lib
#  define NOKBREPEAT            // FIXME: missing in cc65 lib
#  define NOWAITVBLANK          // FIXME: missing in cc65 lib

#elif defined(__C64__) && !defined(__GEOS__)

#if defined(__SOFT80__)
#  define SCREENX	  80
#  define SCREENY	  25
#else
#  define SCREENX	  40
#  define SCREENY	  25
#endif

#elif defined(__PLUS4__)
#  define SCREENX	  40
#  define SCREENY	  25

/* FIXME: file too large */
#elif defined(__C16__)
#  define SCREENX	  40
#  define SCREENY	  25
#  define NOKBREPEAT           // FIXME: missing in cc65 lib
#  define NOWAITVBLANK         // FIXME: missing in cc65 lib
//#define NOHISCORES
//#define NOSTARSFX
//#define NOMELTDOWNFX
//#define NOQUIT
//#define NOCOLORS
//#define NOBGCOLORS
//#define NOREVERS

#elif defined(__C128__)

#if defined (__VDC__)
#  define SCREENX	  80
#  define SCREENY	  25
#else
#  define SCREENX	  40
#  define SCREENY	  25
#endif

#elif defined(__VIC20__)

#if defined(__VIC20HACKED__)
// with hardware 26x25
#  define SCREENX	  26
#  define SCREENY	  25
#elif defined(__SOFT40__)
// with software 40x24
#  define SCREENX	  40
#  define SCREENY	  24
#  define NOCOLORS
#else
// with standard screensize
#  define SCREENX	  22
#  define SCREENY	  23
#endif

#undef COLOR_ORANGE
#undef COLOR_BROWN
#undef COLOR_LIGHTRED
#undef COLOR_GRAY1
#undef COLOR_GRAY2
#undef COLOR_LIGHTGREEN
#undef COLOR_LIGHTBLUE
#undef COLOR_GRAY3

#define COLOR_ORANGE 	       	COLOR_WHITE
#define COLOR_BROWN  	       	COLOR_WHITE
#define COLOR_LIGHTRED       	COLOR_RED
#define COLOR_GRAY1  	       	COLOR_WHITE
#define COLOR_GRAY2  	       	COLOR_WHITE
#define COLOR_LIGHTGREEN     	COLOR_GREEN
#define COLOR_LIGHTBLUE      	COLOR_BLUE
#define COLOR_GRAY3  	       	COLOR_WHITE

#elif defined(__GEOS__)
#  define SCREENX	  40
#  define SCREENY	  25
#  define NOCOLORS
#  define NOBGCOLORS
#  define NOJOYSTICKS		// why is that so?
#  define NOKBREPEAT
#  define NOCLOCK

#  define NOWAITVBLANK          // FIXME: missing in cc65 lib
#define NOSTARSFX
#define NOMELTDOWNFX
#define NOJOYSELECT

/* other cc65 targets */
#elif defined(__APPLE2ENH__)

// that ok ?
#  define SCREENX	  40
#  define SCREENY	  25
#  define NOCOLORS
#  define NOBGCOLORS
#  define NOJOYSTICKS
#  define NOKBREPEAT
#  define NOCLOCK

#  define NOTVMODE              // FIXME: missing in cc65 lib
#define NOSTARSFX
#define NOMELTDOWNFX

#elif defined(__APPLE2__)
// that ok ?
#  define SCREENX	  40
#  define SCREENY	  25
#  define NOCOLORS
#  define NOBGCOLORS
#  define NOJOYSTICKS
#  define NOKBREPEAT
#  define NOCLOCK

#  define NOWAITVBLANK          // FIXME: missing in cc65 lib
#  define NOTVMODE              // FIXME: missing in cc65 lib
#define CH_DEL          0x7F    // FIXME: missing in cc65 lib

#define NOSTARSFX
#define NOMELTDOWNFX

#elif defined(__ATARI__)
#  define SCREENX	  40
#  define SCREENY	  24	// its really 24 not 25!
#  define NOCOLORS
#  define NOKBREPEAT
#  define NOWAITVBLANK          // FIXME: missing in cc65 lib
#  define NOTVMODE              // FIXME: missing in cc65 lib

                                // FIXME: cpeekc() etc missing in lib
#define NOSTARSFX
#define NOMELTDOWNFX

#elif defined(__BBC__)
// that ok ?
#  define SCREENX	  40
#  define SCREENY	  25
#  define NOCOLORS
#  define NOBGCOLORS
#  define NOJOYSTICKS
#  define NOKBREPEAT
#  error "BBC is missing some library support - FIXME!"

#elif defined(__ATMOS__)
#  define SCREENX	  38
#  define SCREENY	  28
#  define NOCOLORS
#  define NOBGCOLORS
#  define NOJOYSTICKS
#  define NOKBREPEAT

#  define NOWAITVBLANK          // FIXME: missing in cc65 lib
#  define NOTVMODE              // FIXME: missing in cc65 lib
#  define NOMELTDOWNFX
#  define NOSTARSFX

#elif defined(__LUNIX__)
// that ok ?
#  define SCREENX	  40
#  define SCREENY	  25
#  define NOCOLORS
#  define NOBGCOLORS
#  define NOJOYSTICKS
#  define NOKBREPEAT
#  error "LUNIX is missing some library support - FIXME!"

#elif defined(__OSA65__)
// that ok ?
#  define SCREENX	  40
#  define SCREENY	  25
#  define NOCOLORS
#  define NOBGCOLORS
#  define NOJOYSTICKS
#  define NOKBREPEAT
#  error "OSA65 is missing some library support - FIXME!"

#elif defined(__NES__)
#  define SCREENX	  (32)
#  define SCREENY	  (28)
#  define NOCOLORS
#  define NOBORDER
#  define NOKBREPEAT
#  define NOJOYSELECT
#  define NOKEYBOARD
#  define NOQUIT

#  define NOJOYSTICKS       // FIXME
#  define NOSTARSFX         // FIXME

#define NOMELTDOWNFX
#define NO2DIMARRAYS
#define NOGLOBALPTRINIT

#elif defined(__PCE__)
#  define SCREENX	  (512/8)
#  define SCREENY	  (224/8)
#  define NOBORDER
#  define NOKBREPEAT
#  define NOJOYSELECT
#  define NOMELTDOWNFX
#  define NOSTARSFX
#  define NOREVERS
#  define NOKEYBOARD
#  define NOWAITVBLANK
#  define NOQUIT

#  define NOJOYSTICKS       // FIXME

/* program too large */
#elif defined(__GAMATE__)
#  define SCREENX	  (32)    // FIXME
#  define SCREENY	  (28)    // FIXME
#  define NOCOLORS
#  define NOBGCOLORS
#  define NOBORDER
#  define NOKBREPEAT
#  define NOJOYSELECT
#  define NOKEYBOARD
#  define NOQUIT

#  define NOJOYSTICKS       // FIXME
#  define NOSTARSFX         // FIXME

#define NOMELTDOWNFX
//#define NO2DIMARRAYS
//#define NOGLOBALPTRINIT
#define NOHISCORES

/* misc targets */

#elif defined(__GAMEBOY__)
#  define SCREENX	  (160/8)
#  define SCREENY	  (144/8)
#  define NOCOLORS
#  define NOBGCOLORS
#  define NOBORDER
#  define NOKBREPEAT
#  define NOJOYSELECT
#  define NOMELTDOWNFX
#  define NOKEYBOARD
#  define NOWAITVBLANK
#  define NOQUIT
#  define NOSTRUCTASSIGN
#  define NOCLOCK
#  define CONIOINIT

#define NO2DIMARRAYS
#define NOGLOBALPTRINIT
#undef __SDCC__
#undef __GAMEBOY__

#elif defined(__GBA__)
#  define SCREENX	  (240/8)
#  define SCREENY	  (160/8)
#  define NOBORDER
#  define NOKBREPEAT
#  define NOJOYSELECT
#  define NOMELTDOWNFX			// "out of memory"
#  define NOCLOCK
#  define NOKEYBOARD
#  define NOQUIT
#  define CONIOUPDATE
#  define CONIOINIT

#elif defined(__GP32__)
#  define SCREENX	 ((320/8))
#  define SCREENY	 ((240/8))
#  define NOBGCOLORS
#  define NOBORDER
#  define NOKBREPEAT
#  define NOJOYSELECT
#  define NOMELTDOWNFX
#  define NOSTARSFX
#  define NOKEYBOARD
#  define NOWAITVBLANK
#  define NOCLOCK
#  define CONIOUPDATE
#  define CONIOINIT

#elif defined(__SPECTRUM__)

// sinclair spectrum
//#define const
//#define register

#define SCREENX	32
#define SCREENY 24

// #define NO2DIMARRAYS
// #define NOGLOBALPTRINIT

//#  define NOCOLORS
//#  define NOBGCOLORS
#  define NOBORDER
//#  define NOREVERS
#  define NOKBREPEAT
#  define NOJOYSELECT
#  define NOMELTDOWNFX
#  define NOSTARSFX
//#  define NOKEYBOARD
#  define NOJOYSTICKS
#  define NOWAITVBLANK
#  define NOCLOCK

#define NO2DIMARRAYS
#define NOGLOBALPTRINIT
#define NOSTRUCTASSIGN

#elif defined(__MINGW32__)

//#  define SCREENX	  ScreenCols()
//#  define SCREENY	  ScreenRows()
#  define SCREENX	  80
#  define SCREENY	  25
//#  define NOCOLORS
#  define NOBGCOLORS
#  define NOJOYSTICKS
#  define NOKBREPEAT
//#  define NOCLOCK
#  define NOREVERS

//# define NOSTARSFX
//# define NOMELTDOWNFX

#define CONIOINIT
#define CONIOSCROLLS
#define CONIORESTORES
//#define CONIODYNSIZE

#elif defined(__LINUX__)

//#  define SCREENX	  ScreenCols()
//#  define SCREENY	  ScreenRows()
#  define SCREENX	  80
#  define SCREENY	  25
//#  define NOCOLORS
#  define NOBGCOLORS
#  define NOJOYSTICKS
#  define NOKBREPEAT
//#  define NOCLOCK
//#  define NOREVERS

# define NOSTARSFX
# define NOMELTDOWNFX

#define CONIOINIT
#define CONIOSCROLLS
#define CONIORESTORES
//#define CONIODYNSIZE

#elif defined(__MACOS__)


#ifdef __SDL__

//#  define SCREENX	  ScreenCols()
//#  define SCREENY	  ScreenRows()
#  define SCREENX	  40
#  define SCREENY	  25
//#  define NOCOLORS
//#  define NOBGCOLORS
#define NOBORDER
#  define NOJOYSTICKS
#  define NOKBREPEAT
#  define NOCLOCK
//#  define NOREVERS
//#define NOWAITVBLANK
# define NOSTARSFX
# define NOMELTDOWNFX

#define CONIOINIT
#define CONIOSCROLLS
#define CONIORESTORES
//#define CONIODYNSIZE
#  define CONIOUPDATE

#else

//#  define SCREENX	  ScreenCols()
//#  define SCREENY	  ScreenRows()
#  define SCREENX	  80
#  define SCREENY	  25
//#  define NOCOLORS
#  define NOBGCOLORS
#  define NOJOYSTICKS
#  define NOKBREPEAT
//#  define NOCLOCK
//#  define NOREVERS
//#define NOWAITVBLANK
# define NOSTARSFX
# define NOMELTDOWNFX

#define CONIOINIT
#define CONIOSCROLLS
#define CONIORESTORES
//#define CONIODYNSIZE

#endif

#elif defined(__PSP__)

#  define SCREENX	  60
#  define SCREENY	  34
#  define NOBORDER
#  define NOJOYSELECT
#  define CONIOUPDATE
#  define CONIOINIT
#  define CONIOSCROLLS
#  define NOKEYBOARD
#  define NOKBREPEAT

//#  define NOCOLORS
//#  define NOREVERS
//#  define NOBGCOLORS
#define STATICJOYDRV
//#  define NOJOYSTICKS
#define NOJOYSELECT
// mmmh...why doesnt that work?
#  define NOMELTDOWNFX
#  define NOSTARSFX

//#  define NOWAITVBLANK

//#  define NOCLOCK

#elif defined(__GAMECUBE__)

#  define SCREENX	  80
#  define SCREENY	  30
#  define NOBORDER
#  define NOJOYSELECT
#  define CONIOUPDATE
#  define CONIOINIT
#define CONIOSCROLLS
#  define NOKEYBOARD
#  define NOKBREPEAT

//#  define NOCOLORS
//#  define NOREVERS
//#  define NOBGCOLORS

//#  define NOJOYSTICKS

// mmmh...why doesnt that work?
//#  define NOMELTDOWNFX
//#  define NOSTARSFX

//#  define NOWAITVBLANK

#  define NOCLOCK

#else
#error "Unknown target system - FIXME!"
#endif

