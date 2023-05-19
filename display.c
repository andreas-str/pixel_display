#include "display.h"
#include "apa102_led.h"


void display_init()
{
    ;
}

void display_frame(struct frame_t* frame, int frame_size, int start_x, int start_y, int glob_brightness)
{
    int row = start_x;
    int colomn = start_y;
    int max_col = 0;
    int offset = (start_x * 10) + start_y;
    switch(frame_size)
    {
        case size_frame_full:
            max_col = 9;
            break;
        case size_frame_3x5:
            max_col = colomn + 2;
            break;
    }
    for(int i = 0; i < frame_size; i++)
    {
        if(colomn > max_col)
        {
            row++;
            colomn = start_y;
            offset +=  start_y + (9-max_col); 
        }
        if(row % 2 == 0)
        {
            DigiLed_setColor(i+offset, frame[i].R, frame[i].G, frame[i].B);
        }
        else
        {
            //start from the other side for the led itterator at each odd row
            int temp = i+offset - (10 * row);
            int i_rev = (9 + (10 * row)) - temp;
            DigiLed_setColor(i_rev, frame[i].R, frame[i].G, frame[i].B);
        }
        colomn++;
    }
    DigiLed_setAllIllumination(glob_brightness);
    DigiLed_update(TRUE);
}

void clear_display()
{
    DigiLed_setAllColor(0, 0, 0);
}

void clear_buffer(struct frame_t * frame, int frame_size)
{
    for(int i = 0; i < frame_size; i++)
    {
        frame[i].R = 0;
        frame[i].G = 0;
        frame[i].B = 0;
    }
}

