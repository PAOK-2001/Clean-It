#ifndef PROXIMITY_H
#define PROXIMITY_H
#include <MKL25Z4.h>

void proximity_init();
uint16_t proximity_read();

void proximity_init(){
    SIM->SCGC5 |= 0x2000; /* clock to PORTE */
    PORTE->PCR[20] = 0; /* PTE20 analog input */
    SIM->SCGC6 |= 0x8000000; /* clock to ADC0 */
    ADC0->SC2 &= ~0x40; /* software trigger */
    /* clock div by 4, long sample time, single ended 12 bit, bus clock */
    ADC0->CFG1 = 0x40 | 0x10 | 0x04 | 0x00;
}

uint16_t proximity_read(){
    uint16_t readValue;

    return readValue;
}

#endif