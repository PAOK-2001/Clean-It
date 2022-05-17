#include "mbed.h"
#include <MKL25Z4.h>
#include"Headers/proximity.h"
#include "Headers/delayTPM0.h"
#include<cstdio>
 
// main() runs in its own thread in the OS
int main(){ 
    printf("Pene\n");
    tpm_delayMs(10);
    proximity_init();
    int leftSensor, rightSensor;
    while (true) {
        leftSensor = proximity_read(8);
        
    }
}

