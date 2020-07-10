
.SILENT:

#CC65HOME=~/Desktop/cc65/github-current/cc65/bin/
CL65=$(CC65HOME)cl65

CC65FLAGS+=-Osir
JOYDRV := $(shell $(CL65) --print-target-path)

# c64/cbm tools
PUCRUNCH=pucrunch
EXOMIZER=exomizer
C1541=c1541
PETCAT=petcat

# apple2 tools
ACMD=~/bin/AppleCommander-ac-1.6.0.jar

# gamate tools
GFIX=gamate-fixcart

# generic tools
CP=cp
DD=dd
RM=rm -f

DEVNULL=/dev/null

##########################################################################################

.PHONY: all clean nice targets cc65

CBMTARGETS=c64 c6480 c6480m c128 c128vdc pet plus4 cbm510 cbm610 vic c16 geos
#vichacked vic40 gamate

CC65TARGETS=apple2 apple2e atari nes pcengine atmos
CC65TARGETS+=$(CBMTARGETS) cbmloader cbmdisk geosdisk

help:
	@echo available targets are:
	@echo all clean nice cbm cc65 $(CC65TARGETS)
	@echo look in the makefile for more.

all: clean targets nice

cc65targets: $(CC65TARGETS)
cc65: cc65targets

SOURCEFILES=\
	blocks.c \
	effects.c \
	effects.h \
	hiscore.c \
	hiscore.h \
	input.c \
	input.h \
	main.c \
	output.c \
	output.h \
	portris.h \
	target.h \
	title.c \
	title.h

##########################################################################################
#	6502/cc65
##########################################################################################

.PHONY: cbmloader c64 c128 c128vdc plus4 c16 vic20 cbm510 cbm610 pet atari apple2 apple2e geos geosdic cbmdisk

cbmloader: loader.bas
	@echo "cbm loader..."
	$(PETCAT) -w2 -l 0401 loader.bas > loader.prg 2> $(DEVNULL)

cbm: cbmloader $(CBMTARGETS)
	
c64: $(SOURCEFILES)
	@echo "c64..."
	$(CL65) $(CC65FLAGS) -t c64 -o portris_c64.bin main.c
	$(PUCRUNCH) -c64 -x2061 portris_c64.bin portris_c64.prg 1> $(DEVNULL) 2> $(DEVNULL)
#	$(EXOMIZER) -q -s 2061 -o portris_c64.prg portris_c64.bin

c6480: $(SOURCEFILES)
	@echo "c64 + soft80..."
	$(CL65) $(CC65FLAGS) -o portris_c64_80x25.bin -D__SOFT80__ main.c c64-soft80.o
	$(PUCRUNCH) -c64 -x2061 portris_c64_80x25.bin portris_c64_80x25.prg 1> $(DEVNULL) 2> $(DEVNULL)
#	$(EXOMIZER) -q -s 2061 -o portris_c64_80x25.prg portris_c64_80x25.bin

c6480m: $(SOURCEFILES)
	@echo "c64 + soft80..."
	$(CL65) $(CC65FLAGS) -o portris_c64_80x25m.bin -D__SOFT80__ main.c c64-soft80mono.o
	$(PUCRUNCH) -c64 -x2061 portris_c64_80x25m.bin portris_c64_80x25m.prg 1> $(DEVNULL) 2> $(DEVNULL)
#	$(EXOMIZER) -q -s 2061 -o portris_c64_80x25m.prg portris_c64_80x25m.bin

c128: $(SOURCEFILES)
	@echo "c128 ..."
	$(CL65) $(CC65FLAGS) -t c128 -o portris_c128.bin main.c
	$(PUCRUNCH) -c128 -x7181 portris_c128.bin portris_c128.prg 1> $(DEVNULL) 2> $(DEVNULL)

c128vdc: $(SOURCEFILES)
	@echo "c128 + vdc..."
	$(CL65) $(CC65FLAGS) -o portris_c128_vdc.bin -t c128 -D__VDC__ main.c
	$(PUCRUNCH) -c128 -x7181 portris_c128_vdc.bin portris_c128_vdc.prg 1> $(DEVNULL) 2> $(DEVNULL)

plus4: $(SOURCEFILES)
	@echo "plus4 ..."
	$(CL65) $(CC65FLAGS) -o portris_plus4.bin -t plus4 main.c
	$(PUCRUNCH) -c16 -x4109 portris_plus4.bin portris_plus4.prg 1> $(DEVNULL) 2> $(DEVNULL)

