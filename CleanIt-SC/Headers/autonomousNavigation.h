#ifndef AUTONOMOUSNAVIGATION_H
#define AUTONOMOUSNAVIGATION_H
#include "hbridge.h"
#include "collisionAvoidance.h"
#include "delays.h"
#include "pathFollower.h"
#include <stdlib.h>
#include <stdio.h>
#define ONE_DEGREE_TURN_TIME 0.0066f
#define SPIRAL_TIME 15
#define WALL_TIME 5
#define MS_UNTIL_CLEAR 5000

void turnNDegreesLeft(int, int*);
void turnNDegreesRight(int, int*);
void spiral(int, int*);
void straightToWall(int*);
void moveAlongWall(int, int*);
bool headToClear(int*);
void autonomousRoutine(int);

void turnNDegreesLeft(int degrees, int* routineTime) {
    if (degrees > 0) {
        rotate_left(NORMAL_OPERATION_SPEED);
        delayMs(ONE_DEGREE_TURN_TIME*degrees*1000);
        stop();
    } else turnNDegreesRight(degrees, routineTime);
    *routineTime = *routineTime - ONE_DEGREE_TURN_TIME*degrees*1000;
}

void turnNDegreesRight(int degrees, int* routineTime) {
    if (degrees > 0) {
        rotate_right(NORMAL_OPERATION_SPEED);
        delayMs(ONE_DEGREE_TURN_TIME*degrees*1000);
        stop();
    } else turnNDegreesLeft(degrees, routineTime);
    *routineTime = *routineTime - ONE_DEGREE_TURN_TIME*degrees*1000;
}

//step 1: spiral for a set amount of time
void spiral(int time, int* routineTime) {
    printf("Spiral\n");
    int msLeft = time*1000;
    int forwardTimeMs = ROOMBA_LENGTH_TIME*1000;
    int temp;
    while (msLeft > 0) {
        for (int i=0; i<2; i++) {
            temp = forwardTimeMs;
            while (temp > 0) {
                forward(NORMAL_OPERATION_SPEED);
                delayMs(ROOMBA_LENGTH_TIME*1000);
                temp -= ROOMBA_LENGTH_TIME*1000;
            }
            turnNDegreesLeft(90, routineTime);
        }
        msLeft -= forwardTimeMs*2 + ONE_DEGREE_TURN_TIME*90*1000*2;
        *routineTime = *routineTime - (forwardTimeMs*2 + ONE_DEGREE_TURN_TIME*90*1000*2);
        forwardTimeMs += ROOMBA_LENGTH_TIME*1000;
    } 
    int degrees = rand()%181 - 90;
    turnNDegreesLeft(degrees, routineTime);
    printf("Spiral done\n");
}

//step 2: head for the perimeter
void straightToWall(int* routineTime) {
    printf("Straight to wall\n");
    forward(NORMAL_OPERATION_SPEED);
    int wallState = tooCloseToWall();
    int collisionState = collisionDetected();
    while(!(wallState || collisionState)) {
        wallState = tooCloseToWall();
        collisionState = collisionDetected();
        delayMs(50);
        *routineTime = *routineTime - 50;
    }
    stop();
    printf("Straight to wall done\n");
}

//step 3: move along the wall/edge for some time
void moveAlongWall(int time, int* routineTime) {
    printf("Move along wall\n");
    roomOutline(time);
    *routineTime = *routineTime - time*1000;
    printf("Move along wall done\n");
}

//step 4: head for a clear path (and spiral if a wall is not hit for an amount of time)
bool headToClear(int* routineTime) {
    printf("Head to clear\n");
    int proximityState = tooCloseToWall();
    int collisionState = collisionDetected();
    int time = 0;
    switch (proximityState) {
    case 1:
        while(proximityState) {
            proximityState = tooCloseToWall();
            turnNDegreesRight(15, routineTime);
        }
        turnNDegreesRight(45, routineTime);
        break;
    case 2:
        while(proximityState) {
            proximityState = tooCloseToWall();
            turnNDegreesLeft(15, routineTime);
        }
        turnNDegreesLeft(45, routineTime);
        break;
    case 3:
        turnNDegreesLeft(180, routineTime);
        break;
    default:
        break;
    }
    forward(NORMAL_OPERATION_SPEED);
    while (time < MS_UNTIL_CLEAR) {
        proximityState = tooCloseToWall();
        collisionState = collisionDetected();
        delayMs(100);
        time += 300;
        if (proximityState || collisionState) break;
    }
    stop();
    *routineTime = *routineTime - time;
    printf("Head to clear done\n");
    return time >= MS_UNTIL_CLEAR;
}

void autonomousRoutine(int time) {
    int msTime = time*1000;
    bool clear = true;
    while (time > 0) {
        if (clear) {
            spiral(SPIRAL_TIME, &msTime);
            delayMs(100);
            straightToWall(&msTime);
            delayMs(100);
        }
        moveAlongWall(WALL_TIME, &msTime);
        delayMs(100);
        clear = headToClear(&msTime);
        delayMs(100);
    }
}

#endif