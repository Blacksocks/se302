#ifndef _LED_H_
#define _LED_H_

/* Macros used to switch on/off, toggle and init LED
*/
#define LED_ON()      palClearPad(GPIOC, GPIOC_LED)
#define LED_OFF()     palSetPad(GPIOC, GPIOC_LED)
#define LED_TOGGLE()  palTogglePad(GPIOC, GPIOC_LED)
#define LED_SET_PIN() palSetPadMode(GPIOC, GPIOC_LED, PAL_MODE_OUTPUT_PUSHPULL)

/* init LED
** set LED on GPIO C as output using pushpull mode
** pushpull means that output is either connect to VCC or to GND, no floating state
*/
void led_init(void);

/* switch on LED
*/
void led_on(void);

/* switch off LED
*/
void led_off(void);

/* toggle LED
*/
void led_toggle(void);

/* Set led intensity in percent
** intensity: integer between 0 and 100
** return 0 if function terminate successfully,
**        1 else (parameter is wrong)
*/
int led_pwm(unsigned int intensity);

/* Blink LED
** period: blinking period in ms between 10 and 10000
** return 0 if function terminate successfully,
**        1 else (parameter is wrong)
*/
int led_blink(unsigned int m_period);

#endif // _LED_H_