c16: $(SOURCEFILES)
	@echo "c16 ..."
	$(CL65) $(CC65FLAGS) -o portris_c16.bin -t c16 -C c16-32k.cfg main.c
	$(PUCRUNCH) -c16 -x4109 portris_c16.bin portris_c16.prg 1> $(DEVNULL) 2> $(DEVNULL)

# only works for expanded machines (NOT with default config for vic!)
vic: $(SOURCEFILES)
	@echo "vic20 (expanded) ..."
	$(CL65) $(CC65FLAGS) -o portris_vic20.bin -t vic20 -C vic20-32k.cfg main.c
	$(PUCRUNCH) -c20 -x4621 portris_vic20.bin portris_vic20.prg 1> $(DEVNULL) 2> $(DEVNULL)
	
# FIXME: needs not yet merged "hard26" hack
vichacked: $(SOURCEFILES)
	@echo "vic20 (expanded) + hard26..."
	$(CL65) $(CC65FLAGS) -o portris_vic20_26x25.bin -t vic20 \
	-C $(PARCH)/vic20/vic20exp.x -D__VIC20HACKED__ main.c \
		$(PLIB)/vic20hard26port.lib

	$(PUCRUNCH) -c20 -x4621 portris_vic20_26x25.bin portris_vic20_26x25.prg 1> $(DEVNULL) 2> $(DEVNULL)

# FIXME: needs not yet merged "soft40" lib
vic40: $(SOURCEFILES)
	@echo "vic20 (expanded) + soft40..."
	$(CL65) $(CC65FLAGS) -t vic20 -o portris_vic20_40x24.bin -D__SOFT40__ -C vic20-32k.cfg main.c vic20-soft40.o
		
#	$(CL65) $(CC65FLAGS) -o portris_vic20_40x24.bin -t vic20 \
#	-C $(PARCH)/vic20/soft40/vic20exp.x -D__SOFT40__ main.c \
#		$(PLIB)/vic20soft40port.lib
	@$(PUCRUNCH) -c20 -x5645 portris_vic20_40x24.bin portris_vic20_40x24.prg 1> $(DEVNULL) 2> $(DEVNULL)


pet: $(SOURCEFILES)
	@echo "pet ..."
	$(CL65) $(CC65FLAGS) -o portris_pet.prg -t pet main.c

cbm510: $(SOURCEFILES)
	@echo "cbm510 ..."
	$(CL65) $(CC65FLAGS) -o portris_cbm510.prg -t cbm510 main.c

cbm610: $(SOURCEFILES)
	@echo "cbm610 ..."
	$(CL65) $(CC65FLAGS) -o portris_cbm610.prg -t cbm610 main.c

cbmdisk: $(CBMTARGETS) cbmloader
	@echo "cbm disc ..."
	$(C1541) -format portris,00 d64 portris.d64 \
	-write loader.prg portris \
	-write portris_c64.prg portris-c64 \
	-write portris_c64_80x25.prg portris-c64-80 \
	-write portris_c64_80x25m.prg portris-c64-80m \
	-write portris_c128.prg portris-c128 \
	-write portris_c128_vdc.prg portris-c128vdc \
	-write portris_vic20.prg portris-vic20 \
	-write portris_c16.prg portris-c16 \
	-write portris_plus4.prg portris-plus4 \
	-write portris_pet.prg portris-pet \
	-write portris_cbm510.prg portris-cbm510 \
	-write portris_cbm610.prg portris-cbm610 \
	-write $(JOYDRV)/c16/drv/joy/c16-stdjoy.joy c16-stdjoy.joy \
	-write $(JOYDRV)/c64/drv/joy/c64-stdjoy.joy c64-stdjoy.joy \
	-write $(JOYDRV)/c64/drv/joy/c64-ptvjoy.joy c64-ptvjoy.joy \
	-write $(JOYDRV)/c64/drv/joy/c64-hitjoy.joy c64-hitjoy.joy \
	-write $(JOYDRV)/c64/drv/joy/c64-numpad.joy c64-numpad.joy \
	-write $(JOYDRV)/c128/drv/joy/c128-stdjoy.joy c128-stdjoy.joy \
	-write $(JOYDRV)/c128/drv/joy/c128-ptvjoy.joy c128-ptvjoy.joy \
	-write $(JOYDRV)/cbm510/drv/joy/cbm510-std.joy cbm510-std.joy \
	-write $(JOYDRV)/pet/drv/joy/pet-stdjoy.joy pet-stdjoy.joy \
	-write $(JOYDRV)/plus4/drv/joy/plus4-stdjoy.joy plus4-stdjoy.joy \
	-write $(JOYDRV)/vic20/drv/joy/vic20-stdjoy.joy vic20-stdjoy.joy \
	-write $(JOYDRV)/vic20/drv/joy/vic20-ptvjoy.joy vic20-ptvjoy.joy \
	> $(DEVNULL)

