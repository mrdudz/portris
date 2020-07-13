
#ifndef INPUT_H_
#define INPUT_H_

#define TKEY_LEFT   1
#define TKEY_RIGHT  2
#define TKEY_UP     4
#define TKEY_DOWN   8
#define TKEY_BUTTON 16

#define TKEY_DROP   TKEY_DOWN
#define TKEY_ROTATE TKEY_BUTTON

extern unsigned char usejoysticks;
extern unsigned char numjoysticks;
extern unsigned char lastkey;

#if !defined (NO2DIMARRAYS)
extern const unsigned char joykeys[8][4];
#else
extern const unsigned char joykeys[8 * 4];
#endif

#if !defined (NOKEYBOARD)
unsigned char poll_key(unsigned char n);
#else
#define poll_key(x) 0
#endif

void init_joy(void);
#if !defined (NOJOYSTICKS)
unsigned char poll_joy(unsigned char n);
#endif

void poll_controller(void);

unsigned char keypressed(void);
void flushkeys(void);

#endif // INPUT_H_
