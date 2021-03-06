################################################################################
# These are variables for the GBA toolchain build
# You can add others if you wish to
# ***** Eugene Chu *****
################################################################################

# The name of your desired GBA game
# This should be a just a name i.e MyFirstGBAGame
# No SPACES AFTER THE NAME.
PROGNAME = GOKU_JUMP

# The object files you want to compile into your program
# This should be a space (SPACE!) separated list of .o files
OFILES = main.o mylib.o text.o font.o ./images/goku_stand.o ./images/goku_standL.o ./images/goku_dead.o ./images/goku_main.o \
./images/goku_run1.o ./images/goku_run1L.o ./images/goku_run2.o ./images/goku_run2L.o \
./images/goku_run3.o ./images/goku_run3L.o images/goku_jump.o ./images/goku_jumpL.o \
./images/goku_dash1.o ./images/goku_dash1L.o ./images/goku_dash2.o ./images/goku_dash2L.o \
./images/goku_dash3.o ./images/goku_dash3L.o ./images/platform_up.o ./images/platform_left.o ./images/platform_right.o \
./images/goku_down.o ./images/goku_downL.o ./images/goal.o

# The header files you have created.
# This is necessary to determine when to recompile for files.
# This should be a space (SPACE!) separated list of .h files
HFILES = mylib.h text.h goku_main.h goku_dead.h goku_stand.h goku_standL.h \
goku_run1.h goku_run1L.h goku_run2.h goku_run2L.h goku_run3.h goku_run3L.h \
goku_jump.h goku_jumpL.h goku_dash1.h goku_dash1L.h goku_dash2.h goku_dash2L.h goku_dash3.h goku_dash3L.h \
goku_down.h goku_downL.h platform_up.h platform_left.h platform_right.h goal.h

################################################################################
# These are various settings used to make the GBA toolchain work
# DO NOT EDIT BELOW.
################################################################################

TOOLDIR  = /usr/local/cs2110-tools
ARMLIB   = $(TOOLDIR)/arm-thumb-eabi/lib
CFLAGS   = -Wall -Werror -std=c99 -pedantic -Wextra
CFLAGS   += -mthumb-interwork -mlong-calls -nostartfiles -MMD -MP -I $(TOOLDIR)/include
LDFLAGS = -L $(TOOLDIR)/lib \
		  -L $(TOOLDIR)/lib/gcc/arm-thumb-eabi/4.4.1/thumb \
		  -L $(ARMLIB) \
		  --script $(ARMLIB)/arm-gba.ld
CDEBUG   = -g -DDEBUG
CRELEASE = -O2 
CC       = $(TOOLDIR)/bin/arm-thumb-eabi-gcc
AS       = $(TOOLDIR)/bin/arm-thumb-eabi-as
LD       = $(TOOLDIR)/bin/arm-thumb-eabi-ld
OBJCOPY  = $(TOOLDIR)/bin/arm-thumb-eabi-objcopy
GDB      = $(TOOLDIR)/bin/arm-thumb-eabi-gdb
CFILES   = $(OFILES:.o=.c)

################################################################################
# These are the targets for the GBA build system
################################################################################

all : CFLAGS += $(CRELEASE)
all : $(PROGNAME).gba
	@echo "[FINISH] Created $(PROGNAME).gba"

.PHONY : all clean

$(PROGNAME).gba : $(PROGNAME).elf
	@echo "[LINK] Linking objects together to create $(PROGNAME).gba"
	@$(OBJCOPY) -O binary $(PROGNAME).elf $(PROGNAME).gba

$(PROGNAME).elf : crt0.o $(OFILES)
	@$(LD) $(LDFLAGS) -o $(PROGNAME).elf $^ -lgcc -lc -lgcc $(LDDEBUG)
	@rm -f *.d

crt0.o : $(ARMLIB)/crt0.s
	@$(AS) -mthumb-interwork $^ -o crt0.o

%.o : %.c
	@echo "[COMPILE] Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o : %.s                                                                        
	@echo "[ASSEMBLE] Assembling $<"                                               
	@$(AS) $< -o $@ -mthumb -mthumb-interwork

clean :
	@echo "[CLEAN] Removing all compiled files"
	@rm -f *.o *.elf *.gba *.d

vba : CFLAGS += $(CRELEASE)
vba : $(PROGNAME).gba
	@echo "[EXECUTE] Running Emulator VBA-M"
	@vbam $(VBAOPT) $(PROGNAME).gba > /dev/null 2> /dev/null

med : CFLAGS += $(CRELEASE)
med : $(PROGNAME).gba
	@echo "[EXECUTE] Running emulator Mednafen"
	@mednafen $(PROGNAME).gba > /dev/null 2>&1

-include $(CFILES:%.c=%.d)

