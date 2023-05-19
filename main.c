#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"
#include "apa102_led.h"
#include "display.h"
#include "display_processor.h"

int main() {
    //----------------------------------------------------------------//
    //------------------------Init variables--------------------------//
    //----------------------------------------------------------------//
    const uint sck_pin = 18;
    const uint mosi_pin = 19;
    spi_inst_t *spi = spi0;
    char datetime_buf[256];
    char *datetime_str = &datetime_buf[0];
    datetime_t default_time = {
        .year  = 2022,
        .month = 1,
        .day   = 1,
        .dotw  = 1, // 0 is Sunday, so 5 is Friday
        .hour  = 1,
        .min   = 1,
        .sec   = 1
    };

    //----------------------------------------------------------------//
    //--------------------Init general peripherals--------------------//
    //----------------------------------------------------------------//
    stdio_init_all();
    //Set smps mode
    gpio_init(23);
    gpio_set_dir(23, GPIO_OUT);
    gpio_put(23, 0);
    //Init ADC
    adc_init();
    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(26);
    // Select ADC input 2 (GPIO26)
    adc_select_input(2);
    //Init SPI
    //Initialize SPI port at 10 MHz
    spi_init(spi, 7000 * 1000);
    // Set SPI format
    spi_set_format( spi0,   // SPI instance
                    8,      // Number of bits per transfer
                    1,      // Polarity (CPOL)
                    1,      // Phase (CPHA)
                    SPI_MSB_FIRST);
    // Initialize SPI pins
    gpio_set_function(sck_pin, GPIO_FUNC_SPI);
    gpio_set_function(mosi_pin, GPIO_FUNC_SPI);
    //Init RTC
    rtc_init();
    //set time to 0
    rtc_set_datetime(&default_time);
    //wait for cpu clock
    sleep_us(64);
    //Init LEDs
    DigiLed_init(spi);

    while (true) {

        main_display_loop(state_connecting_to_wifi);

    }
}
