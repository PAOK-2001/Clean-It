#ifndef LCD_H
#define LCD_H
 
#include <MKL25Z4.h>
#include "delays.h"
#define RS 0x20 /* PTD5 mask */
#define RW 0x40 /* PTD6 mask */
#define EN 0x80 /* PTD7 mask */
 
void LCD_nibble_write(unsigned char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);
 
void LCD_init(void)
{
SIM->SCGC5 |= 0x1000; /* enable clock to Port D */
 
PORTD->PCR[0] = 0x100; /* make PTD0 pin as GPIO */
PORTD->PCR[1] = 0x100; /* make PTD1 pin as GPIO */
PORTD->PCR[2] = 0x100; /* make PTD2 pin as GPIO */
PORTD->PCR[3] = 0x100; /* make PTD3 pin as GPIO */
PORTD->PCR[5] = 0x100; /* make PTD5 pin as GPIO */
PORTD->PCR[6] = 0x100; /* make PTD6 pin as GPIO */
PORTD->PCR[7] = 0x100; /* make PTD7 pin as GPIO */
 
PTD->PDDR |= 0xEF; /* make PTD0-3, 5, 6, 7 as output pins */
 
delayMs(30); /* initialization sequence */
LCD_nibble_write(0x03, 0);
delayMs(10);
LCD_nibble_write(0x03, 0);
delayMs(1);
 
LCD_nibble_write(0x03, 0);
delayMs(1);
LCD_nibble_write(0x02, 0); /* use 4-bit data mode */
delayMs(1);
LCD_command(0x28); /* set 4-bit D, 2-line, 5x7 font */
LCD_command(0x06); /* move cursor right */
LCD_command(0x01); /* clr screen, move cursor home */
LCD_command(0x0F); /* turn on display, cursor blink*/}
 
void LCD_nibble_write(unsigned char data, unsigned char control)
{
data &= 0x0F; /* clear upper nibble for control */
 
control &= 0xF0; /* clear lower nibble for data */
 
PTD->PDOR = data | control; /* RS = 0, R/W = 0 */
PTD->PDOR = data | control | EN; /* pulse E */
 
delayMs(0);
PTD->PDOR = data;
PTD->PDOR = 0;
}
 
void LCD_command(unsigned char command)
{
/* upper nibble first */
 
LCD_nibble_write(command >> 4, 0);
 
/* then lower nibble */
 
LCD_nibble_write(command & 0x0F, 0);
 
if (command < 4)
 
delayMs(4); /* commands 1 and 2 need up to 1.64ms */
else
delayMs(1); /* all others 40 us */
}
 
void LCD_data(unsigned char data)
{/* upper nibble first */
LCD_nibble_write(data >> 4, RS);
/* then lower nibble */
LCD_nibble_write(data & 0x0F, RS);
delayMs(1);
}

int send_string(char* str, int len) {
    LCD_command(1); /* clear display */
    LCD_command(0x80); /* set cursor at first line */
    for (int c=0; c<len; c++) {
        if (str[c] == '\n') LCD_command(0xC0); /* jump to second line for line break */
        else LCD_data(str[c]); /* write the word */
    }
    return 0;
}

#endif