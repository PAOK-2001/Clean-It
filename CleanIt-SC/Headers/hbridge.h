#ifndef HBRIDGE_H
#define HBRIDGE_H
#include <MKL25Z4.h>

static float mod = 41940;

void hbridge_init(void) {
    SIM->SCGC5 |= 0x0800;  /* enable clock to Port C */
    SIM->SCGC5 |= 0x0200;  /* enable clock to Port A */
    PORTC->PCR[9] = 0x0300; /* PTC9 used by TPM0ch5 */
    PORTC->PCR[8] = 0x0300; /* PTC8 used by TPM0ch4 */
    PORTA->PCR[5] = 0x0300; /* PTA5 used by TPM0ch2 */
    PORTA->PCR[4] = 0x0300; /* PTA4 used by TPM0ch1 */
    SIM->SCGC6 |= 0x01000000; /* enable clock to TPM0 */
    SIM->SOPT2 |= 0x01000000; /* use MCGFLLCLK as timer counter clock */
    TPM0->SC = 0; /* disable timer */
    TPM0->SC = 0x00; // prescaler 4

    TPM0->CONTROLS[5].CnSC = 0x20 | 0x08;
    TPM0->CONTROLS[4].CnSC = 0x20 | 0x08;
    TPM0->CONTROLS[2].CnSC = 0x20 | 0x08;
    TPM0->CONTROLS[1].CnSC = 0x20 | 0x08;
    TPM0->MOD = mod; // modulo 41940
    TPM0->CONTROLS[5].CnV = 0;
    TPM0->CONTROLS[4].CnV = 0;
    TPM0->CONTROLS[2].CnV = 0;
    TPM0->CONTROLS[1].CnV = 0;
    TPM0->SC |= 0x08; // enable timer free-running mode
}

void forward(float speed){
    TPM0->CONTROLS[5].CnV = mod * speed;
    TPM0->CONTROLS[4].CnV = 0;
    TPM0->CONTROLS[2].CnV = 0;
    TPM0->CONTROLS[1].CnV = mod * speed;
}

void backward(float speed){
    TPM0->CONTROLS[5].CnV = 0;
    TPM0->CONTROLS[4].CnV = mod * speed;
    TPM0->CONTROLS[2].CnV = mod * speed;
    TPM0->CONTROLS[1].CnV = 0;
}

void rotate_left(float speed){
    TPM0->CONTROLS[5].CnV = mod * speed;
    TPM0->CONTROLS[4].CnV = 0;
    TPM0->CONTROLS[2].CnV = mod * speed;
    TPM0->CONTROLS[1].CnV = 0;
}


void rotate_right(float speed){
    TPM0->CONTROLS[5].CnV = 0;
    TPM0->CONTROLS[4].CnV = mod * speed;
    TPM0->CONTROLS[2].CnV = 0;
    TPM0->CONTROLS[1].CnV = mod * speed;
}
#endif