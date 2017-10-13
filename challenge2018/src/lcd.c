#include "lcd.h"
#include "ch.h"
#include "hal.h"

#define LCD_RESET GPIOB_I2C1_SCL  // PB8
#define LCD_DC    GPIOB_I2C1_SDA  // PB9
#define LCD_MOSI  GPIOC_SPI2_MOSI // PC3
#define LCD_SCK   GPIOB_SPI2_SCK  // PB10
#define LCD_CS    GPIOG_SPI2_CS   // PG10

#define LCD_RESET_LOW()     palClearPad(GPIOB, LCD_RESET)
#define LCD_RESET_HIGH()    palSetPad(GPIOB, LCD_RESET)
#define LCD_CS_LOW()        palClearPad(GPIOG, LCD_CS)

static void set_lcd_pins(void)
{
    palSetPadMode(GPIOB, LCD_RESET, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, LCD_DC, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC, LCD_MOSI, PAL_MODE_ALTERNATE(5));
    palSetPadMode(GPIOB, LCD_SCK, PAL_MODE_ALTERNATE(5));
    palSetPadMode(GPIOG, LCD_CS, PAL_MODE_OUTPUT_PUSHPULL);
}

void init_lcd(void)
{
    set_lcd_pins();
    LCD_CS_LOW();

    // Init procedure
    LCD_RESET_LOW();
    chThdSleepMilliseconds(15);
    LCD_RESET_HIGH();
    chThdSleepMilliseconds(15);

}
