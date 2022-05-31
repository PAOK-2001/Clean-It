#ifndef MENUS_H
#define MENUS_H
#include <MKL25Z4.h>
#include <cstdio>
#include "delayTPM0.h"
#include "lcd.h"
#include "keypad.h"

void menus_init(void);
int mode_select(void);
int time_select(void);
int pattern_select(void);

// menus_init()
// Intilizies LCD and keypad for menu management
void menus_init() {
    tpm_init();
    LCD_init();
    keypad_init();
}

//mode_select()
// Ask user to input a mode.
// @returns int with selected mode
int mode_select() {
    // Bool flag for selected
    bool isSelected, isValid;
    char key;
    // return value with selected mode
    int mode;
    // Show "Welcome" menu for 4 seconds
    send_string("Welcome!\nSelect mode", 20);
    tpm_delayMs(4000);
    // Input loop for mode select until "# or *" is pressed
    isSelected = false;
    isValid    = false;
    do {
        //Get key from keypad
        key = keypad_getkey();
        // Display modes
        send_string("1.Auto 2.Pattern\nSelected:", 25);
        mode = key - key/4; // set selected mode as entered key
        LCD_data((mode)+48) // Display selected mode #number
        // if key is # or * set isSelected as true and set mode as key value
        if (key == 13 || key == 15) isSelected = true;
        if (mode <= 2 && mode > 0) isValid = true;
        tpm_delayMs(300);
    } while (!isSelected || !isValid);
    return mode;
}

// time_select()
// Ask user to input time in seconds, ends when # or * is pressed
// @returns int with number of seconds to execute mode
int time_select() {
    bool isSelected;
    char key;
    int returnTime;
    isSelected = false;
    do {
        //Get key from keypad
        key = keypad_getkey();
        // Display input prompt
        send_string("Clean time:\n",12);
        if (key == 13 || key == 15) isSelected = true; // if pound or asterisk is pressed
            else if (key > 0 && key < 13 && key % 4 != 0) { // if a number other than 0 is pressed
                returnTime *= 10;
                returnTime += key-key/4; // add number (compensating for keypad offset)
                LCD_data((key-key/4)+48); // display number
            } else if (key == 14) { // if 0 is pressed
                returnTime *= 10;
                LCD_data(48);
            }
        // if key is # or * set isSelected as true and set mode as key value 
    } while (!isSelected);
    return returnTime;
}

//pattern_select()
// Ask user to input the pattern to be followed.
// @returns int with selected pattern
int pattern_select() {
    // Bool flag for selected
    bool isSelected, isValid;
    char key;
    // return value with selected mode
    int pattern;
    // Show "Welcome" menu for 4 seconds
    send_string("Select cleaning\npattern:", 24);
    tpm_delayMs(4000);
    // Input loop for mode select until "# or *" is pressed
    isSelected = false;
    isValid    = false;
    do {
        //Get key from keypad
        key = keypad_getkey();
        // Display modes
        send_string("1.E 2.S 3.B 4.R\nSelected:", 25);
        pattern = key - key/4; // set selected mode as entered key
        LCD_data((pattern)+48) // Display selected mode #number
        // if key is # or * set isSelected as true and set mode as key value
        if (key == 13 || key == 15) isSelected = true;
        if (pattern <= 4 && pattern > 0) isValid = true;
        tpm_delayMs(300);
    } while (!isSelected || !isValid);

    isSelected = false;
    isValid = false;

    if (pattern == 1) {
        do {
            //Get key from keypad
            key = keypad_getkey();
            // Display modes
            send_string("Deep clean?\n1.Yes 2.No", 22);
            pattern = (key == 0 || key > 2) ? -1 : pattern-(key%2) ; // set selected mode as entered key
            // if key is # or * set isSelected as true and set mode as key value
            if (key == 13 || key == 15) isSelected = true;
            if (pattern != -1) isValid = true;
            tpm_delayMs(300);
        } while (!isSelected || !isValid);
    }
    return pattern; //0:empty room clean (deep) 1:empty room clean (not deep) 2:spill clean 3:Battery test 4:room outline
}
    
#endif