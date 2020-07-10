===============================================================================
      #####                   ##            ##      
      ##  ##                  ##            ##      
     ##  ##  ######  ##  ##  #####  ##  ##     #####
     #####   ##  ##  #####   ##     #####  ## ##    
     ##      ##  ##  ##      ##     ##     ## ######
    ##      ##  ##  ##      ##  ## ##     ##     ## 
    ##      ######  ##       ##### ##     ## #####  
    
   a portable game which is somewhat similar to Tetris
===============================================================================

WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP


The bulk of this code was originally written almost 20 years ago, and since 
then was mostly used to test my crude "conio" implementations on various
targets (mostly consoles). A lot changed since then, the current goal is to
clean this up and make CC65 the primary target. 

You can find some other games with a similar target here:

https://github.com/StewBC/cc65-Chess
https://github.com/Fabrizio-Caruso/CROSS-CHASE

The conio improvemnt PR that has pushed me to put this on github is here:

https://github.com/cc65/cc65/pull/532

Everything here is very - and i mean VERY - messy, and you might have to wash 
your eyes with gasoline after looking at it. Don't say i didn't warn you :)

To build, type "make cc65". Just typing "make" will show a list of targets you
can use (some of which will not work, i guess).

You also have to edit the CC65HOME at the top of the Makefile eventually, and
perhaps adjust some pathes to the required tools.

To build the CBM programs, you will need additional tools in your path:

- pucrunch
- c1541 (from VICE)
- petcat (from VICE)

To build the PC-Engine image you need

- dd

To build the Apple2 and Apple2enh image you need

- Java
- AppleCommander-ac-1.6.0.jar

To build the gamate image (which doesnt work yet) you will need

- gamate-fixcart

Some things will probably also require a bash shell - this stuff must be found
and removed/replaced by generic makefile commands.

Everything below is copied together from the old and very outdated 
documentation and still needs a major overhaul.

WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP

===============================================================================

Portability:

no machine dependend parts, any target that has conio should work. (look at the 
source and define screen dimensions for new target etc)

Dependencies:

- conio output (required)
- conio input (optional, required for keyboard input)
- joystick api (optional, required for joystick input)
  (either keyboard or joystick is recommended, obviously :))
- file i/o (optional, required for saving hiscore)

-------------------------------------------------------------------------------

        compiles    works    # of plr    has color

cc65 standard targets:

c64        yes        ok          4           yes
c128       yes        ok          4           yes
c128 vdc   yes        ok          8           yes
c16        yes        ok          4           yes
plus4      yes        ok          4           yes
cbm510     yes        ok          4           yes
cbm610     yes        ok          8           no
pet        yes        ok          8           no
geos (c64) yes      conio bugs    4           no
atari      yes        ok          4           no
apple2     yes        ?           4 (?)       no (?)
atmos      yes        ok          3           no
nes        yes        ok          3           no

c64+soft80 yes        ok          8           yes
pcengine   yes        ok          6           yes
vic20exp   yes        ok          2           yes

lunix       no (library missing - FIXME)
bbc         no (library missing - FIXME)

vic20       no (program too large)
gamate      no (program too large)

custom/unsupported cc65 targets:

vic20exp+soft40 yes   ok          4           no

osa65       no (library missing - FIXME)

-----------------------------------------------------

Z80 Targets

Spectrum   yes      buggy       3           yes
Gameboy    yes      buggy       2           no

ARM Targets

GBA        yes      ok          3           yes
GP32       yes      ok          4           yes

PPC Targets

Gamecube   yes      ok          4           yes

===============================================================================

Status:

- more targets need a (well) working conio
- portable way to access directory needed
    (for user-selectable joy driver)
- portable (microseconds-) timer api needed
    (for better synchronization)
- exe-packer needed for apple2/atari and some cbm targets

--------------------------------------------------------------------------------

CBM targets (cc65):
-------------------

pet:
- fix keyboard repeat (available but does not work?)
- add waitvblank (possible?)
- configure CRTC to remove blank line between character rows

cbm510:
- add get_tv
- add kbrepeat
- cursor() doesnt seem to work

cbm610:
- add kbrepeat
- add waitvsync
- add joystick support

