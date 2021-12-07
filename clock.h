#ifndef __CLOCK_H
#define __CLOCK_H

struct clock{
    int hour;
    int min;
    int sec;
    int hour_offset;
    int min_offset;
    int sec_offset;
};

void needle_clock(int h, int m, int s);

#endif