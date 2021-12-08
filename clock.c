#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "clock.h"
#include "tools.h"
#include "spi.h"

//struct clock cl;

void init_clock(struct clock *cl, int h, int m, int s) {
    cl->hour_offset = h;
    cl->min_offset = m;
    cl->sec_offset = s;
}

void global_counter_to_clock(int global_counter, struct clock *cl) {
    int nb_sec = (int) (global_counter / 200);
    int nb_min = (int) (nb_sec / 60);
    int nb_hour = (int) (nb_min / 60);
    cl->hour = nb_hour % 12;
    cl->min = nb_min - 60 * nb_hour;
    cl->sec = nb_sec - 60 * nb_min;
}

//void displayMin() {
//    SPI_MasterInit();
//    while (1) {
//        if (is_magnet_dectected()) {
//            for (int i = 0; i < 120; i++) {
//                if (i == 2 * nb_min) {
//                    SPI_MasterTransmit(0b1111111111111111);
//                } else {
//                    if (i <= 2 * nb_sec) {
//                        SPI_MasterTransmit(0b1000000000000000);
//                    }
//                }
//                _delay_ms(0.395);
//                SPI_MasterTransmit(0x00);
//            }
//        }
//    }
//    while (1) {}
//}
//
//void displaySec() {
//    SPI_MasterInit();
//    while (1) {
//        if (is_magnet_dectected()) {
//
//            for (int i = 0; i < 2 * nb_sec; i++) {
//                SPI_MasterTransmit(0b1000000000000000);
//                _delay_ms(0.38);
//                SPI_MasterTransmit(0x00);
//            }
//        }
//    }
//    while (1) {}
//}