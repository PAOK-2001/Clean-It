#ifndef KEYPAD_H
#define KEYPAD_H
#include <MKL25Z4.h>
#include "delays.h" 
void keypad_init(void);
char keypad_getkey(void);
void delayUs(int n);

/* Initializes Port that is connected to the keypad. */
/*  Pins as GPIO input pin with pullup enabled.*/ 
void keypad_init(void) {
    // Clock to PORTB AND PORTE
    SIM->SCGC5 |= 0x0400;  /* enable clock to Port B */
    SIM->SCGC5 |= 0x2000; /* enable clock to Port E */
    // Initialize rows, colums as GPIO
    //Rows
    PORTB->PCR[8] = 0x103; /*  PTB8, GPIO, enable pullup*/
    PORTB->PCR[9] = 0x103; /*  PTB9, GPIO, enable pullup*/
    PORTB->PCR[10] = 0x103; /* PTB10, GPIO, enable pullup*/
    PORTB->PCR[11] = 0x103; /* PTB11, GPIO, enable pullup*/
    //Columns
    PORTE->PCR[2] = 0x103; /*  PTE2, GPIO, enable pullup*/
    PORTE->PCR[3] = 0x103; /*  PTE3, GPIO, enable pullup*/
    PORTE->PCR[4] = 0x103; /*  PTE4, GPIO, enable pullup*/
    PORTE->PCR[5] = 0x103; /*  PTE5, GPIO, enable pullup*/
    // make Rows as output and Columns as input pins 
    PTB->PDDR |= 0xF00; //rows as output
    PTE->PDDR |= 0x00; //columns as input
    //PTC->PDDR = 0x0F; 
}
 
/* keypad_getkey()
* If a key is pressed, it returns a key code. Otherwise, a zero is returned.
The upper nibble of Port C is used as input. Pull-ups are enabled when the keys are not pressed
* The lower nibble of Port C is used as output that drives the keypad rows.
* First all rows are driven low and the input pins are read. If no key is pressed, it will read as all ones. Otherwise, some key is pressed.
* If any key is pressed, the program drives one row low at a time and leave the rest of the rows inactive (float) then read the input pins.
* Knowing which row is active and which column is active, the program can decide which key is pressed. */
 
char keypad_getkey(void) {
    int row, col;
    const int row_select[] = {0x100, 0x200, 0x400, 0x800};

    /* one row is active */
    /* check to see any key pressed */
    PTB->PDDR |= 0xF00; /* enable all rows */
    PTB->PCOR = 0xF00;
    delayUs(2); /* wait for signal return */
    col = PTE->PDIR & 0x3C; /* read all columns */
    PTB->PDDR = 0; /* disable all rows */
    if (col == 0x3C) return 0; /* no key pressed */
    
    /* If a key is pressed, we need find out which key.*/
    for (row = 0; row < 4; row++) { 
        PTB->PDDR = 0; /* disable all rows */
        PTB->PDDR |= row_select[row]; /* enable one row */
        PTB->PCOR = row_select[row]; /* drive active row low*/
        
        delayUs(2); /* wait for signal to settle */
        col = PTE->PDIR & 0x3C; /* read all columns */
    
        if (col != 0x3C) break; /* if one of the input is low, some key is pressed. */
    }
    
    PTB->PDDR = 0; /* disable all rows */
    
    if (row == 4) return 0; /* if we get here, no key is pressed */
    
    /* gets here when one of the rows has key pressed*/
    /*check which column it is*/
    
    if (col == 0x38) return row * 4 + 1; /* key in column 0 */
    if (col == 0x34) return row * 4 + 2; /* key in column 1 */
    if (col == 0x2C) return row * 4 + 3; /* key in column 2 */
    if (col == 0x1C) return row * 4 + 4; /* key in column 3 */
    return 0; /* just to be safe */
}
#endif