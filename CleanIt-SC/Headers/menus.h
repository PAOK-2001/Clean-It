#ifndef MENUS_H
#define MENUS_H
#include <MKL25Z4.h>
#include<cstdio>
#include"delays.h"
#include"lcd.h"
#include"keypad.h"

void menus_init(void);
int mode_select(void);
int time_select(void);

// menus_init()
// Intilizies LCD and keypad for menu management
void menus_init(){
    LCD_init();
    keypad_init();
}
//mode_-select()
// Ask user to input a mode
int mode_select(){
    // Bool flag for selected
    bool isSelected;
    // return value with selected mode
    int mode;
    // Clear display and set cursor in first line
    // Show "Welcome" menu for 4 seconds
    // Input loop for mode select until "# or *" is pressed
    do{
        //Get key from keypad
        // Display modes
        // if key is # or * set isSelected as true and set mode as key value 
    } while (!isSelected);
    
    return mode
}
// time_select()
// Ask user to input time in seconds, ends until # or * is pressed
int time_select(){
    bool isSelected;
    int returnTime;
    do{
        //Get key from keypad
        // Display input prompt
        // if key is # or * set isSelected as true and set mode as key value 
    } while (/* condition */);
    
    return returnTime;
}
    
#endif