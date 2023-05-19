#ifndef DISPLAY_PROCESSOR_H_
#define DISPLAY_PROCESSOR_H_
#include "display.h"
#include "hardware/rtc.h"
#include "hardware/adc.h"
#include <time.h>
#include "pico/util/datetime.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

datetime_t current_time;
struct frame_t frame_color;
enum STATES {
    state_time,
    state_photo,
    state_video,
    state_connecting_to_wifi,
    state_connecting,
    state_syncing,
    state_in_sync,
    state_tests,
    state_unknown
};
enum STATUS {
    status_ok,
    status_wifi_fail,
    status_sync_fail
};
enum TIMEZONES {
    UTP_ZONE_AMS = 2,
    UTP_ZONE_GR = 3
};

int main_display_loop(int current_state);
void display_status(int state, int status);
int connect_to_wifi();
void tcp_server_start();
void display_time(datetime_t current_time, struct frame_t frame_color, int brightness);
struct frame_t time_colorator();


#endif /* DISPLAY_PROCESSOR_H_ */
