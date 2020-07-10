
.SILENT:

CC65HOME=/home/groepaz/Desktop/cc65/github-current/cc65/bin/
CL65=$(CC65HOME)cl65
#include $(PORTLIB)/makefile.config

DEVNULL=/dev/null

CP=cp
XARGS=xargs

#PORTLIB=~/groepaz/libs/cc65-portlib
#PLIB=$(PORTLIB)/lib
#PARCH=$(PORTLIB)/arch
#PINC=$(PORTLIB)/include

PUCRUNCH=~/bin/pucrunch
EXOMIZER=~/bin/exomizer

CC65FLAGS+=-Osir

JOYDRV := $(shell $(CL65) --print-target-path)

##########################################################################################

.PHONY: all clean nice targets cc65 sdcc arm

CBMTARGETS=c64 c6480 c6480m c128 c128vdc pet plus4 cbm510 cbm610 vic c16 geos
#vichacked vic40

CC65TARGETS=apple2 atari nes pcengine atmos
CC65TARGETS+=$(CBMTARGETS) cbmloader cbmdisk geosdisk

ARMTARGETS=gp32 gba
SDCCTARGETS=gameboy

help:
	@echo some targets are:
	@echo all clean nice cbm cc65 $(CC65TARGETS) sdcc $(SDCCTARGETS) arm $(ARMTARGETS)
	@echo look in the makefile for more.

all: clean targets nice

cc65targets: $(CC65TARGETS)
armcctargets: $(ARMTARGETS)
sdcctargets: $(SDCCTARGETS)

arm: armcctargets
cc65: cc65targets
sdcc: sdcctargets

targets: cc65 sdcc arm

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

.PHONY: cbmloader c64 c128 c128vdc plus4 c16 vic20 cbm510 cbm610 pet atari apple2 geos geosdic cbmdisk

cbmloader: loader.bas
	@echo "cbm loader..."
	@petcat -w2 -l 0401 loader.bas > loader.prg 2> $(DEVNULL)

cbm: cbmloader $(CBMTARGETS)
	
c64: $(SOURCEFILES)
	@echo "c64..."
	$(CL65) $(CC65FLAGS) -t c64 -o portris_c64.bin main.c
	@$(PUCRUNCH) -c64 -x2061 portris_c64.bin portris_c64.prg 1> $(DEVNULL) 2> $(DEVNULL)
	@#$(EXOMIZER) -q -s 2061 -o portris_c64.prg portris_c64.bin

c6480: $(SOURCEFILES)
	@echo "c64 + soft80..."
	$(CL65) $(CC65FLAGS) -o portris_c64_80x25.bin -D__SOFT80__ main.c c64-soft80.o
	@$(PUCRUNCH) -c64 -x2061 portris_c64_80x25.bin portris_c64_80x25.prg 1> $(DEVNULL) 2> $(DEVNULL)
	@#$(EXOMIZER) -q -s 2061 -o portris_c64_80x25.prg portris_c64_80x25.bin

c6480m: $(SOURCEFILES)
	@echo "c64 + soft80..."
	$(CL65) $(CC65FLAGS) -o portris_c64_80x25m.bin -D__SOFT80__ main.c c64-soft80mono.o
	@$(PUCRUNCH) -c64 -x2061 portris_c64_80x25m.bin portris_c64_80x25m.prg 1> $(DEVNULL) 2> $(DEVNULL)
	@#$(EXOMIZER) -q -s 2061 -o portris_c64_80x25m.prg portris_c64_80x25m.bin

c128: $(SOURCEFILES)
	@echo "c128 ..."
	$(CL65) $(CC65FLAGS) -t c128 -o portris_c128.bin main.c
	@$(PUCRUNCH) -c128 -x7181 portris_c128.bin portris_c128.prg 1> $(DEVNULL) 2> $(DEVNULL)

c128vdc: $(SOURCEFILES)
	@echo "c128 + vdc..."
	$(CL65) $(CC65FLAGS) -o portris_c128_vdc.bin -t c128 -D__VDC__ main.c
	@$(PUCRUNCH) -c128 -x7181 portris_c128_vdc.bin portris_c128_vdc.prg 1> $(DEVNULL) 2> $(DEVNULL)

plus4: $(SOURCEFILES)
	@echo "plus4 ..."
	$(CL65) $(CC65FLAGS) -o portris_plus4.bin -t plus4 main.c
	@$(PUCRUNCH) -c16 -x4109 portris_plus4.bin portris_plus4.prg 1> $(DEVNULL) 2> $(DEVNULL)

