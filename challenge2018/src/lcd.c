#include "lcd.h"

#define LCD_RESET I2C1_SCL  // PB8
#define LCD_DC    I2C1_SDA  // PB9
#define LCD_MOSI  SPI2_MOSI // PC3
#define LCD_SCK   SPI2_SCK  // PB10
#define LDC_CS    SPI2_CS   // PG10

static void set_lcd_pins(void)
{

}

void init_lcd(void)
{
    set_lcd_pins();
}
