# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

set(PICO_BOARD pico_w)

# Include build functions from Pico SDK
include(pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C   standards
project(blink C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
    main.c
    apa102_led.c
    animations.c
    display.c
    display_processor.c
    network_time.c
    tcp_server.c
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(
    ${PROJECT_NAME} 
    pico_stdlib
    pico_bootsel_via_double_reset
    hardware_spi
    hardware_adc
    hardware_rtc
    pico_cyw43_arch_lwip_poll
)

# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)