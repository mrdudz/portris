
#ifndef EFFECTS_H_
#define EFFECTS_H_

#ifdef NOMELTDOWNFX
# define domeltdown()
#else
void domeltdown(void);
#endif

#ifdef NOSTARSFX
# define initstars()
# define dostars()
#else
void dostars(void);
void initstars(void);
#endif

#if !defined(NOBGCOLOR)
/* renamed, flash() clashes with ncurses */
void flashit(unsigned char n);
void flasher(void);
#else
# define flashit(_x)
# define flasher()
#endif

#endif // EFFECTS_H_
