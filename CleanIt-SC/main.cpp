#include "mbed.h"
#include <MKL25Z4.h>
#include"Headers/proximity.h"
// main() runs in its own thread in the OS
int main(){ 
    proximity_init();
    int leftSensor, rightSensor;
    while (true) {
        leftSensor = proximity_read(8);
    }
}