c16: $(SOURCEFILES)
	@echo "c16 ..."
	$(CL65) $(CC65FLAGS) -o portris_c16.bin -t c16 -C c16-32k.cfg main.c
	@$(PUCRUNCH) -c16 -x4109 portris_c16.bin portris_c16.prg 1> $(DEVNULL) 2> $(DEVNULL)

# only works for expanded machines (NOT with default config for vic!)
vic: $(SOURCEFILES)
	@echo "vic20 (expanded) ..."
	$(CL65) $(CC65FLAGS) -o portris_vic20.bin -t vic20 -C vic20-32k.cfg main.c
	@$(PUCRUNCH) -c20 -x4621 portris_vic20.bin portris_vic20.prg 1> $(DEVNULL) 2> $(DEVNULL)
vichacked: $(SOURCEFILES)
		@echo "vic20 (expanded) + hard26..."
		$(CL65) $(CC65FLAGS) -o portris_vic20_26x25.bin -t vic20 \
		-C $(PARCH)/vic20/vic20exp.x -D__VIC20HACKED__ main.c \
			$(PLIB)/vic20hard26port.lib

		@$(PUCRUNCH) -c20 -x4621 portris_vic20_26x25.bin portris_vic20_26x25.prg 1> $(DEVNULL) 2> $(DEVNULL)

vic40: $(SOURCEFILES)
		@echo "vic20 (expanded) + soft40..."
		$(CL65) $(CC65FLAGS) -t vic20 -o portris_vic20_40x24.bin -D__SOFT40__ -C vic20-32k.cfg main.c vic20-soft40.o
		
#		$(CL65) $(CC65FLAGS) -o portris_vic20_40x24.bin -t vic20 \
#		-C $(PARCH)/vic20/soft40/vic20exp.x -D__SOFT40__ main.c \
#			$(PLIB)/vic20soft40port.lib
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

	@c1541 -format portris,00 d64 portris.d64 \
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

#		-write portris_c64_80x25.prg portris-c64-80x25 \
#		-write portris_vic20_26x25.prg portris-vic20-26x25 \
#		-write portris_vic20_40x24.prg portris-vic20-40x24 \
	
		@# copy the joystick drivers

#		@find $(PLIB) -name "*.joy" -printf "%f\n" | \
#			xargs --replace \
#			c1541 -attach portris.d64 -write $(PLIB)/{} {},u \
#			> $(DEVNULL)

#	@find $(JOYDRV)/ -name "*.joy" -printf "%f\n" | \
#		xargs --replace \
#		c1541 -attach portris.d64 -write $(JOYDRV)/{} {},p \
#		> $(DEVNULL)

portris_geos.cvt: $(SOURCEFILES) portrisres.grc
		@echo "geos ..."
		$(CL65) $(CL65FLAGS) -t geos -o portris_geos.cvt portrisres.grc main.c
		
#		@cc65 $(CC65FLAGS) -o main.s -t geos main.c
#		@ca65 -o main.o -t geos main.s
#		@ld65 -o portris_geos.cvt -t geos $(PLIB)/geosheader.o geos.o main.o \
#			$(PLIB)/geosport.lib \
#			geos.lib

geos: portris_geos.cvt

geosdisk: geos
		@echo "geosdisk ..."
		$(CP) geos.d64.bak geos.d64
#		c1541 -attach geos.d64 -write portris_geos.cvt portris.cvt > $(DEVNULL)
		c1541 -attach geos.d64 -geoswrite portris_geos.cvt

#
#	apple machines
#

apple2: $(SOURCEFILES)
		@echo "apple2 ..."
		$(CL65) $(CL65FLAGS) -o portris_apple2.xex -t apple2 main.c

#
#	atari machines
#

atari: $(SOURCEFILES)
		@echo "atari800 ..."
		$(CL65) $(CL65FLAGS) -o portris_atari.xex -t atari main.c
#
#	other machines or operating systems
#

.PHONY: nes osa65 atmos pcengine

nes: $(SOURCEFILES)
	@echo "nes ..."
	$(CL65) $(CC65FLAGS) -o portris_nes.nes -t nes main.c

pcengine: $(SOURCEFILES)
	@echo "pcengine ..."
	$(CL65) $(CC65FLAGS) -Wl -D__CARTSIZE__=0x8000 -o portris_pcengine.bin -t pce main.c
