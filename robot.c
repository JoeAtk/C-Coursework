#include "robot.h"
#include "arena.h"
#include "graphics.h"
#include <math.h>

#define M_PI 3.14159265358979323846

Robot bobby = {0, 0, 1};
int carryingMarkerCount = 0;   
double sleepTime = 100;

void drawRobot(int x, int y, int rotation)
{
    background();
    foreground();
    clear();
    
    int xPositions[3];
    int yPositions[3];
    int gridPixelSize = windowSize / gridSize;
    int robotSize = (gridPixelSize) / 2 - 2;
    int horiOffset = sin(M_PI / 3) * robotSize;
    int vertOffset = cos(M_PI / 3) * robotSize;
    int baseX = 5 + (x * gridPixelSize) - (gridPixelSize / 2);
    int baseY = 5 + (y * gridPixelSize) - (gridPixelSize / 2);
    
    if (rotation == 1) {
        int upBaseX = baseX + ((robotSize - horiOffset) / 2);
        int upBaseY = baseY + ((robotSize - vertOffset) / 2);
        xPositions[0] = upBaseX;
        yPositions[0] = upBaseY - robotSize;
        xPositions[1] = upBaseX + horiOffset;
        yPositions[1] = upBaseY + vertOffset;
        xPositions[2] = upBaseX - horiOffset;
        yPositions[2] = upBaseY + vertOffset;
    } else if (rotation == 2) {
        int rightBaseX = baseX - ((robotSize - vertOffset) / 2);
        int rightBaseY = baseY;
        xPositions[0] = rightBaseX + robotSize;
        yPositions[0] = rightBaseY;
        xPositions[1] = rightBaseX - vertOffset;
        yPositions[1] = rightBaseY + horiOffset;
        xPositions[2] = rightBaseX - vertOffset;
        yPositions[2] = rightBaseY - horiOffset;
    } else if (rotation == 3) {
        int downBaseX = baseX;
        int downBaseY = baseY - ((robotSize - vertOffset) / 2);
        xPositions[0] = downBaseX;
        yPositions[0] = downBaseY + robotSize;
        xPositions[1] = downBaseX + horiOffset;
        yPositions[1] = downBaseY - vertOffset;
        xPositions[2] = downBaseX - horiOffset;
        yPositions[2] = downBaseY - vertOffset;
    } else {
        int leftBaseX = baseX + ((robotSize - vertOffset) / 2);
        int leftBaseY = baseY;
        xPositions[0] = leftBaseX - robotSize;
        yPositions[0] = leftBaseY;
        xPositions[1] = leftBaseX + vertOffset;
        yPositions[1] = leftBaseY + horiOffset;
        xPositions[2] = leftBaseX + vertOffset;
        yPositions[2] = leftBaseY - horiOffset;
    }
    
    setColour(darkgray);
    fillPolygon(3, xPositions, yPositions);
    sleep(sleepTime);
    
    if (carryingMarkerCount > 0) {
        setColour(blue);
        fillOval(baseX, baseY, gridPixelSize / 4, gridPixelSize / 4);
    }
}

int canMoveForward()
{
    
    if (bobby.direction == 1) {
        if (bobby.y > 1 && grid[bobby.x][bobby.y - 1] != 1) {
            return 1;
        }
    } else if (bobby.direction == 2) {
        if (bobby.x < gridSize && grid[bobby.x + 1][bobby.y] != 1) {
            return 1;
        }
    } else if (bobby.direction == 3) {
        if (bobby.y < gridSize && grid[bobby.x][bobby.y + 1] != 1) {
            return 1;
        }
    } else if (bobby.direction == 4) {
        if (bobby.x > 1 && grid[bobby.x - 1][bobby.y] != 1) {
            return 1;
        }
    }
    return 0;
}

void forward()
{
    if (canMoveForward() != 1) {
        return;
    }
    
    if (bobby.direction == 1) {
        bobby.y--;
    } else if (bobby.direction == 2) {
        bobby.x++;
    } else if (bobby.direction == 3) {
        bobby.y++;
    } else if (bobby.direction == 4) {
        bobby.x--;
    }
    drawRobot(bobby.x, bobby.y, bobby.direction);
}

void right()
{
    bobby.direction++;
    if (bobby.direction > 4) {
        bobby.direction = 1;
    }
    drawRobot(bobby.x, bobby.y, bobby.direction);
}

void left()
{
    bobby.direction--;
    if (bobby.direction < 1) {
        bobby.direction = 4;
    }
    drawRobot(bobby.x, bobby.y, bobby.direction);
}

int atMarker()
{
    if (grid[bobby.x][bobby.y] == 2) {
        return 1;
    }
    return 0;
}

void pickUpMarker()
{
    if (grid[bobby.x][bobby.y] == 2) {
        grid[bobby.x][bobby.y] = 0;
        emptyCell(bobby.x, bobby.y);
        carryingMarkerCount++;
    }
}

void dropMarker()
{
    extern void drawMarker(int x, int y);
    
    if (carryingMarkerCount > 0) {
        drawMarker(bobby.x, bobby.y);
        carryingMarkerCount--;
    }
    drawRobot(bobby.x, bobby.y, bobby.direction);
}