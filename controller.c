#include "controller.h"
#include "robot.h"
#include "arena.h"
#include "graphics.h"
#include <stdlib.h>
#include <stdio.h>

#define array_SIZE GRID_SIZE*GRID_SIZE*4

int pathX[GRID_SIZE*GRID_SIZE];
int pathY[GRID_SIZE*GRID_SIZE];
int searchGrid[GRID_SIZE+1][GRID_SIZE+1]; //0=unvisited,1=visited

void wallFollow()
{
    if (canMoveForward() == 1) {
        forward();
    } else {
        right();
    }
    return;
}
int search(int x, int y,int arenaSize) 
{
    //UNUSED DFS
    
    if (grid[x][y] == 2) {
        return 1;
    }else if (grid[x][y] == 1)
    {
        searchGrid[x][y] = 1; 
        return 0;
    }else{
        searchGrid[x][y] = 1;
        if(x>1 && searchGrid[x-1][y] == 0){
            //if not far left grid and the left grid hasn't been searched yet
            if(search(x-1,y,arenaSize)==1){
                searchGrid[x][y] = 0;
                fillGrid(x,y);
                return 1;
            }
        }
        if(x<arenaSize && searchGrid[x+1][y]==0){
            //same as above but for the right.
            if(search(x+1,y,arenaSize)==1){
                searchGrid[x][y] = 0;
                fillGrid(x,y);
                return 1;
            }
        }
        if(y>1 && searchGrid[x][y-1]==0){
            //for above
            if(search(x,y-1,arenaSize)==1){
                searchGrid[x][y] = 0;
                fillGrid(x,y);
                return 1;
            }
        }
        if(y<arenaSize && searchGrid[x][y+1]==0){
            //for below
            if(search(x,y+1,arenaSize)==1){
                searchGrid[x][y] = 0;
                fillGrid(x,y);
                return 1;
            }
        }
    }
    
    return 0;
}
int bfsSearch(int x, int y, int arenaSize, int mode)
{
    //mode = either navigating to a marker or a drop location
    //initialise empty array for the queue and parent tracking
    arenaSize = gridSize;
    int queueX[array_SIZE];
    int queueY[array_SIZE];
    int front = 0;
    int rear = 0;
    int parentX[GRID_SIZE+1][GRID_SIZE+1];
    int parentY[GRID_SIZE+1][GRID_SIZE+1];
    //-1 = unvisited,-2= root
    //set every node to unvisited
    for (int i = 1; i <= arenaSize; i++) {
        for (int j = 1; j <= arenaSize; j++) {
            parentX[i][j] = -1;
            parentY[i][j] = -1;
        }
    }
    
    queueX[rear] = x;
    queueY[rear] = y;
    //initial element.
    rear++;
    parentX[x][y] = -2;
    parentY[x][y] = -2;
    
    int found = 0;
    int markerX = -1;
    int markerY = -1;
    
    //loop until queue is empty or marker found
    while (front < rear) {
        //set current to the queue front and add one to front
        int currX = queueX[front];
        int currY = queueY[front];
        front++;
        
        if (grid[currX][currY] == mode) {
            found = 1;
            markerX = currX;
            markerY = currY;
            break;
            //break when marker found as by definition of BFS it must be the shortest path
        }
        
        //explore all neighbours that haven't been visited
        //above:
        if (rear >= array_SIZE - 4) {
            printf("Queue overflow detected!\n");
            return 0; //exit the function to prevent infinite loop
        }
        
        if (currY > 1 && grid[currX][currY-1] != 1 && parentX[currX][currY-1] == -1) {
            //if not top row, not an obstacle and unvisited
            queueX[rear] = currX;
            queueY[rear] = currY - 1;
            rear++;
            parentX[currX][currY-1] = currX;
            parentY[currX][currY-1] = currY;
            //add to queue and set parent to current cell.
        }
        if (currY < arenaSize && grid[currX][currY+1] != 1 && parentX[currX][currY+1] == -1) {
            //if not bottom row, not an obstacle and unvisited
            queueX[rear] = currX;
            queueY[rear] = currY + 1;
            rear++;
            parentX[currX][currY+1] = currX;
            parentY[currX][currY+1] = currY;
            //add to queue and set parent to current cell.
        }
        if (currX > 1 && grid[currX-1][currY] != 1 && parentX[currX-1][currY] == -1) {
            //if not left column, not an obstacle and unvisited
            queueX[rear] = currX - 1;
            queueY[rear] = currY;
            rear++;
            parentX[currX-1][currY] = currX;
            parentY[currX-1][currY] = currY;
            //add to queue and set parent to current cell.
        }
        if (currX < arenaSize && grid[currX+1][currY] != 1 && parentX[currX+1][currY] == -1) {
            //if not right column, not an obstacle and unvisited
            queueX[rear] = currX + 1;
            queueY[rear] = currY;
            rear++;
            parentX[currX+1][currY] = currX;
            parentY[currX+1][currY] = currY;
            //add to queue and set parent to current cell.
        }
        //every unvisited cell has now been added to the queue.
    }
    
    //walk back the path now if the marker was found
    if (found == 1) {
        int pathLength = 0;
        int currentX = markerX;
        int currentY = markerY;
        
        while (currentX != -1 && currentX != -2) {
            //loop until the root is reached.
            pathX[pathLength] = currentX;
            pathY[pathLength] = currentY;
            pathLength++;
            if (parentX[currentX][currentY] == -2 && parentY[currentX][currentY] == -2) {
                //if the parent is the root, exit the loop
                break;
            }
            //cX as otherwise the currentX would be overwritten before getting to currentY
            int cX = currentX;
            //set current to parent for next loop
            currentX = parentX[currentX][currentY];
            currentY = parentY[cX][currentY];
        }
        
        //reverse the path array.
        int j = 0;
        int tempArrayX[GRID_SIZE*GRID_SIZE];
        int tempArrayY[GRID_SIZE*GRID_SIZE];
        for (int i = 0; i < pathLength; i++) {
            tempArrayX[i] = pathX[i];
            tempArrayY[i] = pathY[i];
        }
        for (int i = pathLength - 1; i >= 0; i--) {
            pathX[j] = tempArrayX[i];
            pathY[j] = tempArrayY[i];
            j++;
        }
        //return distance to the nearest objective (marker/drop location)
        return pathLength;
    }
    //return 0 if no path found/possible
    return 0;
}

