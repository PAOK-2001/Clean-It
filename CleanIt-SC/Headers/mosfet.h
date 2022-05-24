#ifndef MOSFET_H
#define MOSFET_H
#include <MKL25Z4.h>

void mosfet_init(void);
void mosfet_on(void);
void mosfet_off(void);
// mosfet_init
// Initializes mosfet port as GPIO and sets it as output
void mosfet_init(void){
    SIM->SCGC5 |= 0x0200; /* enable clock to Port A */
    PORTA->PCR[12] = 0x100; /* make PTA12 pin as GPIO */
    // Make pins as output
    PTA->PDDR |= 0x1000;    /* make PTA12* output*/
}
// mosfet_on
// Sets mosfet pin as HIGH
void mosfet_on(void){
    PTA->PSOR = 0x1000;
}
// mosfet_off
// Sets mosfet pin as LOW
void mosfet_off(void){
    PTA->PCOR = 0x1000;
}

#endif