void change_frame_color(int R, int G, int B, struct frame_t* frame, int frame_size)
{
    for(int i = 0; i < frame_size; i++)
    {
        if(frame[i].R > 0 || frame[i].G > 0|| frame[i].B > 0)
        {
            frame[i].R = R;
            frame[i].G = G;
            frame[i].B = B;
        }
    }
}
long map(long x, long in_min, long in_max, long out_min, long out_max) 
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void convert_number_to_3x5_frame(int number, struct frame_t* frame)
{
    
    if(number >=0 && number <=9)
    {
        switch(number)
        {
            case 0:
                frame[0].R = frame[0].G = frame[0].B = 255;
                frame[1].R = frame[1].G = frame[1].B = 255;
                frame[2].R = frame[2].G = frame[2].B = 255;
                frame[3].R = frame[3].G = frame[3].B = 255;
                frame[5].R = frame[5].G = frame[5].B = 255;
                frame[6].R = frame[6].G = frame[6].B = 255;
                frame[8].R = frame[8].G = frame[8].B = 255;
                frame[9].R = frame[9].G = frame[9].B = 255;
                frame[11].R = frame[11].G = frame[11].B = 255;
                frame[12].R = frame[12].G = frame[12].B = 255;
                frame[13].R = frame[13].G = frame[13].B = 255;
                frame[14].R = frame[14].G = frame[14].B = 255;
                break;
            case 1:
                frame[1].R = frame[1].G = frame[1].B = 255;
                frame[4].R = frame[4].G = frame[4].B = 255;
                frame[7].R = frame[7].G = frame[7].B = 255;
                frame[10].R = frame[10].G = frame[10].B = 255;
                frame[13].R = frame[13].G = frame[13].B = 255;
                break;
            case 2:
                frame[0].R = frame[0].G = frame[0].B = 255;
                frame[1].R = frame[1].G = frame[1].B = 255;
                frame[2].R = frame[2].G = frame[2].B = 255;
                frame[5].R = frame[5].G = frame[5].B = 255;
                frame[6].R = frame[6].G = frame[6].B = 255;
                frame[7].R = frame[7].G = frame[7].B = 255;
                frame[8].R = frame[8].G = frame[8].B = 255;
                frame[9].R = frame[9].G = frame[9].B = 255;
                frame[12].R = frame[12].G = frame[12].B = 255;
                frame[13].R = frame[13].G = frame[13].B = 255;
                frame[14].R = frame[14].G = frame[14].B = 255;
                break;
            case 3:
                frame[0].R = frame[0].G = frame[0].B = 255;
                frame[1].R = frame[1].G = frame[1].B = 255;
                frame[2].R = frame[2].G = frame[2].B = 255;
                frame[5].R = frame[5].G = frame[5].B = 255;
                frame[6].R = frame[6].G = frame[6].B = 255;
                frame[7].R = frame[7].G = frame[7].B = 255;
                frame[8].R = frame[8].G = frame[8].B = 255;
                frame[11].R = frame[11].G = frame[11].B = 255;
                frame[12].R = frame[12].G = frame[12].B = 255;
                frame[13].R = frame[13].G = frame[13].B = 255;
                frame[14].R = frame[14].G = frame[14].B = 255;
                break;
            case 4:
                frame[0].R = frame[0].G = frame[0].B = 255;
                frame[2].R = frame[2].G = frame[2].B = 255;
                frame[3].R = frame[3].G = frame[3].B = 255;
                frame[5].R = frame[5].G = frame[5].B = 255;
                frame[6].R = frame[6].G = frame[6].B = 255;
                frame[7].R = frame[7].G = frame[7].B = 255;
                frame[8].R = frame[8].G = frame[8].B = 255;
                frame[11].R = frame[11].G = frame[11].B = 255;
                frame[14].R = frame[14].G = frame[14].B = 255;
                break;
            case 5:
                frame[0].R = frame[0].G = frame[0].B = 255;
                frame[1].R = frame[1].G = frame[1].B = 255;
                frame[2].R = frame[2].G = frame[2].B = 255;
                frame[3].R = frame[3].G = frame[3].B = 255;
                frame[6].R = frame[6].G = frame[6].B = 255;
                frame[7].R = frame[7].G = frame[7].B = 255;
                frame[8].R = frame[8].G = frame[8].B = 255;
                frame[11].R = frame[11].G = frame[11].B = 255;
                frame[12].R = frame[12].G = frame[12].B = 255;
                frame[13].R = frame[13].G = frame[13].B = 255;
                frame[14].R = frame[14].G = frame[14].B = 255;
                break;
            case 6:
                frame[0].R = frame[0].G = frame[0].B = 255;
                frame[1].R = frame[1].G = frame[1].B = 255;
                frame[2].R = frame[2].G = frame[2].B = 255;
                frame[3].R = frame[3].G = frame[3].B = 255;
                frame[6].R = frame[6].G = frame[6].B = 255;
                frame[7].R = frame[7].G = frame[7].B = 255;
                frame[8].R = frame[8].G = frame[8].B = 255;
                frame[9].R = frame[9].G = frame[9].B = 255;
                frame[11].R = frame[11].G = frame[11].B = 255;
                frame[12].R = frame[12].G = frame[12].B = 255;
                frame[13].R = frame[13].G = frame[13].B = 255;
                frame[14].R = frame[14].G = frame[14].B = 255;
                break;
            case 7:
                frame[0].R = frame[0].G = frame[0].B = 255;
                frame[1].R = frame[1].G = frame[1].B = 255;
                frame[2].R = frame[2].G = frame[2].B = 255;
                frame[5].R = frame[5].G = frame[5].B = 255;
                frame[8].R = frame[8].G = frame[8].B = 255;
                frame[11].R = frame[11].G = frame[11].B = 255;
                frame[14].R = frame[14].G = frame[14].B = 255;
                break;
            case 8:
                frame[0].R = frame[0].G = frame[0].B = 255;
                frame[1].R = frame[1].G = frame[1].B = 255;
                frame[2].R = frame[2].G = frame[2].B = 255;
                frame[3].R = frame[3].G = frame[3].B = 255;
                frame[5].R = frame[5].G = frame[5].B = 255;
                frame[6].R = frame[6].G = frame[6].B = 255;
                frame[7].R = frame[7].G = frame[7].B = 255;
                frame[8].R = frame[8].G = frame[8].B = 255;
                frame[9].R = frame[9].G = frame[9].B = 255;
                frame[11].R = frame[11].G = frame[11].B = 255;
                frame[12].R = frame[12].G = frame[12].B = 255;
                frame[13].R = frame[13].G = frame[13].B = 255;
                frame[14].R = frame[14].G = frame[14].B = 255;
                break;
            case 9:
                frame[0].R = frame[0].G = frame[0].B = 255;
                frame[1].R = frame[1].G = frame[1].B = 255;
                frame[2].R = frame[2].G = frame[2].B = 255;
                frame[3].R = frame[3].G = frame[3].B = 255;
                frame[5].R = frame[5].G = frame[5].B = 255;
                frame[6].R = frame[6].G = frame[6].B = 255;
                frame[7].R = frame[7].G = frame[7].B = 255;
                frame[8].R = frame[8].G = frame[8].B = 255;
                frame[11].R = frame[11].G = frame[11].B = 255;
                frame[12].R = frame[12].G = frame[12].B = 255;
                frame[13].R = frame[13].G = frame[13].B = 255;
                frame[14].R = frame[14].G = frame[14].B = 255;
                break;
        }
    }
}