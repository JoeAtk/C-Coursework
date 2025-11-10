#ifndef ROBOT_H
#define ROBOT_H

typedef struct {
    int x;
    int y;
    int direction;  // 1=up, 2=right, 3=down, 4=left
} Robot;

extern Robot bobby;
extern int carryingMarkerCount;
extern double sleepTime;

void drawRobot(int x, int y, int rotation);
int canMoveForward();
void forward();
void right();
void left();
int atMarker();
void pickUpMarker();
void dropMarker();

#endif