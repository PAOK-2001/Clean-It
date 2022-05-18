#ifndef PROXIMITY_H
#define PROXIMITY_H
#include <MKL25Z4.h>
#include <cstdio>
#include<math.h>
#include "delays.h"

void adc_init(void);
void proximity_init(void);
double proximity_read_average(int sensorValue, int meassurments);

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
    SIM->SCGC5 |= 0x400;
    // Set PORT B0 & PORT B1 as analogue input
    PORTB->PCR[0] = 0;
    PORTB->PCR[1] = 1;
    
}
//proximity_read()
// @param  sensorChannel: int corresponding to ADC channel. Should be 8 or 9.
// @param  meassurments: measurments to average
// @returns float in cm
double proximity_read_average(int sensorChannel, int meassurments){
    int rawValue,sum;
    double valueCm;
    sum = 0;
    for (int i = 0; i < meassurments; i++){
        ADC0->SC1[0] = sensorChannel;
        while(!(ADC0->SC1[0] & 0x80)) { } /* wait for COCO */
        sum = sum + ADC0->R[0];
        delayMs(50);
    }
    rawValue = sum/meassurments;
    valueCm  = 66.3 -0.0507*rawValue + 0.0000163*pow(rawValue,2) -0.00000000188*pow(rawValue,3);
    return valueCm;
}

#endif


/* Driver code

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
    double leftSensor, rightSensor;
    double test = 9.64;
    while (true) {
        leftSensor = proximity_read_average(8,20);
        printf("%.9f \n", leftSensor);
        tpm_delayMs(500);
        // rightSensor = proximity_read_average(9, 20);
        // printf("Right Sensor: %d\n\n", rightSensor);
        // tpm_delayMs(250);
    }
}
*/