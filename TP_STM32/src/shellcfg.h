#include "chprintf.h"
#include "rtt.h"

void cmd_hello(BaseSequentialStream *sdu, int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    chprintf(sdu, "world\r\n");
}

void cmd_led_on(BaseSequentialStream *sdu, int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    (void)sdu;
    led_on();
}

void cmd_led_off(BaseSequentialStream *sdu, int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    (void)sdu;
    led_off();
}

void cmd_led_toggle(BaseSequentialStream *sdu, int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    (void)sdu;
    led_toggle();
}

void cmd_led_pwm(BaseSequentialStream *sdu, int argc, char *argv[])
{
    if(argc != 1){
        chprintf(sdu, "Usage: led_pwm <intensity>\r\nintensity: integer between 0 and 100\r\n");
        return;
    }
    if(led_pwm(atoi(argv[0])))
        chprintf(sdu, "An error occured using led_pwm function\r\n");
}

void cmd_led_blink(BaseSequentialStream *sdu, int argc, char *argv[])
{
    if(argc != 1){
        chprintf(sdu, "Usage: led_blink <period>\r\nperiod: blinking period in ms between 10 and 10000\r\n");
        return;
    }
    if(led_blink(atoi(argv[0])))
        chprintf(sdu, "An error occured using led_blink function\r\n");
}

const ShellCommand commands[] = {
    {"hello", cmd_hello},
    {"led_on", cmd_led_on},
    {"led_off", cmd_led_off},
    {"led_toggle", cmd_led_toggle},
    {"led_pwm", cmd_led_pwm},
    {"led_blink", cmd_led_blink},
    {NULL, NULL}
};

ShellConfig shell_cfg1 = {
    // Stream where data are send/received
    // RTT_stream to use RTT
    // SDU1       to use Serial over USB
    (BaseSequentialStream *) &RTT_stream,
    // Commands structure
    commands
};

thread_t *shell = NULL;
