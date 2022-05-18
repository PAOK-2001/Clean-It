#ifndef HBRIDGE_H
#define HBRIDGE_H
#include <MKL25Z4.h>

void hbridge_init(void) {
    SIM->SCGC5 |= 0x0800;  /* enable clock to Port C */
    SIM->SCGC5 |= 0x0200;  /* enable clock to Port A */
    PORTC->PCR[9] = 0x0400; /* PTC9 used by TPM0ch5 */
    //PORTC->PCR[10] = 0x0400; /* PTC10 used by TPM0 */
    PORTA->PCR[5] = 0x0400; /* PTA5 used by TPM0ch2 */
    PORTA->PCR[4] = 0x0400; /* PTA4 used by TPM0ch1 */
    SIM->SCGC6 |= 0x01000000; /* enable clock to TPM0 */
    SIM->SOPT2 |= 0x01000000; /* use MCGFLLCLK as timer counter clock */
    TPM0->SC = 0; /* disable timer */

    TPM0->CONTROLS[5].CnSC = 0x20 | 0x08;
    TPM0->CONTROLS[2].CnSC = 0x20 | 0x08;
    TPM0->CONTROLS[1].CnSC = 0x20 | 0x08;
    TPM0->MOD = 43702;
    TPM0->CONTROLS[5].CnV = 14568;
    TPM0->CONTROLS[2].CnV = 14568;
    TPM0->CONTROLS[1].CnV = 14568;
}

#endif