void navigatePath(int pathX[], int pathY[], int pathLength)
{
    for (int i = 0; i < pathLength; i++) {
        //find the direction relative to robot direction
        if (pathX[i] == bobby.x && pathY[i] == bobby.y) {
            //same cell do nothing
            continue;
        }
        if (pathY[i] < bobby.y) {
            //target cell is above
            if (bobby.direction == 2) {
                left();
            } else {
                while (bobby.direction != 1) {

                    right();
                }
            }
        } else if (pathY[i] > bobby.y) {
            if (bobby.direction == 4) {
                left();
            } else {
                while (bobby.direction != 3) {
                  
                    right();
                }
            }
        } else if (pathX[i] > bobby.x) {
            if (bobby.direction == 3) {
                left();
            } else {
                while (bobby.direction != 2) {
                  
                    right();
                }
            }
        } else if (pathX[i] < bobby.x) {
            if (bobby.direction == 1) {
                left();
            } else {
                while (bobby.direction != 4) {
                
                    right();
                }
            }
        }
        forward();
       
    }
    return;
}

void stageOne()
{
    //define the outside/wall-touching cells as a circle. Randomly select one.
    int position = rand() % (gridSize * 4 - 3);
    if (position <= gridSize) {
        drawMarker(position, 1);
    } else if (position <= (2 * gridSize) - 1) {
        drawMarker(gridSize, (position - gridSize + 1));
    } else if (position <= (3 * gridSize) - 2) {
        int offset = (2 * gridSize) - position;
        drawMarker(offset + gridSize - 1, gridSize);
    } else {
        int offset = (3 * gridSize) - position;
        drawMarker(1, gridSize + offset - 2);
    }
    
    //Generate random robot start position
    bobby.x = rand() % gridSize + 1;
    bobby.y = rand() % gridSize + 1;
    bobby.direction = 1;
    drawRobot(bobby.x, bobby.y, bobby.direction);
    
    //keep moving forward until at wall or marker. if at wall, then turn right
    do {
        if (canMoveForward() == 1) {
            forward();
        } else {
            right();
        }
        
    } while (atMarker() == 0);
    
    return;
}