# the first 8k block must be the last 8k block in the cartridge image
	dd if=portris_pcengine.bin bs=8K skip=3 > portris_pcengine.pce
	dd if=portris_pcengine.bin bs=8K count=3 >> portris_pcengine.pce

atmos: $(SOURCEFILES)
		@echo "atmos ..."

		$(CL65) $(CL65FLAGS) -o ATMOS.TAP -t atmos main.c

		@#-oric.header atmos.prg ATMOS.TAP --quiet-autostart

# dont compile (yet?!) due to missing library-support
osa65: $(SOURCEFILES)
		@echo "osa65 ..."
		$(CL65) $(CCFLAGS) -o portris_osa65.prg -t none -C osa65.x -D__OSA65__ main.c

lunix: $(SOURCEFILES)
		@echo "lunix ..."
		$(CL65) $(CCFLAGS) -o portris_lunix.prg -t lunix main.c

#########################################################################################
# Z80/SDCC
##########################################################################################

.PHONY: gameboy gb_sdcc

GBCCFLAGS+=-I $(PINC) -I $(PARCH) -I .

gameboy: gb_sdcc

gb_sdcc: $(SOURCEFILES)
		@echo "gb(c) / sdcc ..."

		@$(GBCC) $(GBCCFLAGS) -S main.c -o main.asm
		@$(GBAS) $(GBASFLAGS) -o main.o main.asm

		@$(GBLD) -- -z portris_gb.gb \
			$(PLIB)/gbstart.o \
 		   /usr/local/share/sdcc/lib/gbz80/crt0_rle.o \
		   \
			main.o \
			\
			-k $(PARCH)/gb/ -l $(PARCH)/gb/gbport.lib \
			\
		   /usr/local/share/sdcc/lib/gbz80/_strcat.o \
  		   /usr/local/share/sdcc/lib/gbz80/_strcpy.o \
  		   /usr/local/share/sdcc/lib/gbz80/_strlen.o \
  		   /usr/local/share/sdcc/lib/gbz80/_memcpy.o \
  		   /usr/local/share/sdcc/lib/gbz80/_memset.o \
		   /usr/local/share/sdcc/lib/gbz80/_modslong.o \
		   /usr/local/share/sdcc/lib/gbz80/_divslong.o \
		   /usr/local/share/sdcc/lib/gbz80/_modulong.o \
		   /usr/local/share/sdcc/lib/gbz80/_divulong.o \
		   /usr/local/share/sdcc/lib/gbz80/_mullong.o \
		   /usr/local/share/sdcc/lib/gbz80/shift.o \
		   /usr/local/share/sdcc/lib/gbz80/mul.o \
		   /usr/local/share/sdcc/lib/gbz80/div.o \
		   /usr/local/share/sdcc/lib/gbz80/stubs.o \
			\
			$(PARCH)/gb/conio.o \
			$(PARCH)/gb/memmove.o \
			$(PARCH)/gb/vsprintf.o \
			$(PARCH)/gb/_oserror.o \
			$(PARCH)/gb/ltostr.o \
			$(PARCH)/gb/get_tv.o \
 			$(PARCH)/gb/joystick.o\
			$(PARCH)/gb/font.o \
		$(PARCH)/gb/init_tt.o\
		$(PARCH)/gb/set_spr.o\
		$(PARCH)/gb/mv_spr.o\
		$(PARCH)/gb/arand.o\
		$(PARCH)/gb/hiramcpy.o\
		$(PARCH)/gb/cpy_data.o \
		$(PARCH)/gb/delay.o \
		$(PARCH)/gb/drawing.o\
		$(PARCH)/gb/rand.o\
		$(PARCH)/gb/scroll_b.o\
		$(PARCH)/gb/scroll_s.o\
		$(PARCH)/gb/get_bk_t.o \
		$(PARCH)/gb/scroll_w.o\
		$(PARCH)/gb/get_data.o \
		$(PARCH)/gb/serial.o\
		$(PARCH)/gb/get_prop.o  \
		$(PARCH)/gb/set_bk_t.o\
		$(PARCH)/gb/set_data.o\
		$(PARCH)/gb/get_wi_t.o \
		$(PARCH)/gb/set_prop.o\
		$(PARCH)/gb/get_xy_t.o \
		$(PARCH)/gb/set_wi_t.o\
		$(PARCH)/gb/set_xy_t.o\
		$(PARCH)/gb/fontms.o

