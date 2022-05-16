#ifndef PROXIMITY_H
#define PROXIMITY_H
#include <MKL25Z4.h>

void adc_init(void);
void proximity_init(void);
uint16_t proximity_read(void);

//adc_init()
// Initialized clock for ADC0 and configures necessary register.
void adc_init(void){
    SIM->SCGC6 |= 0x8000000; /* clock to ADC0 */
    ADC0->SC2 &= ~0x40; /* software trigger */
    /*CLKDIV/4, LS time, single ended 12 bit, bus clock */
    ADC0->CFG1 = 0x40 | 0x10 | 0x04 | 0x00;
}
//Proximity_init
// Configure necessary ports as analogue
void proximity_init(void){
    // Initialize clock to PORTB
    SIM->SCGC4 |= 0x400;
    // Set PORT B0 & PORT B1 as analogue input
    PORTB->PCR[0] = 0;
    PORTB->PCR[1] = 1;
    adc_init();
}

uint16_t proximity_read(int sensorChannel){
    uint16_t readValue;

    return readValue;
}

#endif