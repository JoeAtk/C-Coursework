#include <stdio.h>
#include "graphics.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define M_PI 3.14159265358979323846
#define GRID_SIZE 10
int windowSize = 400;
int gridSize = GRID_SIZE;
int grid[GRID_SIZE+1][GRID_SIZE+1]; //0=empty,1=obstacle,2=marker
int searchGrid[GRID_SIZE+1][GRID_SIZE+1]; //0=unvisited,1=visited
int robotDirection=1;
int robotX;
int robotY;
int carryingMarkerCount=0;
void drawObstacle(int x, int y);
void fillGrid(int x, int y);
void drawMarker(int x, int y);
void stageOne();
void stageTwo();
void stageThree();
void drawRobot(int x, int y,int rotation);
int canMoveForward();
void forward();
void right(); 
void left();
int atMarker();
void pickUpMarker();
int search(int x, int y,int arenaSize);

int main()
{   
    srand((unsigned)time(NULL));
    setWindowSize(windowSize+10, windowSize+10);
    background();
    setColour(red);
    drawLine(5,5,windowSize+5,5);
    drawLine(5,5,5,windowSize+5);
    drawLine(windowSize+5,5,windowSize+5,windowSize+5);
    drawLine(5,windowSize+5,windowSize+5,windowSize+5);
    setColour(black);
    for(int i=1; i<gridSize;i++){
        int Pos = i*(windowSize/gridSize) + 5;
        drawLine(Pos,5,Pos,windowSize+5);
        drawLine(5,Pos,windowSize+5, Pos);
    }
    //stageOne();
    //stageTwo();
    stageThree();
    return 0;
}

void stageOne()
{
    //define the outside/wall-touching cells as a circle. Randomly select one. 
    int position = rand() % (gridSize * 4 - 3);
    if (position <= gridSize){
        drawMarker(position,1);
    }else if(position <= (2*gridSize)-1){
        drawMarker(gridSize,(position-gridSize+1));
    }else if(position <= (3* gridSize)-2){
        int offset = (2*gridSize)-position;
        drawMarker(offset+gridSize-1,gridSize);
    }else{
          int offset = (3*gridSize)-position;
        drawMarker(1,gridSize+offset-2);
    }

    robotX = rand() % gridSize + 1;
    robotY = rand() % gridSize + 1;
    drawRobot(robotX,robotY,robotDirection);
    int counter=gridSize*gridSize+50;
    do{
        if(canMoveForward()==1){
            forward();  
        }else
        {
            right();
        }
        sleep(500);
    }while (atMarker() ==0);
    
    return;
}

void stageTwo()
{
    int arenaSize = rand() % (gridSize-2) + 2;
    int layerRemoveCount = gridSize-arenaSize;
    for(int i =0; i<layerRemoveCount;i++){
        for(int j =1 ;j<=gridSize;j++){
            drawObstacle(gridSize - i,j);
            drawObstacle(j,gridSize - i);
        }
    }
    int position = rand() % (arenaSize * 4 - 3);
    if (position <= arenaSize){
        drawMarker(position,1);
    }else if(position <= (2*arenaSize)-1){
        drawMarker(arenaSize,(position-arenaSize+1));
    }else if(position <= (3* arenaSize)-2){
        int offset = (2*arenaSize)-position;
        drawMarker(offset+arenaSize-1,arenaSize);
    }else{
          int offset = (3*arenaSize)-position;
        drawMarker(1,arenaSize+offset-2);
    }
    
    robotX = rand() % arenaSize + 1;
    robotY = rand() % arenaSize + 1;
    drawRobot(robotX,robotY,robotDirection);
    int counter=arenaSize*arenaSize+50;
    do{
        if(canMoveForward()==1){
            forward();  
        }else
        {
            right();
        }
        sleep(500);
    }while (atMarker() ==0);
    

    return;
}

void stageThree()
{
    int arenaSize = rand() % (gridSize-2) + 2;
    int layerRemoveCount = gridSize-arenaSize;
    for(int i =0; i<layerRemoveCount;i++){
        for(int j =1 ;j<=gridSize;j++){
            drawObstacle(gridSize - i,j);
            drawObstacle(j,gridSize - i);
        }
    }
    int xPos = rand() % arenaSize + 1;
    int yPos = rand() % arenaSize + 1;
    drawMarker(xPos,yPos);

    robotX = rand() % arenaSize + 1;
    robotY = rand() % arenaSize + 1;
    drawRobot(robotX,robotY,robotDirection);
    search(robotX,robotY,arenaSize);
    
    do{
        if(searchGrid[robotX+1][robotY]==0){
           if(robotDirection!=2){
               while(robotDirection!=2){
                   right();
               }
           }
           forward();
        }else if(searchGrid[robotX-1][robotY]==0){
           if(robotDirection!=4){
               while(robotDirection!=4){
                   right();
               }
           }
           forward();
        }else if(searchGrid[robotX][robotY+1]==0){
           if(robotDirection!=3){
               while(robotDirection!=3){
                   right();
               }
           }
           forward();
        }else if(searchGrid[robotX][robotY-1]==0){
              if(robotDirection!=1){
                while(robotDirection!=1){
                     right();
                }
              }
              forward();
        }else{}
        sleep(500);
    }while(atMarker() ==0);
    
    return;
}

