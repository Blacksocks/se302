#include <stdlib.h>
#include "ch.h"
#include "hal.h"
#include "led.h"
#include "usbcfg.h"
#include "shell.h"
#include "chprintf.h"

static void cmd_hello(BaseSequentialStream *sdu, int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    chprintf(sdu, "world\r\n");
}

static void cmd_led_on(BaseSequentialStream *sdu, int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    (void)sdu;
    led_on();
}

static void cmd_led_off(BaseSequentialStream *sdu, int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    (void)sdu;
    led_off();
}

static void cmd_led_toggle(BaseSequentialStream *sdu, int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    (void)sdu;
    led_toggle();
}

static void cmd_led_pwm(BaseSequentialStream *sdu, int argc, char *argv[])
{
    if(argc != 1){
        chprintf(sdu, "Usage: led_pwm <intensity>\r\nintensity: integer between 0 and 100\r\n");
        return;
    }
    if(led_pwm(atoi(argv[0])))
        chprintf(sdu, "An error occured using led_pwm function\r\n");
}

static void cmd_led_blink(BaseSequentialStream *sdu, int argc, char *argv[])
{
    if(argc != 1){
        chprintf(sdu, "Usage: led_blink <period>\r\nperiod: blinking period in ms between 10 and 10000\r\n");
        return;
    }
    if(led_blink(atoi(argv[0])))
        chprintf(sdu, "An error occured using led_blink function\r\n");
}

static const ShellCommand commands[] = {
    {"hello", cmd_hello},
    {"led_on", cmd_led_on},
    {"led_off", cmd_led_off},
    {"led_toggle", cmd_led_toggle},
    {"led_pwm", cmd_led_pwm},
    {"led_blink", cmd_led_blink},
    {NULL, NULL}
};

static ShellConfig shell_cfg1 = {
    (BaseSequentialStream *) &SDU1,
    commands
};

int main(void)
{
    /*
     * System initializations.
     * - HAL initialization, this also init512ializes the configured device drivers
     *   and performs the board-specific initializations.
     * - Kernel initialization, the main() function becomes a thread and the RTOS is active.
     */
    halInit();
    chSysInit();

    // Configure GPIO ports for USB
    // PF11 and PB1 must be configured as input floating but it's by default
    palSetPadMode(GPIOA, GPIOA_USB_HS_BUSON, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, GPIOA_OTG_FS_VBUS, PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOA, GPIOA_OTG_FS_ID, PAL_MODE_ALTERNATE(10));
    palSetPadMode(GPIOA, GPIOA_OTG_FS_DM, PAL_MODE_ALTERNATE(10));
    palSetPadMode(GPIOA, GPIOA_OTG_FS_DP, PAL_MODE_ALTERNATE(10));
    palSetPadMode(GPIOB, GPIOB_USB_FS_BUSON, PAL_MODE_OUTPUT_PUSHPULL);
    // Initialize a serial over USB driver
    sduObjectInit(&SDU1);
    sduStart(&SDU1, &serusbcfg);
    // Start serial connection
    usbDisconnectBus(serusbcfg.usbp);
    chThdSleepMilliseconds(1500);
    usbStart(serusbcfg.usbp, &usbcfg);
    usbConnectBus(serusbcfg.usbp);

    // Configure Shell
    shellInit();

    // Configure LED
    led_init();

    int input;

    while(1){
        if(SDU1.config->usbp->state == USB_ACTIVE)
        {
            // This timeout is needed to find when USB init is finished
            input = chnGetTimeout(&SDU1, MS2ST(4000));
            if (input == Q_TIMEOUT)
                // Run shell
                shellThread(&shell_cfg1);
        }
        chThdSleepMilliseconds(100);
    }
}
