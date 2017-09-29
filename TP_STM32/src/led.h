#ifndef _LED_H_
#define _LED_H_

/* init LED
** set LED on GPIO C as output using pushpull mode
** pushpull means that output is either connect to VCC or to GND, no floating state
*/
void init_led(void);

/* switch on LED
*/
void led_on(void);

/* switch off LED
*/
void led_off(void);

/* if led is off, switch on LED
** if led if on, switch off LED
*/
void led_toogle(void);

#endif // _LED_H_
