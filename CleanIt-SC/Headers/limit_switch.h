#ifndef LIMIT_SWITCH_H
#define LIMIT_SWITCH_H
#include "MKL25Z4.h"
#define SWITCH_FRONT 0
#define SWITCH_LEFT 1
#define SWITCH_RIGHT 2

void limit_switch_init(void);
bool hasNOT_collided(int pinNumber); 

void limit_switch_init(void){
    SIM->SCGC5 |= 0x0800;  /* enable clock to Port C */
    PORTC->PCR[0] = 0x103; /* PTC0, GPIO, enable pullup*/
    PORTC->PCR[1] = 0x103; /* PTC1, GPIO, enable pullup*/
    PORTC->PCR[2] = 0x103; /* PTC2, GPIO, enable pullup*/
    PTC->PDDR = 0x07; /* make PTC2-0 as input pins */
}

bool hasNOT_collided(int pinNumber) {
    return (PTC->PDIR >> pinNumber) & 1;
}

#endif