.PHONY: spectrum

Z80CFLAGS+=-I $(PINC) -I $(PARCH)

spectrum: $(SOURCEFILES)
	$(Z80CC) $(Z80CFLAGS) -c -o $(PARCH)/spectrum/conio.o $(PARCH)/spectrum/conio.c
	$(Z80CC) $(Z80CFLAGS) -c -o $(PARCH)/spectrum/memmove.o $(PARCH)/spectrum/memmove.c
	$(Z80CC) $(Z80CFLAGS) -c -o $(PARCH)/spectrum/fclose.o $(PARCH)/spectrum/fclose.c
	$(Z80CC) $(Z80CFLAGS) -c -o $(PARCH)/spectrum/open.o $(PARCH)/spectrum/open.c
	$(Z80CC) $(Z80CFLAGS) -c -o $(PARCH)/spectrum/ltostr.o $(PARCH)/spectrum/ltostr.c
	$(Z80CC) $(Z80CFLAGS) -c -o $(PARCH)/spectrum/vsprintf.o $(PARCH)/spectrum/vsprintf.c
	$(Z80CC) $(Z80CFLAGS) -c -o $(PARCH)/spectrum/close.o $(PARCH)/spectrum/close.c
	$(Z80CC) $(Z80CFLAGS) -c -o $(PARCH)/spectrum/printk.o $(PARCH)/spectrum/printk.c

	$(Z80AS) $(Z80ASFLAGS) -o $(PARCH)/spectrum/crt0.o $(PARCH)/spectrum/crt0.s

	$(Z80AS) $(Z80ASFLAGS) -o $(PARCH)/spectrum/puts_cons.o $(PARCH)/spectrum/puts_cons.s
	$(Z80AS) $(Z80ASFLAGS) -o $(PARCH)/spectrum/rand.o $(PARCH)/spectrum/rand.s
	$(Z80AS) $(Z80ASFLAGS) -o $(PARCH)/spectrum/getk.o $(PARCH)/spectrum/getk.s
	$(Z80AS) $(Z80ASFLAGS) -o $(PARCH)/spectrum/fputc_cons.o $(PARCH)/spectrum/fputc_cons.s


	    $(Z80CC) $(Z80CFLAGS) -S -o main.asm test.c
	$(Z80AS) $(Z80ASFLAGS) -o main.o main.asm

	    $(Z80CC) --code-loc 32768 $(Z80CFLAGS) test.c -o /temp/test.ihx

bla:

		-Wl-l"$(PARCH)/spectrum/crt0.o" \
		-Wl-l"main.o" -Wl-o,portris_spectrum.bin


		-Wl-l, "$(PARCH)/spectrum/conio.o" \
		-Wl-L, $(PARCH)/spectrum/memmove.o \
		-Wl-L, $(PARCH)/spectrum/fclose.o \
		-Wl-L, $(PARCH)/spectrum/open.o \
		-Wl-L, $(PARCH)/spectrum/ltostr.o \
		-Wl-L, $(PARCH)/spectrum/vsprintf.o \
		-Wl-L, $(PARCH)/spectrum/puts_cons.o \
		-Wl-L, $(PARCH)/spectrum/close.o \
		-Wl-L, $(PARCH)/spectrum/rand.o \
		-Wl-L, $(PARCH)/spectrum/getk.o \
		-Wl-L, $(PARCH)/spectrum/printk.o \
		-Wl-L, $(PARCH)/spectrum/fputc_cons.o \
		  -Wl-L, /usr/local/share/sdcc/lib/z80/_strcat.o \
		   -Wl-L, /usr/local/share/sdcc/lib/z80/_strcpy.o \
		   -Wl-L, /usr/local/share/sdcc/lib/z80/_strlen.o \
		   -Wl-L, /usr/local/share/sdcc/lib/z80/_memcpy.o \
		   -Wl-L, /usr/local/share/sdcc/lib/z80/_memset.o \
		   -Wl-L, /usr/local/share/sdcc/lib/z80/_modslong.o \
		   -Wl-L, /usr/local/share/sdcc/lib/z80/_divslong.o \
		   -Wl-L, /usr/local/share/sdcc/lib/z80/_modulong.o \
		   -Wl-L, /usr/local/share/sdcc/lib/z80/_divulong.o \
		   -Wl-L, /usr/local/share/sdcc/lib/z80/_mullong.o \
		   -Wl-L, /usr/local/share/sdcc/lib/z80/shift.o \
		   -Wl-L, /usr/local/share/sdcc/lib/z80/mul.o \
		   -Wl-L, /usr/local/share/sdcc/lib/z80/div.o \
		   -Wl-L, /usr/local/share/sdcc/lib/z80/stubs.o \

	    makebin < portris_spectrum.ihx > portris_spectrum.bin
	bin2tap portris_spectrum.bin portris_spectrum.tap 32768

