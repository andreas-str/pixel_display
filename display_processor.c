#include "display_processor.h"
#include "network_time.h"
#include "tcp_server.h"

struct frame_t frame_3x5[15] = {0};
struct frame_t full_frame[120] = {0};
int current_brightness = 7;
int curr_timezone = UTP_ZONE_AMS;


int main_display_loop(int current_state)
{
    TCP_SERVER_T *state = tcp_server_init();
    int cur_state = current_state;
    int cur_status = 0;
    int start_tcp_polling = 0;
    new_tcp_message = 0;
    while(true)
    {
        //experimental brightness control
        uint16_t adc_reading = 0;
        for(int j = 0; j < 10; j++)
        {
             adc_reading = adc_reading + adc_read();
        }
        int lumin = adc_reading / 10;
        //printf("adc: %d \n", lumin);
        int suggested_brightness = map(lumin, 0, 4099, 0, 32);

        //status indicator
        if(cur_state == state_connecting_to_wifi || cur_state == state_tests)
        {
            display_status(cur_state, cur_status);
        }
        if(cur_state == state_syncing || cur_state == state_in_sync)
        {
            display_status(cur_state, cur_status);
        }
        //tcp polling starter
        if(start_tcp_polling == 1)
        {
            cyw43_arch_poll();
        }
        //check new messages
        if(new_tcp_message > 0 && new_tcp_message < 10)
        {
            clear_buffer(full_frame, size_frame_full);
            display_frame(full_frame, size_frame_full, 0, 0, current_brightness);
            if (memcmp(tcp_rec_buffer, "time", 5) == 0) 
            {
                cur_state = state_time;
            }
            else if (memcmp(tcp_rec_buffer, "photo", 6) == 0) 
            {
                cur_state = state_photo;
            }
            else if (memcmp(tcp_rec_buffer, "video", 6) == 0) 
            {
                cur_state = state_video;
            }
            else if (memcmp(tcp_rec_buffer, "brightness", 11) == 0) 
            {
                current_brightness = current_brightness +5;
                if(current_brightness > 31)
                {
                    current_brightness = 1;
                }
            }
            else
            {
                ;
            }
            for(int i = 0; i < 512; i++)
            {
                tcp_rec_buffer[i] = 0;
            }
            new_tcp_message = 0;
        }

        switch(cur_state){
            case state_time:
                rtc_get_datetime(&current_time);
                display_time(current_time, time_colorator(), current_brightness);
                break;
            case state_photo:
                if(new_tcp_message == 360)
                {
                    clear_buffer(full_frame, size_frame_full);
                    int full_index = 0;
                    for(int i = 0; i < 120; i++)
                    {
                        full_frame[i].R = tcp_rec_buffer[full_index];
                        full_index++;
                        full_frame[i].G = tcp_rec_buffer[full_index];
                        full_index++;
                        full_frame[i].B = tcp_rec_buffer[full_index];
                        full_index++;
                    }
                    display_frame(full_frame, size_frame_full, 0, 0, current_brightness);
                    new_tcp_message = 0;
                }
                break;
            case state_video:
                break;
            case state_connecting_to_wifi:
                cur_status = connect_to_wifi();
                //if wifi connection fails, try again by coming back to this state again
                if(cur_status == 1 || cur_status == -1)
                {
                    cur_state = state_connecting_to_wifi;
                }
                else
                {
                    cur_state = state_syncing;
                }
                break;
            case state_syncing:
                //sync time with ntp
                run_ntp_getter(&current_time, curr_timezone);
                rtc_set_datetime(&current_time);
                //got time, start tcp server
                if (!tcp_server_open(state)) {
                    cur_state = state_syncing;
                }
                start_tcp_polling = 1;
                //go to in sync state
                cur_state = state_in_sync;
                break;
            case state_in_sync:
                //do nothing, stay here for now.
                cur_state = state_in_sync;
                break;
            case state_tests:
                ////////testss///////
                    display_status(state_syncing, cur_status);
                break;
            case state_unknown:
                break;
            default:
                break;
        };
    }
    free(state);
    return 0;
}

struct frame_t time_colorator()
{
    struct frame_t color;
    rtc_get_datetime(&current_time);
    if(current_time.hour >=0 && current_time.hour < 8)
    {
        color.R = 50;
        color.G = 0;
        color.B = 20;
        current_brightness = 1;
    }
    if(current_time.hour >=8 && current_time.hour < 12)
    {
        color.R = 190;
        color.G = 170;
        color.B = 20;
        current_brightness = 1 + ((current_time.hour - 8)*2);
    }
    if(current_time.hour >=12 && current_time.hour < 17)
    {
        color.R = 40;
        color.G = 50;
        color.B = 170;
        current_brightness = 9 - (current_time.hour - 12);
    }
    if(current_time.hour >=17 && current_time.hour < 24)
    {
        color.R = 95;
        color.G = 0;
        color.B = 105;
        if(current_time.hour < 22)
        {
            current_brightness = 3;
        }
        else
        {
            current_brightness = 2;
        }
    }
    return color;
}

