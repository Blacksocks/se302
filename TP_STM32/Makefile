##############################################################################
# Project, sources and paths
#

# Define project name here
PROJECT = se302vg

# Imported source files and paths
CHIBIOS = ../ChibiOS
# Startup files.
include $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_stm32f4xx.mk
# HAL-OSAL files (optional).
include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS)/os/hal/ports/STM32/STM32F4xx/platform.mk
include board_E407/board.mk
include $(CHIBIOS)/os/hal/osal/rt/osal.mk
# RTOS files (optional).
include $(CHIBIOS)/os/rt/rt.mk
include $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/mk/port_v7m.mk

# Define linker script file here
LDSCRIPT= $(STARTUPLD)/STM32F407xG.ld

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CSRC = $(STARTUPSRC) \
       $(KERNSRC) \
       $(PORTSRC) \
       $(OSALSRC) \
       $(HALSRC) \
       $(PLATFORMSRC) \
       $(BOARDSRC) \
       $(TESTSRC) \
       $(LWSRC) \
       $(FATFSSRC) \
       $(STREAMSSRC) \
       $(SHELLSRC) \
       $(CHIBIOS)/os/various/evtimer.c \
       src/main.c

# List ASM source files here
ASMXSRC = $(STARTUPASM) $(PORTASM) $(OSALASM)

INCDIR = $(CHIBIOS)/os/license \
         $(STARTUPINC) $(KERNINC) $(PORTINC) $(OSALINC) \
         $(HALINC) $(PLATFORMINC) $(BOARDINC) $(TESTINC) \
         $(STREAMSINC) $(SHELLINC) $(LWINC) $(FATFSINC) \
         $(CHIBIOS)/os/various

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

# Compiler options here.
ifeq ($(USE_OPT),)
	USE_OPT = -O2 -g -std=gnu99
endif

MCU  = cortex-m4
TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
LD   = $(TRGT)gcc
DB   = $(TRGT)gdb
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
AR   = $(TRGT)ar
OD   = $(TRGT)objdump
SZ   = $(TRGT)size
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

# THUMB-specific options here
TOPT = -mthumb -DTHUMB

# Define C warning options here
CWARN = -Werror -Wall -Wextra -Wundef -Wstrict-prototypes

#
# Compiler settings
##############################################################################

RULESPATH = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC
include $(RULESPATH)/rules.mk

startgdbserver:
	@pidof JLinkGDBServer > /dev/null || JLinkGDBServer -if swd -speed auto -device STM32F405ZG

stopgdbserver:
	@pidof JLinkGDBServer > /dev/null && killall JLinkGDBServer || true

debug: $(ELF) startgdbserver
	$(DB) $<

.PHONY: startgdbserver stopgdbserver debug
