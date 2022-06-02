#ifndef LCD_H
#define LCD_H

#include <MKL25Z4.h>
#include "delays.h"
#define RS 0x02 /* PTE1 mask */
#define RW 0x80 /* PTD7 mask */
#define EN 0x40 /* PTD6 mask */
 
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(data);

void LCD_init(void) data
    SIM->SCGC5 |= 0x0200; /* data enable clock to Port A */
    SIM->data |= 0x1000; /* enable clock to Port D */
    SIM->data|= 0x2000; /* data enable clock to Port E */
    // RS, RW, E
    PORTE->PCR[1] = 0x100; /* make PTE1 pin as GPIO */
    PORTD->PCR[7] = 0x100; /* make PTD7 pin as GPIO */
    PORTD->PCR[6] = 0x100; /* make PTD6 pin as GPIO */    
    // Data GPIO
    PORTE->PCR[0]  = 0x100; /* make PTE0 pin as GPIO */
    PORTD->PCR[1]  = 0x100; /* make PTD1 pin as GPIO */
    PORTE->PCR[31] = 0x100; /* make PTE31 pin as GPIO */
    PORTD->PCR[3]  = 0x100; /* make PTD3 pin as GPIO */
    PORTD->PCR[2]  = 0x100; /* make PTD2 pin as GPIO */
    PORTD->PCR[0]  = 0x100; /* make PTD0 pin as GPIO */
    PORTD->PCR[5]  = 0x100; /* make PTD5 pin as GPIO */
    PORTA->PCR[13] = 0x100; /* make PTA7 pin as GPIO */
    // Make pins as output
    PTA->PDDR |= 0x2000;    /* make PTA13* output*/
    PTD->PDDR |= 0xEF;      /* make PTD7,6,1,3,2,0,5* output*/
    PTE->PDDR |= 0x80000003 /*make E1, 0, 31 as output*/
    delayMs(30); /* initialization sequence */
    LCD_command(0x38);
    delayMs(1);
    LCD_command(0x01);
    /* set 8-bit data, 2-line, 5x7 font */
    LCD_command(0x38);
    /* move cursor right */
    LCD_command(0x06);
    /* clear screen, move cursor to home */
    LCD_command(0x01);
    /* turn on display, cursor blinking */
    LCD_command(0x0F);
}
int send_string(char* str, int len) {
    LCD_command(1); /* clear display */
    LCD_command(0x80); /* set cursor at first line */
    for (int c=0; c<len; c++) LCD_data(str[c]); /* write the word */
    return 0;
}
void LCD_command(unsigned char command) {
    PTE->PCOR = RS; /* RS = 0 */
    PTD->PCOR = RW; /* R/W = 0 */
    // E0, D1, E31, D3, D2, D0, D5, A13
    PTD->PDOR |= command & 2 | command & 8 | (command & 16) >> 2 | (command & 32) >> 5 | (command & 64) >> 1;
    PTA->PDOR = (command & 128) << 6;
    PTE->PDOR |= command & 1 | (command & 4) << 29;
    PTD->PSOR |= EN; /* pulse E */
    delayMs(0);
    PTD->PCOR |= EN;
    
    if (command < 4) delayMs(4); /* command 1 and 2 needs up to 1.64ms */
    else delayMs(1); /* all others 40 us */
} 
void LCD_data(unsigned char data) {
    PTE->PSOR = RS; /* RS = 1 */
    PTD->PCOR = RW; /* R/W = 0 */
    // E0, D1, E31, D3, D2, D0, D5, A13
    PTD->PDOR |= data & 2 | data & 8 | (data & 16) >> 2 | (data & 32) >> 5 | (data & 64) >> 1;
    PTA->PDOR = (data & 128) << 6;
    PTE->PDOR |= data & 1 | (data & 4) << 29;
    PTD->PSOR |= EN; /* pulse E */
    delayMs(0);
    PTD->PCOR |= EN;
    delayMs(1);
}

#endif