aspectrum: $(SOURCEFILES)
	 $(Z80CC) $(Z80CFLAGS) -S -o main.asm main.c
	$(Z80AS) $(Z80ASFLAGS) -o main.o main.asm

	$(Z80CC) $(Z80CFLAGS) -c -o $(PARCH)/spectrum/conio.o $(PARCH)/spectrum/conio.c
	$(Z80CC) $(Z80CFLAGS) -c -o $(PARCH)/spectrum/memmove.o $(PARCH)/spectrum/memmove.c
	$(Z80CC) $(Z80CFLAGS) -c -o $(PARCH)/spectrum/fclose.o $(PARCH)/spectrum/fclose.c
	$(Z80CC) $(Z80CFLAGS) -c -o $(PARCH)/spectrum/open.o $(PARCH)/spectrum/open.c
	$(Z80CC) $(Z80CFLAGS) -c -o $(PARCH)/spectrum/ltostr.o $(PARCH)/spectrum/ltostr.c
	$(Z80CC) $(Z80CFLAGS) -c -o $(PARCH)/spectrum/vsprintf.o $(PARCH)/spectrum/vsprintf.c
	$(Z80CC) $(Z80CFLAGS) -c -o $(PARCH)/spectrum/close.o $(PARCH)/spectrum/close.c
	$(Z80CC) $(Z80CFLAGS) -c -o $(PARCH)/spectrum/printk.o $(PARCH)/spectrum/printk.c

	$(Z80AS) $(Z80ASFLAGS) -o $(PARCH)/spectrum/crt0.o $(PARCH)/spectrum/crt0.s

	$(Z80AS) $(Z80ASFLAGS) -o $(PARCH)/spectrum/puts_cons.o $(PARCH)/spectrum/puts_cons.s
	$(Z80AS) $(Z80ASFLAGS) -o $(PARCH)/spectrum/rand.o $(PARCH)/spectrum/rand.s
	$(Z80AS) $(Z80ASFLAGS) -o $(PARCH)/spectrum/getk.o $(PARCH)/spectrum/getk.s
	$(Z80AS) $(Z80ASFLAGS) -o $(PARCH)/spectrum/fputc_cons.o $(PARCH)/spectrum/fputc_cons.s
#$(Z80CFLAGS)
#bla:
		    $(Z80LD) -- -m -i portris_spectrum.bin \
		$(PARCH)/spectrum/crt0.o \
		        main.o \
		$(PARCH)/spectrum/conio.o \
		$(PARCH)/spectrum/memmove.o \
		$(PARCH)/spectrum/fclose.o \
		$(PARCH)/spectrum/open.o \
		$(PARCH)/spectrum/ltostr.o \
		$(PARCH)/spectrum/vsprintf.o \
		$(PARCH)/spectrum/puts_cons.o \
		$(PARCH)/spectrum/close.o \
		$(PARCH)/spectrum/rand.o \
		$(PARCH)/spectrum/getk.o \
		$(PARCH)/spectrum/printk.o \
		$(PARCH)/spectrum/fputc_cons.o \
			\
		   /usr/local/share/sdcc/lib/z80/_strcat.o \
  		   /usr/local/share/sdcc/lib/z80/_strcpy.o \
  		   /usr/local/share/sdcc/lib/z80/_strlen.o \
  		   /usr/local/share/sdcc/lib/z80/_memcpy.o \
  		   /usr/local/share/sdcc/lib/z80/_memset.o \
		   /usr/local/share/sdcc/lib/z80/_modslong.o \
		   /usr/local/share/sdcc/lib/z80/_divslong.o \
		   /usr/local/share/sdcc/lib/z80/_modulong.o \
		   /usr/local/share/sdcc/lib/z80/_divulong.o \
		   /usr/local/share/sdcc/lib/z80/_mullong.o \
		   /usr/local/share/sdcc/lib/z80/shift.o \
		   /usr/local/share/sdcc/lib/z80/mul.o \
		   /usr/local/share/sdcc/lib/z80/div.o \
		   /usr/local/share/sdcc/lib/z80/stubs.o

	    makebin -p < portris_spectrum.ihx > portris_spectrum.bin
	bin2tap portris_spectrum.bin portris_spectrum.tap 32768

