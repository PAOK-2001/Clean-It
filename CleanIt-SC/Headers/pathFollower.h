#ifndef PATHFOLLOWER_H
#define PATHFOLLOWER_H
#include "hbridge.h"
#include "collisionAvoidance.h"
#include "delays.h"
#include <stdio.h>
#define NORMAL_OPERATION_SPEED 0.5f
#define ROOMBA_LENGTH_TIME 0.7f
#define NINETY_DEGREE_TURN_TIME 0.6f
#define SENSOR_DISTANCE_AT_WALL 12
#define STOP_TIME 0.3f

void turn90Left(void);
void turn90Right(void);
void emptyRoomClean(bool);
void spillClean(int);
void batteryTest(void);
void roomOutline(int);

void turn90Left() {
    rotate_left(NORMAL_OPERATION_SPEED);
    delayMs(NINETY_DEGREE_TURN_TIME*1000);
    stop();
}

void turn90Right() {
    rotate_right(NORMAL_OPERATION_SPEED);
    delayMs(NINETY_DEGREE_TURN_TIME*1000);
    stop();
}

/*
The robot follows a simple, zig zag pattern 
(long forward, turn right, short forward, turn right, long forward, 
turn left, short forward, turn left, repeat) until it gets to the 
end of the room.
*/
void emptyRoomClean(bool deep) {
    bool sensorFacingEnd = 1; //0 left, 1 right
    while (proximity_read_average(8+sensorFacingEnd, 20) >= SENSOR_DISTANCE_AT_WALL) {
        forward(NORMAL_OPERATION_SPEED);
        while (hasNOT_collided(SWITCH_FRONT)) {
            delayMs(ROOMBA_LENGTH_TIME*1000);
            if (deep) {
                stop(); 
                delayMs(STOP_TIME*1000);
                forward(NORMAL_OPERATION_SPEED);
            }
        }
        stop();
        delayMs(100);
        backward(NORMAL_OPERATION_SPEED);
        delayMs(500);
        if (sensorFacingEnd) {
            turn90Right();
            delayMs(100);
            forward(NORMAL_OPERATION_SPEED);
            delayMs(ROOMBA_LENGTH_TIME*1000);
            stop();
            delayMs(100);
            turn90Right();
        } else {
            turn90Left();
            delayMs(100);
            forward(NORMAL_OPERATION_SPEED);
            delayMs(ROOMBA_LENGTH_TIME*1000);
            stop();
            delayMs(100);
            turn90Left();
        }
        sensorFacingEnd = !sensorFacingEnd;
    }
    forward(NORMAL_OPERATION_SPEED);
    while (hasNOT_collided(SWITCH_FRONT)) {}
    stop();
}

/*
The robot does an outward spiral, implemented with 
an increasing counter, so as to clean a specific area. It stops 
every roomba length or so to do a more thorough cleaning.
*/
void spillClean(int totalTime) {
    int msLeft = totalTime*1000;
    int forwardTimeMs = ROOMBA_LENGTH_TIME*1000;
    int tempIter;
    int iterations = 1;
    while (msLeft > 0) {
        for (int i=0; i<2; i++) {
            tempIter = iterations;
            while (tempIter > 0) {
                forward(NORMAL_OPERATION_SPEED);
                delayMs(ROOMBA_LENGTH_TIME*1000);
                stop();
                delayMs(STOP_TIME*1000);
                tempIter--;
            }
            turn90Left();
        }
        msLeft -= forwardTimeMs*2 + STOP_TIME*1000*iterations*2 + NINETY_DEGREE_TURN_TIME*1000*2;
        forwardTimeMs += ROOMBA_LENGTH_TIME*1000;
        iterations++;
    } 
}

void batteryTest() { //The robot goes forward indefinitely.
    forward(1);
}

/*
The robot goes around the room, trying to minimize 
the measurement of the left proximity sensor and maximize the measurement 
of the right one. This is done to cover the areas that are usually dirtier 
after sweeping the floor.
*/
void roomOutline(int time) {
    int msTime = time*500;
    int proximityState;
    int switchState;
    double leftSensor, rightSensor;
    bool justLeft = false;
    bool justCorrected;
    bool lastCorrectionLeft = false;
    while (msTime > 0) {
        proximityState = tooCloseToWall(&leftSensor, &rightSensor);
        switchState = collisionDetected();
        if ((switchState & 1) && !(proximityState & 2) && justLeft) {
            printf("Turn right\n");
            backward(0.4);
            delayMs(200);
            turn90Right();
        } else if (proximityState & 2 || switchState & 4) { //right too close
            printf("Wall on right\n");
            turn90Right();
            delayMs(100);
            turn90Right();
        } else if (proximityState == 1 || switchState == 2 || justCorrected) { //left too close, right not too close
            printf("Wall on left\n");
            forward(NORMAL_OPERATION_SPEED);
            delayMs(300);
            justLeft = true;
            justCorrected = false;
        } else if (proximityState == 0 && leftSensor > CLOSEST_ALLOWED_DISTANCE + 15) { //both not too close
            printf("Sensors clear\n");
            forward(NORMAL_OPERATION_SPEED);
            delayMs(ROOMBA_LENGTH_TIME*1500);
            turn90Left();
            delayMs(100);
            forward(NORMAL_OPERATION_SPEED);
            delayMs(ROOMBA_LENGTH_TIME*2500);
        } else if (justLeft && !(proximityState & 1) ) {
            if (lastCorrectionLeft) {
                printf("Correcting to wall on right\n");
                rotate_right(NORMAL_OPERATION_SPEED/3);
                delayMs(250);
                forward(NORMAL_OPERATION_SPEED);
                delayMs(500);
                stop();
                lastCorrectionLeft = false;
            } else {
                printf("Correcting to wall on left\n");
                rotate_left(NORMAL_OPERATION_SPEED/3);
                delayMs(250);
                forward(NORMAL_OPERATION_SPEED);
                delayMs(500);
                stop();
                lastCorrectionLeft = true;
            }
            justCorrected = true;
            justLeft = false;
        } else {
            printf("Help me I'm stuck\n");
            stop();
            break;
        }
        delayMs(100);
        msTime -= 500;
    }
    stop();
}

#endif