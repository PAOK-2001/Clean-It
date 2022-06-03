#ifndef DELAYS_H
#define DELAYS_H
#include <MKL25Z4.h>

void delayMs(int n);
void delay1ms(void);
void delayUs(int n);
void delay1us(void);
 
/* Delay n milliseconds */
/*The CPU core clock is set to MCGFLLCLK at
41.94 MHz in SystemInit(). */
void delayMs(int n) {
    for (int i=0; i<n; i++) delay1ms();
}

void delayUs(int n) {
    for (int i=0; i<n; i++) delay1us();
}

void delay1ms(void) {
    SysTick->LOAD = 41939;
    SysTick->CTRL = 0x5; /* Enable the timer and choose sysclk as the clock source */
    while((SysTick->CTRL & 0x10000) == 0) /* wait until the COUNT flag is set */ { }
    SysTick->CTRL = 0; /* Stop the timer (Enable = 0) */
}

void delay1us(void) {
    SysTick->LOAD = 42;
    SysTick->CTRL = 0x5; /* Enable the timer and choose sysclk as the clock source */
    while((SysTick->CTRL & 0x10000) == 0) /* wait until the COUNT flag is set */ { }
    SysTick->CTRL = 0; /* Stop the timer (Enable = 0) */
}

#endif