spectrum_z88dk: $(SOURCEFILES)
	$(Z88DKCC) $(Z88DKCFLAGS) -o portris_spectrum.bin main.c \
		$(PARCH)/spectrum/conio.c \
		$(PARCH)/spectrum/memmove.c \
		$(PARCH)/spectrum/fclose.c \
		$(PARCH)/spectrum/open.c \
		$(PARCH)/spectrum/ltostr.c \
		$(PARCH)/spectrum/vsprintf.c \
		$(PARCH)/spectrum/puts_cons.asm \
		$(PARCH)/spectrum/close.c \
		$(Z80CLIBS)

	@bin2tap portris_spectrum.bin portris_spectrum.tap 32768

#########################################################################################
# Arm/GCC
##########################################################################################

.PHONY: gba gp32

gba: $(SOURCEFILES)
		@echo "gba ..."
		# -W -Wall
		@$(ARMCC) -O6 -c -I $(PINC) -I $(PARCH) -I. -D__GBA__ \
			main.c

		@$(ARMLD) -nostdlib -o portris_gba.elf \
			-T $(PARCH)/gba/arm-gbabin.x \
			$(PLIB)/gbastart.o \
			main.o \
			/opt/devkitadv/lib/gcc-lib/arm-agb-elf/3.0.2/libgcc.a \
			-lc -lg -lm -lstdc++ \
			$(PLIB)/gbaport.a \
			/opt/devkitadv/lib/gcc-lib/arm-agb-elf/3.0.2/libgcc.a \
			-lc -lg -lm -lstdc++

		@$(ARMOBJCOPY) -v -O binary portris_gba.elf portris_gba.gba >$(DEVNULL)

gp32: $(SOURCEFILES)

		@echo "gp32 ..."
		# -W -Wall
		@$(ARMCC) -O6 -c -I $(PINC) -I $(PARCH) -D__GP32__ \
			main.c

		@$(ARMLD) -nostdlib -o portris_gp32.elf -T $(PARCH)/gp32/arm-gp32bin.x \
			$(PLIB)/gp32start.o \
			main.o \
			/opt/devkitadv/lib/gcc-lib/arm-agb-elf/3.0.2/libgcc.a \
			-lc -lg -lm -lstdc++ \
			$(PLIB)/gp32port.a \
			/opt/devkitadv/lib/gcc-lib/arm-agb-elf/3.0.2/libgcc.a \
			-lc -lg -lm -lstdc++

		@$(ARMOBJCOPY) -v -O binary portris_gp32.elf portris_gp32.gxb >$(DEVNULL)

#########################################################################################
#   gamecube
#########################################################################################

DEVKITCUBE=/usr/local/devkitcube
PORTLIBROOT=~/groepaz/libs/portlib_gc

PATH+=:/usr/local/devkitcube/bin
#PATH+=:~/gamecube/tools
#export PATH

CC=$(DEVKITCUBE)/bin/powerpc-eabi-elf-gcc
CCFLAGS=-nostdlib -Wall -W
CCFLAGS+=-I $(PORTLIBROOT)/include -D__GAMECUBE__

OBJCOPY=$(DEVKITCUBE)/bin/powerpc-eabi-elf-objcopy
STRIP=$(DEVKITCUBE)/bin/powerpc-eabi-elf-strip

LD=$(DEVKITCUBE)/bin/powerpc-eabi-elf-ld
LDFLAGS=-T $(PORTLIBROOT)/ldscripts/ppc-ngcbin.x

AS=$(DEVKITCUBE)/bin/powerpc-eabi-elf-as
AR=$(DEVKITCUBE)/bin/powerpc-eabi-elf-ar

STARTUP=$(PORTLIBROOT)/lib/startup/crt0.o

LIBS= \
        $(PORTLIBROOT)/lib/libngcport.a \
        -lc -lnosys

gamecube: $(SOURCEFILES)
	$(CC) -c $(CCFLAGS) main.c -o main.o
	$(LD) $(LDFLAGS) -o portris_gc.elf $(STARTUP) main.o $(LIBS)
	$(STRIP) --remove-section=".comment" --strip-all --strip-debug --strip-unneeded --discard-all --discard-locals portris_gc.elf
    #strip portris_gc.elf
	$(OBJCOPY) -O binary portris_gc.elf portris_gc.bin