portris_geos.cvt: $(SOURCEFILES) portrisres.grc
	@echo "geos ..."
	$(CL65) $(CL65FLAGS) -t geos -o portris_geos.cvt portrisres.grc main.c

geos: portris_geos.cvt

geosdisk: geos
	@echo "geosdisk ..."
	$(CP) geos.d64.bak geos.d64
#	$(C1541) -attach geos.d64 -write portris_geos.cvt portris.cvt > $(DEVNULL)
	$(C1541) -attach geos.d64 -geoswrite portris_geos.cvt

#
#	apple machines
#

apple2: $(SOURCEFILES)
	@echo "apple2 ..."
	$(CL65) $(CL65FLAGS) -o portris_apple2.xex -t apple2 -C apple2-system.cfg main.c
	$(CP) ProDOS_2_4_2.dsk portris_apple2.dsk
	java -jar $(ACMD) -p portris_apple2.dsk portris.system sys < portris_apple2.xex

apple2e: $(SOURCEFILES)
	@echo "apple2e ..."
	$(CL65) $(CL65FLAGS) -o portris_apple2e.xex -t apple2enh -C apple2-system.cfg main.c
	$(CP) ProDOS_2_4_2.dsk portris_apple2e.dsk
	java -jar $(ACMD) -p portris_apple2e.dsk portris.system sys < portris_apple2e.xex
#
#	atari machines
#

atari: $(SOURCEFILES)
	@echo "atari800 ..."
	$(CL65) $(CL65FLAGS) -o portris_atari.xex -t atari main.c
#
#	other machines or operating systems
#

.PHONY: nes osa65 atmos pcengine gamate

nes: $(SOURCEFILES)
	@echo "nes ..."
	$(CL65) $(CC65FLAGS) -o portris_nes.nes -t nes main.c

pcengine: $(SOURCEFILES)
	@echo "pcengine ..."
	$(CL65) $(CC65FLAGS) -Wl -D__CARTSIZE__=0x8000 -o portris_pcengine.bin -t pce main.c
# the first 8k block must be the last 8k block in the cartridge image
	$(DD) if=portris_pcengine.bin bs=8K skip=3 > portris_pcengine.pce
	$(DD) if=portris_pcengine.bin bs=8K count=3 >> portris_pcengine.pce

atmos: $(SOURCEFILES)
	@echo "atmos ..."
	$(CL65) $(CL65FLAGS) -o portlib_atmos.tap -t atmos main.c
#	-oric.header atmos.prg portlib_atmos.tap --quiet-autostart

# FIXME: file too larger
gamate: $(SOURCEFILES)
	@echo "gamate ..."
	$(CL65) -Osir -t gamate -o portris_gamate.bin main.c
	$(GFIX) portris_gamate.bin

# dont compile (yet?!) due to missing library-support
osa65: $(SOURCEFILES)
	@echo "osa65 ..."
	$(CL65) $(CCFLAGS) -o portris_osa65.prg -t none -C osa65.x -D__OSA65__ main.c

lunix: $(SOURCEFILES)
	@echo "lunix ..."
	$(CL65) $(CCFLAGS) -o portris_lunix.prg -t lunix main.c

#########################################################################################
#   cleanup etc
#########################################################################################

.PHONY: clean nice

clean:
	$(RM) portris.exe
	$(RM) portris
	$(RM) portris*.idb
	$(RM) portris*.prg
	$(RM) portris*.gb
	$(RM) portris*.gba
	$(RM) loader.prg
	$(RM) portris*.cvt
	$(RM) portris*.xex
	$(RM) portris*.bin
	$(RM) portris*.nes
	$(RM) portris*.pce
	$(RM) portris*.sav
	$(RM) portris*.mem
	$(RM) portris*.rst
	$(RM) portris*.ihx
	$(RM) ang-c++.ihx
	$(RM) debug.log
	$(RM) portris*.lnk
	$(RM) portris*.elf
	$(RM) portris*.gxb
	$(RM) gbz80.lib
	$(RM) *.TAP
	$(RM) *.tap
	$(RM) *.s
	$(RM) *.asm
	$(RM) *.rel
	$(RM) *.sym
	$(RM) *.joy
	$(RM) *.lnk
	$(RM) *.pre
	$(RM) *.cfg
	$(RM) *.def
	$(RM) pass1
	$(RM) pass2
	$(RM) pass3
	$(RM) pass4
	$(RM) pass5
	$(RM) *.o
	$(RM) *.lst
	$(RM) *.map
	$(RM) *.d64
	$(RM) portris*.dsk