void display_status(int cur_state, int cur_status)
{
    switch(cur_state){
        case state_connecting_to_wifi:
            full_frame[0].R = 255;
            full_frame[0].G = 0;
            full_frame[0].B = 0;
            display_frame(full_frame, size_frame_full, 0, 0, 5);
            break;
        case state_syncing:
            full_frame[0].R = 255;
            full_frame[0].G = 0;
            full_frame[0].B = 255;
            display_frame(full_frame, size_frame_full, 0, 0, 5);
            break;
        case state_in_sync:
            full_frame[0].R = 0;
            full_frame[0].G = 255;
            full_frame[0].B = 0;
            display_frame(full_frame, size_frame_full, 0, 0, 5);
            break;
        default:
            break;
    };
}


int connect_to_wifi()
{
    if (cyw43_arch_init()) {
        printf("WiFi init failed");
        return -1;
    }
    cyw43_arch_enable_sta_mode();

    if (cyw43_arch_wifi_connect_timeout_ms("aaaaaaaaaa", "665808eFc6", CYW43_AUTH_WPA2_AES_PSK, 100000)) {
        printf("failed to connect\n");
        return 1;
    }
    printf("connected\n");
    return 0;
}

void display_time(datetime_t current_time, struct frame_t frame_color, int brightness)
{
    if(brightness < 0)
    {
        brightness = 0;
    }
    if(brightness > 10)
    {
        brightness = 10;
    }

    if(current_time.hour >= 13)
    {
        current_time.hour = current_time.hour - 12;
    }
    if(current_time.hour == 0)
    {
        current_time.hour = 12;
    }

    int current_hour = current_time.hour;
    int hour_digits[2] = {0};
    int index = 0;
    while (current_hour > 0) 
    {
        hour_digits[index] = current_hour % 10;
        index++;
        current_hour /= 10;
    }
    index = 0;
    int current_minute = current_time.min;
    int minute_digits[2] = {0};
    while (current_minute > 0) 
    {
        minute_digits[index] = current_minute % 10;
        index++;
        current_minute /= 10;
    }
    index = 0;

    if(current_time.hour >= 10)
    {
        clear_buffer(frame_3x5, size_frame_3x5);
        convert_number_to_3x5_frame(hour_digits[1], frame_3x5);
        change_frame_color(frame_color.R, frame_color.G, frame_color.B, frame_3x5, size_frame_3x5);
        display_frame(frame_3x5, size_frame_3x5, 0, 1, brightness);

        clear_buffer(frame_3x5, size_frame_3x5);
        convert_number_to_3x5_frame(hour_digits[0], frame_3x5);
        change_frame_color(frame_color.R, frame_color.G, frame_color.B, frame_3x5, size_frame_3x5);
        display_frame(frame_3x5, size_frame_3x5, 0, 6, brightness);
    }
    else
    {
        clear_buffer(frame_3x5, size_frame_3x5);
        convert_number_to_3x5_frame(0, frame_3x5);
        change_frame_color(frame_color.R, frame_color.G, frame_color.B, frame_3x5, size_frame_3x5);
        display_frame(frame_3x5, size_frame_3x5, 0, 1, brightness);
        clear_buffer(frame_3x5, size_frame_3x5);
        convert_number_to_3x5_frame((int)current_time.hour, frame_3x5);
        change_frame_color(frame_color.R, frame_color.G, frame_color.B, frame_3x5, size_frame_3x5);
        display_frame(frame_3x5, size_frame_3x5, 0, 6, brightness);
    }
    if(current_time.min >= 10)
    {
        clear_buffer(frame_3x5, size_frame_3x5);
        convert_number_to_3x5_frame(minute_digits[1], frame_3x5);
        change_frame_color(frame_color.R, frame_color.G, frame_color.B, frame_3x5, size_frame_3x5);
        display_frame(frame_3x5, size_frame_3x5, 7, 1, brightness);

        clear_buffer(frame_3x5, size_frame_3x5);
        convert_number_to_3x5_frame(minute_digits[0], frame_3x5);
        change_frame_color(frame_color.R, frame_color.G, frame_color.B, frame_3x5, size_frame_3x5);
        display_frame(frame_3x5, size_frame_3x5, 7, 6, brightness);
    }
    else
    {
        clear_buffer(frame_3x5, size_frame_3x5);
        convert_number_to_3x5_frame(0, frame_3x5);
        change_frame_color(frame_color.R, frame_color.G, frame_color.B, frame_3x5, size_frame_3x5);
        display_frame(frame_3x5, size_frame_3x5, 7, 1, brightness);
        clear_buffer(frame_3x5, size_frame_3x5);
        convert_number_to_3x5_frame((int)current_time.min, frame_3x5);
        change_frame_color(frame_color.R, frame_color.G, frame_color.B, frame_3x5, size_frame_3x5);
        display_frame(frame_3x5, size_frame_3x5, 7, 6, brightness);
    }
}