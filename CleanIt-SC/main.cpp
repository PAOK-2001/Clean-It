#include "mbed.h"
#include <MKL25Z4.h>
#include"Headers/proximity.h"
#include "Headers/delayTPM0.h"
#include<cstdio>
 
// main() runs in its own thread in the OS
int main(){ 
    printf("Program Begin\n");
    tpm_init();
    proximity_init();
    int leftSensor, rightSensor;
    while (true) {
        leftSensor = proximity_read_average(8,20);
        printf("%d\n", leftSensor);
        tpm_delayMs(1000);
        
    }
}