nice:
	$(RM) portris*.prg
	$(RM) loader.prg
	$(RM) portris*.bin
	$(RM) portris*.srm
	$(RM) portris*.sav
	$(RM) portris*.elf
	$(RM) *.s
	$(RM) *.asm
	$(RM) *.rel
	$(RM) *.sym
	$(RM) *.joy
	$(RM) *.lnk
	$(RM) *.pre
	$(RM) *.cfg
	$(RM) *.def
	$(RM) pass1
	$(RM) pass2
	$(RM) pass3
	$(RM) pass4
	$(RM) pass5
	$(RM) *.o
	$(RM) *.lst
	$(RM) *.map

#########################################################################################
#   some stuff for lazy peeps like me :o)
#########################################################################################

runc64: c64
	$(C1541) -format portris,00 d64 portris.d64 -write portris_c64.prg portris-c64 \
	-write $(JOYDRV)/c64/drv/joy/c64-stdjoy.joy c64-stdjoy.joy \
	-write $(JOYDRV)/c64/drv/joy/c64-ptvjoy.joy c64-ptvjoy.joy \
	-write $(JOYDRV)/c64/drv/joy/c64-hitjoy.joy c64-hitjoy.joy \
	-write $(JOYDRV)/c64/drv/joy/c64-numpad.joy c64-numpad.joy \
	 > $(DEVNULL)
	x64sc --autostart portris.d64 > $(DEVNULL)
#	x64sc --autostart portris_c64.prg > $(DEVNULL)
#	x64sc -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)

runc6480: c6480
#	x64sc --autostart portris_c64_80x25.prg #> $(DEVNULL)
	x64sc -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)
rungeos: geos geosdisk
	x64sc --autostart geos.d64 > $(DEVNULL)

runc128: c128
#	x128 --autostart portris_c128.prg > $(DEVNULL)
	x128 -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)
runc128vdc: c128vdc
#	x128 -8 portris.d64 --autostart portris_c128_vdc.prg > $(DEVNULL)
	x128 -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)

runpet: pet
#	xpet --autostart portris_pet.prg #> $(DEVNULL)
#	xpet -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)
	xpet +truedrive -virtualdev --autostart portris.d64 > $(DEVNULL)

runvic20: vic
#	xvic --autostart portris_vic20.prg > $(DEVNULL)
	$(C1541) -format portris,00 d64 portris.d64 -write portris_vic20.prg portris-vic20 > $(DEVNULL)
	xvic -memory all -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)
runvichacked: vichacked
#	xvic --autostart portris_vic20_26x25.prg > $(DEVNULL)
	xvic -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)
runvic40: vic40
#	xvic --autostart portris_vic20_40x24.prg > $(DEVNULL)
	xvic -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)

runc16: c16
#	xplus4 --autostart portris_c16.prg > $(DEVNULL)
	xplus4 -model c232 -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)
runplus4: plus4
#	xplus4 --autostart portris_plus4.prg > $(DEVNULL)
	xplus4 -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)
runcbm510: cbm510
#	xcbm2 -model 510 --autostart portris_cbm510.prg > $(DEVNULL)
	xcbm5x0 +truedrive -virtualdev -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)
runcbm610: cbm610
#	xcbm2 -model 610 --autostart portris_cbm610.prg > $(DEVNULL)
	xcbm2 +truedrive -virtualdev -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)

runatari: atari
	atari800 -windowed -run portris_atari.xex > $(DEVNULL)
	xset r on

runnes: nes
#	InfoNES portris_nes.nes
#	fceu -pal portris_nes.nes
#	fceux portris_nes.nes
	mednafen portris_nes.nes

runatmos: atmos
#	xeuphoric -z 2; xset r on;rm printer.out
	CWD=`pwd`; cd ~/Desktop/oricutron-master/ && ./oricutron -m atmos -t $$CWD/portlib_atmos.tap

runpce: pcengine
#	xvpce portris_pcengine.pce
#	xyame -f 0 portris_pcengine.pce; xset r on
	mednafen -force_module pce portris_pcengine.pce

# FIXME
runapple2: apple2
	linapple --d1 portris_apple2.dsk --autoboot

# FIXME
runapple2e: apple2e
	linapple --d1 portris_apple2e.dsk --autoboot