#########################################################################################
#   pc
#########################################################################################

# to make sure the mingw32 compiler is used
#export PATH=/cygdrive/g/devcpp/bin:$PATH
BASE=j:/groepaz3
INCLUDE=$(BASE)/include
LIB=$(BASE)/lib

MCC=g:/devcpp/bin/gcc
MCCFLAGS+=-I $(INCLUDE)
MCCFLAGS+=-L $(LIB)

#MCCFLAGS+=-O2 -Wall -W -D__MINGW32__ -fshort-double -fvtable-thunks -w -fno-exceptions
MCCFLAGS+=-O2 -Wall -W -fshort-double
MCCLIBS+=-lgcc -mconsole -lmsvcrt -lmingw32port -lmingw32port-con
#-D__MINGW32__ // automatically

mingw32: main.c
	$(MCC) $(MCCFLAGS) main.c $(MCCLIBS) -o portris.exe

#BASE=~/groepaz4
BASE=~/groepaz/libs/portlib_pc
INCLUDE=$(BASE)/include
LIB=$(BASE)/lib

LINCC=gcc
LINCCFLAGS+=-I $(INCLUDE)
LINCCFLAGS+=-L $(LIB)

LINCCFLAGS+=-O2 -Wall -W -fshort-double
LINCCLIBS+=-lgcc -llinuxport -llinuxport-con -lncurses -D__LINUX__

linux: main.c
	$(LINCC) $(LINCCFLAGS) main.c $(LINCCLIBS) -o portris


BASE=/Users/groepaz/groepaz/libs/portlib_pc
INCLUDE=$(BASE)/include
LIB=$(BASE)/lib

OSXCC=gcc
OSXCCFLAGS+=-I$(INCLUDE)
OSXCCFLAGS+=-L$(LIB)

OSXCCFLAGS+=-O2 -Wall -W -fshort-double
OSXCCLIBS+=-lgcc -lmacosport -lmacosport-con -lncurses -D__MACOS__

macos: main.c
	$(OSXCC) $(OSXCCFLAGS) main.c $(OSXCCLIBS) -o portris


OSXSDLCCFLAGS+=-I$(INCLUDE) -I/opt/local/include
OSXSDLCCFLAGS+=-L$(LIB) -L/opt/local/lib
OSXSDLCCFLAGS+=-O2 -Wall -W -fshort-double
OSXSDLCCLIBS+= `sdl-config --static-libs`
OSXSDLCCLIBS+=-lmacosport -lmacosport-gcon -D__MACOS__ -D__SDL__

macos-sdl:
	$(OSXCC) $(OSXSDLCCFLAGS) main.c $(OSXSDLCCLIBS) -o portris

#########################################################################################
#   cleanup etc
#########################################################################################

.PHONY: clean nice

clean:
		@$(RM) portris.exe
		@$(RM) portris
		@$(RM) portris*.idb
		@$(RM) portris*.prg
		@$(RM) portris*.gb
		@$(RM) portris*.gba
		@$(RM) loader.prg
		@$(RM) portris*.cvt
		@$(RM) portris*.xex
		@$(RM) portris*.bin
		@$(RM) portris*.nes
		@$(RM) portris*.pce
		@$(RM) portris*.sav
		@$(RM) portris*.mem
		@$(RM) portris*.rst
		@$(RM) portris*.ihx
		@$(RM) ang-c++.ihx
		@$(RM) debug.log
		@$(RM) portris*.lnk
		@$(RM) portris*.elf
		@$(RM) portris*.gxb
		@$(RM) gbz80.lib
		@$(RM) *.TAP
		@$(RM) *.tap
		@$(RM) *.s
		@$(RM) *.asm
		@$(RM) *.rel
		@$(RM) *.sym
		@$(RM) *.joy
		@$(RM) *.lnk
		@$(RM) *.pre
		@$(RM) *.cfg
		@$(RM) *.def
		@$(RM) pass1
		@$(RM) pass2
		@$(RM) pass3
		@$(RM) pass4
		@$(RM) pass5
		@$(RM) *.o
		@$(RM) *.lst
		@$(RM) *.map
		@$(RM) *.d64

