#include "mbed.h"
#include <MKL25Z4.h>
#include"Headers/proximity.h"
#include "Headers/delayTPM0.h"
#include<cstdio>
 
// main() runs in its own thread in the OS
int main(){ 
    printf("Program Begin\n");
    proximity_init();
    int leftSensor, rightSensor;
    while (true) {
        //leftSensor = proximity_read(8);
        printf("Loop\n");
        //printf("%d\n", leftSensor);
        
    }
}

