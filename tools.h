#ifndef __TOOLS_H
#define __TOOLS_H

void blue_led_debug();

void blue_led_blink(int duration);

int is_magnet_dectected();

void quarter(int duration);

uint16_t min(uint16_t a, uint16_t b);

int max(int a, int b);

int abs(int a);

float mean(int *list, int len);

uint16_t power(int x, int n);

void displayTime(uint16_t *leds, uint32_t resolution, uint16_t seconds)

#endif