nice:
		@$(RM) portris*.prg
		@$(RM) loader.prg
		@$(RM) portris*.bin
		@$(RM) portris*.srm
		@$(RM) portris*.sav
		@$(RM) portris*.elf
		@$(RM) *.s
		@$(RM) *.asm
		@$(RM) *.rel
		@$(RM) *.sym
		@$(RM) *.joy
		@$(RM) *.lnk
		@$(RM) *.pre
		@$(RM) *.cfg
		@$(RM) *.def
		@$(RM) pass1
		@$(RM) pass2
		@$(RM) pass3
		@$(RM) pass4
		@$(RM) pass5
		@$(RM) *.o
		@$(RM) *.lst
		@$(RM) *.map

#########################################################################################
#   some stuff for lazy peeps like me :o)
#########################################################################################

run: c64
		@c1541 -format portris,00 d64 portris.d64 -write portris_c64.prg portris-c64 > $(DEVNULL)

# copy the joystick drivers
		@find $(JOYDRV)/c64/drv/joy -name "*.joy" -printf "%f\n" | \
			xargs --replace \
			c1541 -attach portris.d64 -write $(JOYDRV)/c64/drv/joy/{} {},u \
			> $(DEVNULL)
		@x64sc --autostart portris.d64 > $(DEVNULL)

runc64: c64
		@#x64sc --autostart portris_c64.prg > $(DEVNULL)
		@x64sc -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)
runc6480: c6480
		@#x64sc --autostart portris_c64_80x25.prg #> $(DEVNULL)
		@x64sc -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)
rungeos: geos geosdisk
		@x64sc --autostart geos.d64 > $(DEVNULL)

runc128: c128
		@#x128 --autostart portris_c128.prg > $(DEVNULL)
		@x128 -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)
runc128vdc: c128vdc
		@#x128 -8 portris.d64 --autostart portris_c128_vdc.prg > $(DEVNULL)
		@x128 -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)

runpet: pet
		@#xpet --autostart portris_pet.prg #> $(DEVNULL)
		@#xpet -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)
		@xpet +truedrive -virtualdev --autostart portris.d64 > $(DEVNULL)

runvic20: vic
#		@xvic --autostart portris_vic20.prg > $(DEVNULL)
		@c1541 -format portris,00 d64 portris.d64 -write portris_vic20.prg portris-vic20 > $(DEVNULL)
		@xvic -memory all -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)
runvichacked: vichacked
		@#xvic --autostart portris_vic20_26x25.prg > $(DEVNULL)
		@xvic -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)
runvic40: vic40
		@#xvic --autostart portris_vic20_40x24.prg > $(DEVNULL)
		@xvic -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)

runc16: c16
		@#xplus4 --autostart portris_c16.prg > $(DEVNULL)
		@xplus4 -model c232 -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)
runplus4: plus4
		@#xplus4 --autostart portris_plus4.prg > $(DEVNULL)
		@xplus4 -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)
runcbm510: cbm510
		@#xcbm2 -model 510 --autostart portris_cbm510.prg > $(DEVNULL)
		@xcbm5x0 +truedrive -virtualdev -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)
runcbm610: cbm610
		@#xcbm2 -model 610 --autostart portris_cbm610.prg > $(DEVNULL)
		@xcbm2 +truedrive -virtualdev -8 portris.d64 -keybuf \\x4c\\xcf\\x22\\x2a\\x22\\x2c\\x38\\x3a\\x83 > $(DEVNULL)

runatari: atari
		@atari800 -windowed -run portris_atari.xex > $(DEVNULL)
		@xset r on

runnes: nes
#		@InfoNES portris_nes.nes
#		@fceu -pal portris_nes.nes

#		fceux portris_nes.nes
		mednafen portris_nes.nes

runatmos: atmos
		@xeuphoric -z 2; xset r on;rm printer.out

runpce: pcengine
#		@xvpce portris_pcengine.pce
#		@xyame -f 0 portris_pcengine.pce; xset r on
		mednafen -force_module pce portris_pcengine.pce

runapple2: apple2
		@echo yeah i want that too! how to use the damned emu? \:\o\)

################################################################################
		
rungb: gameboy
		@xgnuboy --scale=2 portris_gb.gb

runspectrum: spectrum
#runspectrum: spectrum_z88dk
		@xspect -quick-load -load-immed portris_spectrum.tap

rungba: gba
		@VisualBoyAdvance -T 500000 -f 0 -1 -s 9 --ifb-none portris_gba.gba 2> $(DEVNULL)

rungp32: gp32
		@~/geepee32/geepee32 /EXE=portris_gp32.gxb /RUN > $(DEVNULL)
