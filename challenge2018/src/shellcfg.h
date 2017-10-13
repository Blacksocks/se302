#include "chprintf.h"
#include "rtt.h"

void cmd_hello(BaseSequentialStream *stream, int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    chprintf(stream, "world\r\n");
}

void cmd_led_on(BaseSequentialStream *stream, int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    (void)stream;
    led_on();
}

void cmd_led_off(BaseSequentialStream *stream, int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    (void)stream;
    led_off();
}

void cmd_led_toggle(BaseSequentialStream *stream, int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    (void)stream;
    led_toggle();
}

void cmd_led_pwm(BaseSequentialStream *stream, int argc, char *argv[])
{
    if(argc != 1){
        chprintf(stream, "Usage: led_pwm <intensity>\r\nintensity: integer between 0 and 100\r\n");
        return;
    }
    if(led_pwm(atoi(argv[0])))
        chprintf(stream, "An error occured using led_pwm function\r\n");
}

void cmd_led_blink(BaseSequentialStream *stream, int argc, char *argv[])
{
    if(argc != 1){
        chprintf(stream, "Usage: led_blink <period>\r\nperiod: blinking period in ms between 10 and 10000\r\n");
        return;
    }
    if(led_blink(atoi(argv[0])))
        chprintf(stream, "An error occured using led_blink function\r\n");
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
    // RTT1  to use RTT
    // SDU1  to use Serial over USB
    RTT1,
    // Commands structure
    commands
};

thread_t *shell = NULL;
