#ifndef COLOR_SENSOR__H
#define COLOR_SENSOR__H

#define NUMBER_OF_COLORS 4

// order of values reflects sensor color table
typedef enum color
{
    red,
    blue,
    clear,
    green
} Color_t;

int color_values[NUMBER_OF_COLORS];

void color_sensor_init(void);

#endif
