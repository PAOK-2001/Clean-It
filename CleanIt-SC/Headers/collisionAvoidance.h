#ifndef COLLISION_AVOIDANCE_H
#define COLLISION_AVOIDANCE_H
#include "proximity.h"
#include "limit_switch.h"
#define CLOSEST_ALLOWED_DISTANCE 15

void collisionDetectionInit(void);
int collisionDetected(void);
int tooCloseToWall(void);

void collisionDetectionInit() {
    proximity_init();
    limit_switch_init();
}

int collisionDetected() {
    int state = 0x0;
    if (!hasNOT_collided(SWITCH_FRONT)) state |= 1;
    else if (!hasNOT_collided(SWITCH_LEFT)) state |= 2;
    else if (!hasNOT_collided(SWITCH_RIGHT)) state |= 4;
    return state;
}

int tooCloseToWall() {
    int state = 0x0;
    double leftSensor,rightSensor;
    leftSensor = proximity_read_average(8,20);
    rightSensor = proximity_read_average(9, 20);
    if (leftSensor < CLOSEST_ALLOWED_DISTANCE) state |= 1;
    else if (rightSensor < CLOSEST_ALLOWED_DISTANCE) state |= 2;
    return state;
}

int tooCloseToWall(double* distanceL, double* distanceR) {
    int state = 0x0;
    double leftSensor,rightSensor;
    leftSensor = proximity_read_average(8,20);
    rightSensor = proximity_read_average(9, 20);
    *distanceL = leftSensor;
    *distanceR = rightSensor;
    if (leftSensor < CLOSEST_ALLOWED_DISTANCE) state |= 1;
    else if (rightSensor < CLOSEST_ALLOWED_DISTANCE) state |= 2;
    return state;
}

#endif