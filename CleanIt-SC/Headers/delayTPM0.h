#ifndef DELAYTPM0_H
#define DELAYTPM0_H
#include <MKL25Z4.h>

void tpm_init(void) {
    SIM->SCGC6 |= 0x01000000; // enable clock to TPM0
    SIM->SOPT2 |= 0x01000000; // use 41.94 MHz clock
    TPM0->SC = 0; // disable timer while configuring
    TPM0->SC = 0x00; // prescaler 4
    TPM0->MOD = 0xA3D4; // modulo 8192
    TPM0->SC |= 0x80; // clear TOF
    TPM0->SC |= 0x08; // enable timer free-running mode
}

void tpm_delayMs(int n) {
    for (int i=0; i<n; i++) {
        while((TPM0->SC & 0x80) == 0) { } // wait until the TOF is set
        TPM0->SC |= 0x80; // clear TOF
    }
}

#endif