int atMarker()
{
    if (grid[robotX][robotY] == 2) {
        return 1;
    }
    return 0;
}

void drawRobot(int x, int y,int rotation)
{
    //rotation defines the way the robot is facing. 1=up,2=right,3=down,4=left
    background();
    foreground();
    //setColour(darkgray);
    clear();
    int xPositions[3];
    int yPositions[3];
    int gridPixelSize = windowSize/gridSize;
    int robotSize = (gridPixelSize)/2 - 2;
    int horiOffset = sin(M_PI/3)*robotSize;
    int vertOffset = cos(M_PI/3)*robotSize;
    int baseX = 5+(x*gridPixelSize)-(gridPixelSize/2);
    int baseY = 5+(y*gridPixelSize)-(gridPixelSize/2);
    if(rotation ==1){
        int upBaseX=baseX+((robotSize-horiOffset)/2);
        int upBaseY=baseY+((robotSize-vertOffset)/2);
        xPositions[0]= upBaseX;
        yPositions[0]= upBaseY-robotSize;
        xPositions[1]=upBaseX + horiOffset;
        yPositions[1]=upBaseY + vertOffset;
        xPositions[2]=upBaseX - horiOffset;
        yPositions[2]=upBaseY + vertOffset;
    }else if (rotation==2){
        int rightBaseX=baseX-((robotSize-vertOffset)/2);
        int rightBaseY=baseY;
        xPositions[0]= rightBaseX+robotSize;
        yPositions[0]= rightBaseY;
        xPositions[1]=rightBaseX - vertOffset;
        yPositions[1]=rightBaseY + horiOffset;
        xPositions[2]=rightBaseX - vertOffset;
        yPositions[2]=rightBaseY - horiOffset;
    }else if (rotation ==3){
        int downBaseX=baseX;
        int downBaseY=baseY-((robotSize - vertOffset)/2);
        xPositions[0]= downBaseX;
        yPositions[0]= downBaseY+robotSize;
        xPositions[1]=downBaseX + horiOffset;
        yPositions[1]=downBaseY - vertOffset;
        xPositions[2]=downBaseX - horiOffset;
        yPositions[2]=downBaseY - vertOffset;
    }else{
        int leftBaseX=baseX+((robotSize - vertOffset)/2);
        int leftBaseY=baseY;
        xPositions[0]= leftBaseX - robotSize;
        yPositions[0]= leftBaseY;
        xPositions[1]=leftBaseX + vertOffset;
        yPositions[1]=leftBaseY + horiOffset;
        xPositions[2]=leftBaseX + vertOffset;
        yPositions[2]=leftBaseY - horiOffset;
    }
    fillPolygon(3,xPositions,yPositions);    
    return;
}

int canMoveForward()
{
    if(robotDirection ==1){
        if(robotY>1 && grid[robotX][robotY-1]!=1){
            return 1;
        }
    }else if(robotDirection ==2){
        if(robotX<gridSize && grid[robotX+1][robotY]!=1){
            return 1;
        }
    }else if(robotDirection ==3){
        if(robotY<gridSize && grid[robotX][robotY+1]!=1){
            return 1;
        }
    }else if(robotDirection ==4){
        if(robotX>1 && grid[robotX-1][robotY]!=1){
            return 1;
        }
    }
    return 0;
}

void forward()
{
    if(canMoveForward()!=1){
        return;
    }
    //move robot position
    if(robotDirection ==1){
        robotY--;
    }else if(robotDirection ==2){
        robotX++;
    }else if(robotDirection ==3){
        robotY++;
    }else if(robotDirection ==4){
        robotX--;
    }
    drawRobot(robotX,robotY,robotDirection);
    return;
}

void right(){
    robotDirection++;
    if(robotDirection>4){
        robotDirection=1;
    }  
    drawRobot(robotX,robotY,robotDirection);   
    return;
}
void left(){
    robotDirection--;
    if(robotDirection<1){
        robotDirection=4;
    }
    drawRobot(robotX,robotY,robotDirection);
    return;
}

void drawMarker(int x, int y)
{
    background();
    grid[x][y]=2;
    setColour(blue);
    fillGrid(x,y);
    return;
}

void drawObstacle(int x, int y)
{
    background();
    grid[x][y]=1;
    setColour(red);
    fillGrid(x,y);
    return;
}

void fillGrid(int x, int y)
{
    int cellWidth= windowSize/gridSize;
    fillRect(((x-1)*cellWidth)+7,((y-1)*cellWidth)+7,(cellWidth-3),(cellWidth-3));
    return;
}

int search(int x, int y,int arenaSize) 
{
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