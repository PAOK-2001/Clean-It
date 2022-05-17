#ifndef PROXIMITY_H
#define PROXIMITY_H
#include <MKL25Z4.h>
#include "delays.h"

void adc_init(void);
void proximity_init(void);
int proximity_read(int sensorValue);

//adc_init()
// Initialized clock for ADC0 and configures necessary register.
void adc_init(void){
    SIM->SCGC6 |= 0x8000000; /* clock to ADC0 */
    ADC0->SC2 &= ~0x40; /* software trigger */
    /*CLKDIV/4, LS time, single ended 12 bit, bus clock */
    ADC0->CFG1 = 0x40 | 0x10 | 0x04 | 0x00;
}
//proximity_init()
// Configure necessary ports as analogue
void proximity_init(void){
    adc_init();
    // Initialize clock to PORTB
    SIM->SCGC4 |= 0x400;
    // Set PORT B0 & PORT B1 as analogue input
    PORTB->PCR[0] = 0;
    PORTB->PCR[1] = 1;
    
}
//proximity_read()
// @param  integer corresponding to ADC channel. Should be 8 or 9.
// @returns int value corresponding to the sensor value
int proximity_read(int sensorChannel){
    int readValue;
    ADC0->SC1[0] = sensorChannel;
    while(!(ADC0->SC1[0] & 0x80)) { } /* wait for COCO */
    readValue = ADC0->R[0]; 
    return readValue;
}

#endif