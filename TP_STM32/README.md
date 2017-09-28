# TP STM32

## Description

The goal of this project is to install and use ChibiOS on a STM32-E407 board.
We have to use basic I/O as LED, USB and finally Ethernet.

## 1st step: Compiling basic program using ChibiOS

### Download ChibiOS

ChibiOS meaning: ちび means small [link](http://chibios.sourceforge.net/docs3/rt/index.html)

I downloaded ChibiOS from this [link](https://sourceforge.net/projects/chibios/files/?SetFreedomCookie)  
and created my project next to ChibiOS folder.

### Project architecture

To don't begin from scratch, I used demo project:
```
<ChibiOS>/demos/<proc_familly>/<board>/
```
I copied this files in my project
> chconf.h  
> halconf.h  
> mcuconf.h  
> Makefile

I created a basic main.c program into a src/
```c
int main(void) {
    while(1);
}
```

Then, I changed pin configuration by copying this folder into my project:
```
<ChibiOS>/os/hal/boards/<board>/
```
and modifying board.h and board.mk  
board.mk:
```
BOARDSRC = board_E407/board.c
BOARDINC = board_E407
```

### Compiling using ChibiOS

To link my own files to compilation, I changed Makefile:
```
CHIBIOS = ../ChibiOS        # set chibios folder
...
CSRC = ... \
       src/main.c           # add own main program
...
include board_E407/board.mk # link my board configuration
```
Doing ```make``` created an executable file <project>.elf

## 2st step: Runing basic program using ChibiOS

### J-Link

To run program on board, I used J-Link debugger.  
I used this command to run JLinkGDBServer:
```
pidof JLinkGDBServer > /dev/null || JLinkGDBServer -if swd -speed auto -device STM32F405ZG
```
And these ones in another terminal:
```
arm-none-eabi-gdb build/<project>.elf
split # use user friendly interface
load  # load program in flash
c     # run program
q     # quit
```

## 3rd step: Use ChibiOS init functions

Add init functions into main.c:
```c
#include "ch.h"
#include "hal.h"

int main(void)
{
    halInit();
    chSysInit();
    while(1);
}
```

If while loop is not reached during execution on board, there is probably an error.  
I had an error using Olimex-STM32-E407 ([soluce](http://www.chibios.com/forum/viewtopic.php?t=1040))  
Into GDB, display position into c code using ```layout n``` and then ```print <variable>``` to show error.  
I personally had to set ```HAL_USE_MAC``` to ```false``` into ```halconf.h```.
