#ifndef MENUS_H
#define MENUS_H
#include <MKL25Z4.h>
#include<cstdio>
#include"delays.h"
#include"delayTPM0.h"
#include"lcd.h"
#include"keypad.h"

void menus_init(void);
int mode_select(void);
int time_select(void);

// menus_init()
// Intilizies LCD and keypad for menu management
void menus_init(){
    tpm_init();
    LCD_init();
    keypad_init();
}
//mode_select()
// Ask user to input a mode.
// @returns int with selected mode
int mode_select(){
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
    do{
        //Get key from keypad
        key = keypad_getkey();
        // Display modes
        send_string("1.Auto 2.Patern\nSelected:", 25);
        mode = key - key/4; // set selected mode as entered key
        LCD_data((mode)+48) // Display selected mode #number
        // if key is # or * set isSelected as true and set mode as key value
        if (key == 13 || key == 15) isSelected = true;
        if(mode <= 2) isValid = true;
        tpm_delayMs(300);
    } while (!isSelected && !isValid);
    return mode
}
// time_select()
// Ask user to input time in seconds, ends until # or * is pressed
int time_select(){
    bool isSelected;
    char key;
    int returnTime;
    isSelected = false;
    do{
        //Get key from keypad

        // Display input prompt

        // if key is # or * set isSelected as true and set mode as key value 
    } while (!isSelected);
    
    return returnTime;
}
    
#endif