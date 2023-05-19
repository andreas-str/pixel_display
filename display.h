#ifndef DISPLAY_H_
#define DISPLAY_H_

struct frame_t
{
    int R;
    int G;
    int B;
};

enum Frame_size {
    size_frame_full = 120,
    size_frame_3x5 = 15
};

void display_init();
void display_frame(struct frame_t* frame, int frame_size, int start_x, int start_y, int glob_brightness);
void clear_display();
void clear_buffer(struct frame_t * frame, int frame_size);
void change_frame_color(int R, int G, int B, struct frame_t* frame, int frame_size);
long map(long x, long in_min, long in_max, long out_min, long out_max);
void convert_number_to_3x5_frame(int number, struct frame_t *frame);

#endif /* DISPLAY_H_ */