void stageTwo()
{
    int arenaSize = gridSize;
    //picks a random position along the edge of the arena.
    int position = rand() % (arenaSize * 4 - 3);
    if (position <= arenaSize) {
        drawMarker(position, 1);
    } else if (position <= (2 * arenaSize) - 1) {
        drawMarker(arenaSize, (position - arenaSize + 1));
    } else if (position <= (3 * arenaSize) - 2) {
        int offset = (2 * arenaSize) - position;
        drawMarker(offset + arenaSize - 1, arenaSize);
    } else {
        int offset = (3 * arenaSize) - position;
        drawMarker(1, arenaSize + offset - 2);
    }
    
    //random start position
    bobby.x = rand() % arenaSize + 1;
    bobby.y = rand() % arenaSize + 1;
    drawRobot(bobby.x, bobby.y, bobby.direction);
    
    //follow the wall until at marker
    do {
        wallFollow();
    } while (atMarker() == 0);
    pickUpMarker();
    
    //continue to follow the wall until at a corner
    do {
        wallFollow();
    } while (!((bobby.x == 1 && bobby.y == 1) || (bobby.x == arenaSize && bobby.y == 1) ||
               (bobby.x == 1 && bobby.y == arenaSize) || (bobby.x == arenaSize && bobby.y == arenaSize)));
    dropMarker();
    
    return;
}

void stageThree()
{
    int arenaSize = gridSize;
    //draw marker in random position
    int xPos = rand() % arenaSize + 1;
    int yPos = rand() % arenaSize + 1;
    drawMarker(xPos, yPos);
    
    //random start position
    bobby.x = rand() % arenaSize + 1;
    bobby.y = rand() % arenaSize + 1;
    drawRobot(bobby.x, bobby.y, bobby.direction);
    
    //use my bfs to find the route to the marker
    int pathLen = bfsSearch(bobby.x, bobby.y, arenaSize, 2);
    navigatePath(pathX, pathY, pathLen);
    pickUpMarker();
    
    //set the corners as drop locations and bfs to the nearest
    grid[1][1] = 3;
    grid[1][arenaSize] = 3;
    grid[arenaSize][1] = 3;
    grid[arenaSize][arenaSize] = 3;
    int newPLen = bfsSearch(bobby.x, bobby.y, arenaSize, 3);
    navigatePath(pathX, pathY, newPLen);
    dropMarker();
    return;
}