c64(all)/library
- implement opendir/readdir to get a portable way for supporting
  user-selectable drivers
- add joy driver for ninjas snes-pad adapter
- add joy driver for keyboard?

c64+soft80/library
- add/fix cpeekc (cant handle chars >0x7f atm)
- add/fix cpeekcolor

c16/plus4:
- fix detection in cbmloader

c16+plus4/library:
- fix color translation

c16:
- does not work (file too large?)
- add kbrepeat
- add waitvsync

c128:
- rewrite/modify 4-player adapter drivers for use in c128 mode
- modify soft-80 to work in c128 mode

c128(vdc):
- remove workaround for old "case" bug

vic20:

vic20+soft40/library:
- add soft40 implementation to cc65 lib
- add "hard26" implementation to cc65 lib (maybe?)
- add/fix cpeekchar (cant handle chars >0x7f atm)
- reset videomode after exit

--------------------------------------------------------------------------------


apple2:
- find an emulator for deving on linux, add generation of approperiate
  disc image to build process if necessary
- fix cpeekchar
[PORT COMPILES, BUT IS COMPLETELY UNTESTED !!!]

atari:
- rewrite the conio in a different mode so it can be used with 16 colors
- fix cpeekchar


nes:
- fix stack/heap placement (linker bug?)
- fix clock.s

pcengine:
- fix stack/heap placement (linker bug?)
- fix clock.s

geos:
- remove "EnterDeskTop()" ?

general:

- cpeekattrib for all targets, rewrite meltdown fx to use it (for testing)
- invent some api for scrolling sections of the console screen, rewrite
  the meltdown-fx to use it
- add blankscreen() for all targets and hide updates when switching screens
- rewrite menue output to use one common "printcentered" routine
  to save some memory
- keep highscorelist on disc/saveram
- handle the (unlikely :o)) case that the game "wraps around"
  due to excessive long (and skillful) playing :)
- also handle wrapping of score
- count free blocks to score at end of stage
- some kind of effect at end of stage
- "modern" mode with all possible and not just the 4 classic blocks
- title picture (?...maybe via tgi?!)
- music (?...goattracker player would come in mind)
- sound effects (invent something portable)

bugs:
- when dropping a block only ONE line is ever cleared at once, when two lines
  should have been cleared, this will happen when the next block is dropped

More Ports?

- (8bit targets) LUnix, Atari Lynx, Atari VCS2600 (hardly :o)), BBC-Micro...
- (16bit targets ???) SuperCPU, SNES, C=1 (?) ...
- (32bit targets ?!?!) Dreamcast, Playstation, Playstation2 (?), Gamecube (?) ...

===============================================================================

very old stuff:

v 0.0.2 can be found here:
http://hitmen.c02.at/files/cc65/65tris_0.0.2_90percent.tar.gz

very old "portlib" things, cc65 "compatible" conio for other targets, and some
things that have since then been merged into cc65 can be found here:
http://hitmen.c02.at/files/portlib/cc65-portlib.zip

very old PC version of the portlib stuff is here:
http://hitmen.c02.at/files/portlib/portlib_pc.zip

Makefile.attic:

There are also a lot of references to other targets, my old development setups
on various machines, my own libraries (which are not included here) and god
knows what. None of this will work right now, don't bother :)

===============================================================================

version log:

0.0.3   -   Atmos Port
        -   GBA Port
        -   GP32 Port
0.0.2:  -   "make all" compiles all working targets in one go
        -   some stuff prepared for ppl who might want to port the code
            to a yet not working target
        -   UZ fixed big-struct related compiler bug so now compilation -Osir works
        -   some little speed tweaks by UZ
        -   support for existing joystick api(s)
        -   "drop" button works :)
        -   vic20 conio fixes
        -   vic20 hacked screen (26*25 characters)
        -   c64 80-columns screen
        -   vic20 40-columns screen
        -   c64 joydriver for protovision 4 player adapter
        -   c64 joydriver for dxs/hit 4 player adapter
        -   added highscore list
        -   NES port, all conio etc added that is needed to run 65tetris
        -   PCEngine Port, also all conio stuff added that was needed
0.0.1:  -   inital release
 
