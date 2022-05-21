#ifndef PATHFOLLOWER_H
#define PATHFOLLOWER_H
#include "hbridge.h"
#include "collisionAvoidance.h"
#include "delays.h"
#define NORMAL_OPERATION_SPEED 0.1f
#define ROOMBA_LENGTH_TIME 1.2f
#define NINETY_DEGREE_TURN_TIME 1.0f
#define SENSOR_DISTANCE_AT_WALL 3
#define STOP_TIME 0.5f

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
        if (sensorFacingEnd) {
            turn90Right();
            forward(NORMAL_OPERATION_SPEED);
            delayMs(ROOMBA_LENGTH_TIME*1000);
            stop();
            turn90Right();
        } else {
            turn90Left();
            forward(NORMAL_OPERATION_SPEED);
            delayMs(ROOMBA_LENGTH_TIME*1000);
            stop();
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
    int temp;
    int iterations = 1;
    while (msLeft > 0) {
        for (int i=0; i<2; i++) {
            temp = forwardTimeMs;
            while (temp > 0) {
                forward(NORMAL_OPERATION_SPEED);
                delayMs(ROOMBA_LENGTH_TIME*1000);
                stop();
                delayMs(STOP_TIME*1000);
                temp -= ROOMBA_LENGTH_TIME*1000;
            }
            turn90Left();
        }
        msLeft -= forwardTimeMs + STOP_TIME*1000*iterations*2 + NINETY_DEGREE_TURN_TIME*1000*2;
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
    int msTime = time * 1000;
    int proximityState;
    while (msTime > 0) {
        proximityState = tooCloseToWall();
        if (proximityState == 1) { //left too close, right not too close
            forward(NORMAL_OPERATION_SPEED);
        } else if (proximityState == 0) { //both not too close
            turn90Left();
        } else if (proximityState == 2) {
            turn90Right();
        }
        msTime--;
        delay1ms();
    }
    stop();
}

#endif