void stageFour(int argC, char **argV)
{
    //default values
    int noOfMarkers = 1;
    int noOfObstacles = 1;
    int arenaSize = gridSize;
    
    //if arguments given in console then use those as the values
    if (argC >= 5) {
        noOfMarkers = atoi(argV[3]);
        noOfObstacles = atoi(argV[4]);
    }
    //if the arguments were invalid then set defaults
    if (arenaSize < 2 || arenaSize > gridSize) {
        arenaSize = 10;
    }
    if (noOfMarkers + noOfObstacles > (arenaSize * arenaSize)) {
        noOfMarkers = 1;
        noOfObstacles = 2;
    }
    
    grid[1][1] = 3; //set drop location(Before obstacle generation to avoid blocking the corners)
    grid[1][arenaSize] = 3;
    grid[arenaSize][1] = 3;
    grid[arenaSize][arenaSize] = 3;
    
    //randomly generate markers and obstacles
    for (int i = 0; i < noOfObstacles; i++) {
        int xPos = rand() % arenaSize + 1;
        int yPos = rand() % arenaSize + 1;
        drawObstacle(xPos, yPos, 0);
    }
    for (int i = 0; i < noOfMarkers; i++) {
        int xPos = rand() % arenaSize + 1;
        int yPos = rand() % arenaSize + 1;
        drawMarker(xPos, yPos);
    }
    
    //random robot start position
    bobby.x = rand() % arenaSize + 1;
    bobby.y = rand() % arenaSize + 1;
    drawRobot(bobby.x, bobby.y, bobby.direction);
    
    //bfs to each marker
    do {
        int pathLen = bfsSearch(bobby.x, bobby.y, arenaSize, 2);
        if (pathLen == 0) {
            break;
        }
        navigatePath(pathX, pathY, pathLen);
        pickUpMarker();
        int newPLen = bfsSearch(bobby.x, bobby.y, arenaSize, 3);
        navigatePath(pathX, pathY, newPLen);
        dropMarker();
    } while (1);
    return;
}

void stageFive(int argC, char **argV)
{
    //default values
    int circleRadius = 5;
    int noOfMarkers = 1;
    int noOfObstacles = 1;
    int arenaSize = gridSize;
    
    //if arguments given in console then use those as the values
    if (argC>=6) {
        noOfMarkers = atoi(argV[3]);
        noOfObstacles = atoi(argV[4]);
        circleRadius = atoi(argV[5]);
    }
    if (arenaSize < 2 || arenaSize > gridSize) {
        arenaSize = 10;
    }
    if (noOfMarkers + noOfObstacles > (arenaSize * arenaSize)) {
        noOfMarkers = 1;
        noOfObstacles = 2;
    }
    
    //Draw the circle arena by checking if a square is further than circle radius and drawing obstacle if so
    for (int x = 1; x <= arenaSize; x++) {
        for (int y = 1; y <= arenaSize; y++) {
            int centerX = (arenaSize + 1) / 2;
            int centerY = (arenaSize + 1) / 2;
            int distSquared = (x - centerX) * (x - centerX) + (y - centerY) * (y - centerY);
            if (distSquared > circleRadius * circleRadius) {
                drawObstacle(x, y, 1);
            }
        }
    }
    
    //randomly generate markers and obstacles, checking to ensure they are within the circle and not overlapping
    int obstaclesPlaced = 0;
    while (obstaclesPlaced < noOfObstacles) {
        int xPos = rand() % arenaSize + 1;
        int yPos = rand() % arenaSize + 1;
        if (grid[xPos][yPos] == 0) {
            drawObstacle(xPos, yPos, 0);
            obstaclesPlaced++;
        }
    }
    int markersPlaced = 0;
    while (markersPlaced < noOfMarkers) {
        int xPos = rand() % arenaSize + 1;
        int yPos = rand() % arenaSize + 1;
        if (grid[xPos][yPos] == 0) {
            drawMarker(xPos, yPos);
            markersPlaced++;
        }
    }
    
    //random robot start position ensuring it isnt on an obstacle or marker
    int robotPlaced = 0;
    while (robotPlaced == 0) {
        int xPos = rand() % arenaSize + 1;
        int yPos = rand() % arenaSize + 1;
        if (grid[xPos][yPos] == 0) {
            bobby.x = xPos;
            bobby.y = yPos;
            robotPlaced = 1;
        }
    }
    drawRobot(bobby.x, bobby.y, bobby.direction);
    
    //use bfs to find and pick up each marker
    do {
        int pathLen = bfsSearch(bobby.x, bobby.y, arenaSize, 2);
        if (pathLen == 0) {
            break;
        }
        navigatePath(pathX, pathY, pathLen);
        pickUpMarker();
    } while (1);
    return;
}