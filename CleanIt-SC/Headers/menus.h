#ifndef MENUS_H
#define MENUS_H
#include <MKL25Z4.h>
#include <cstdio>
#include "delays.h"
#include "lcd.h"
#include "keypad.h"

void menus_init(void);
int mode_select(void);
int time_select(void);
int pattern_select(void);

// menus_init()
// Intilizies LCD and keypad for menu management
void menus_init() {
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
    delayMs(2000);
    // Display modes
    send_string("1.Auto 2.Pattern\nSelected:", 26);
    // Input loop for mode select until "# or *" is pressed
    do {
        //Get key from keypad
        key = keypad_getkey();
        delayMs(300);
        // if key is # or * set isSelected as true and set mode as key value
        if (key == 13 || key == 15) {
            if (mode == 1 || mode == 2) return mode;
        } else {
            if (key == 1 || key == 2) {
                mode = key; // set selected mode as entered key
                LCD_data(mode+48); // Display selected mode #number
            }
        }
    } while (1);
    return key;
}

// time_select()
// Ask user to input time in seconds, ends when # or * is pressed
// @returns int with number of seconds to execute mode
int time_select() {
    bool isSelected;
    char key;
    int returnTime = 0;
    isSelected = false;
    // Display input prompt
    send_string("Clean time:\n",12);
    do {
        //Get key from keypad
        key = keypad_getkey();
        delayMs(300);
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
    send_string("Select cleaning\npattern", 23);
    delayMs(2000);
    // Input loop for mode select until "# or *" is pressed
    isSelected = false;
    isValid    = false;
    // Display modes
    send_string("1.E 2.S 3.B 4.R\nSelected:", 25);
    do {
        //Get key from keypad
        key = keypad_getkey();
        delayMs(300);
        // if key is # or * set isSelected as true and set mode as key value
        if (key == 13 || key == 15) {
            if (pattern > 0 && pattern < 5) break;
        } else {
            if (key <= 5 && key > 0 && key != 4) {
                pattern = key - key/4; // set selected mode as entered key
                LCD_data(pattern+48); // Display selected mode #number
            }
        }
    } while (1);

    if (pattern == 1) {
        send_string("Deep clean?\n1.Yes 2.No", 22);
        do {
            //Get key from keypad
            key = keypad_getkey();
            delayMs(300);
            // Display modes
            if (key == 13 || key == 15) {
                if (pattern >= 0 && pattern < 5) return pattern;
            } else {
                if (key == 1 || key == 2) {
                    pattern = pattern-(key%2) ; // set selected mode as entered key
                    LCD_data(key+48);
                }
            }
        } while (1);
    }
    return pattern; //0:empty room clean (deep) 1:empty room clean (not deep) 2:spill clean 3:Battery test 4:room outline
}
    
#endif