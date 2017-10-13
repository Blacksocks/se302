#include "lcd.h"
#include "ch.h"
#include "hal.h"

#define LCD_RESET GPIOB_I2C1_SCL  // PB8
#define LCD_DC    GPIOB_I2C1_SDA  // PB9
#define LCD_MOSI  GPIOC_SPI2_MOSI // PC3
#define LCD_SCK   GPIOB_SPI2_SCK  // PB10
#define LCD_CS    GPIOG_SPI2_CS   // PG10

#define LCD_RESET_LOW()     palClearPad(GPIOB, LCD_RESET)
#define LCD_RESET_HIGH()    palSetPad  (GPIOB, LCD_RESET)
#define LCD_DC_LOW()        palClearPad(GPIOG, LCD_CS)
#define LCD_DC_HIGH()       palSetPad  (GPIOG, LCD_CS)

static const SPIConfig spicfg = {
    NULL, // Disable callbacks
    GPIOC,
    LCD_MOSI,
    0,
    0
};

static void set_lcd_pins(void)
{
    palSetPadMode(GPIOB, LCD_RESET, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
    palSetPadMode(GPIOB, LCD_DC, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
    palSetPadMode(GPIOC, LCD_MOSI, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);
    palSetPadMode(GPIOB, LCD_SCK, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);
    palSetPadMode(GPIOG, LCD_CS, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
}

static void send_command(int command)
{
    LCD_DC_LOW();
    spiSend(&SPID1, 1, (const void *)&command);
}

static void send_data(int command)
{
    LCD_DC_HIGH();
    spiSend(&SPID1, 1, (const void *)&command);
}

void init_lcd(void)
{
    set_lcd_pins();
    spiAcquireBus(&SPID1);
    spiStart(&SPID1, &spicfg);
    spiSelect(&SPID1);

    // Init procedure
    LCD_RESET_LOW();
    chThdSleepMilliseconds(15);
    LCD_RESET_HIGH();
    chThdSleepMilliseconds(15);
    send_command(0x21);
    send_command(0xF8);
    send_command(0x05);
    send_command(0x12);
    send_command(0x20);
    send_command(0x08);
    send_command(0x0C);

    // Set cursor to [0,0]
    send_command(0x80);
    send_command(0x40);

    // Send data
    send_data(0xAA);
    send_data(0xAA);
    send_data(0xAA);
    send_data(0xAA);

    spiUnselect(&SPID1);
    spiReleaseBus(&SPID1);
}
