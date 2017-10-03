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
