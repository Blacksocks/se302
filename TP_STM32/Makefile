MCU = cortex-m4
TRGT = arm-none-eabi-
CC = $(TRGT)gcc
GDB = $(TRGT)gdb
OBJDUMP = $(TRGT)objdump
AS = $(TRGT)as

TRGT_ARCH    = -mcpu=$(MCU) -mthumb
CFLAGS       = -c -std=gnu99 -g -O1 -Werror -Wall -Wextra
LDFLAGS      = -nostdlib
ASFLAGS      = -g -mcpu=$(MCU)

SRCS = main.c
OBJS = $(subst .c,.o,$(SRCS))
DEPS = $(subst .c,.d,$(SRCS))
EXE = main

.PHONY : db gdbstart gdbstop sec as head seccont clean

# Create ouput (exe)
$(EXE) : $(OBJS)

# ====== OBJDUMP ======

# Dump output sections informations
sec: $(EXE)
    $(OBJDUMP) -h $(EXE)

# Display output assembly code
as: $(EXE)
    $(OBJDUMP) -d $(EXE)

# Display ouput header
head: $(EXE)
    $(OBJDUMP) -x $(EXE)

# Display ouput sections content
seccont: $(EXE)
    $(OBJDUMP) -s $(EXE)

# ======= CLEAN =======

# Remove temporary useful files
clean:
    rm -f $(EXE) $(OBJS) $(DEPS)

# ======== GDB ========

# Start JLink Server
gdbstart:
    @pidof JLinkGDBServer > /dev/null || JLinkGDBServer -if swd -speed auto -device STM32F405ZG

# Stop JLink Server
gdbstop:
    @pidof JLinkGDBServer > /dev/null && killall JLinkGDBServer || true

# Debug
db: $(ELF) gdbstart
    $(GDB) $<

# === DEPENDENCIES ====

-include $(DEPS)

%.d : %.c
    $(CC) $(CFLAGS) $(TRGT_ARCH) -MM -MF $@ -MP $<
