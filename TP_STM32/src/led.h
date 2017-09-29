#ifndef _LED_H_
#define _LED_H_

/* init LED
** set LED on GPIO C as output using pushpull mode
** pushpull means that output is either connect to VCC or to GND, no floating state
*/
#define led_init() palSetPadMode(GPIOC, GPIOC_LED, PAL_MODE_OUTPUT_PUSHPULL)

/* switch on LED
*/
#define led_on() palClearPad(GPIOC, GPIOC_LED)

/* switch off LED
*/
#define led_off() palSetPad(GPIOC, GPIOC_LED)

/* toggle LED
*/
#define led_toggle() palTogglePad(GPIOC, GPIOC_LED)

#endif // _LED_H_
