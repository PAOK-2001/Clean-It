#include "mbed.h"
#include <MKL25Z4.h>
#include "Headers/menus.h"
#include "Headers/pathFollower.h"
#include "Headers/autonomousNavigation.h"
#include "Headers/mosfet.h"

void systemInit(void);

int main() {
    int mode, time, pattern;
    systemInit();
    while (1) {
        mode = mode_select();
        if (mode == 1) {
            time = time_select();
            mosfet_on();
            autonomousRoutine(time);
            mosfet_off();
        } else {
            pattern = pattern_select();
            if (pattern == 2 || pattern == 4) time = time_select();
            mosfet_on();
            switch (pattern) {
            case 0:
                emptyRoomClean(true);
                break;
            case 1:
                emptyRoomClean(false);
                break;
            case 2:
                spillClean(time);
                break;
            case 3:
                batteryTest();
                break;
            case 4:
                roomOutline(time);
                break;
            default:
                break;
            }
            mosfet_off();
        }
    }
}

void systemInit() {
    collisionDetectionInit();
    mosfet_init();
    hbridge_init();
    menus_init();
}

