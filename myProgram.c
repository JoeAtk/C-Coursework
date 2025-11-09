#include <stdio.h>
#include "graphics.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define M_PI 3.14159265358979323846
#define GRID_SIZE 150//max grid size before logic stops working, set to desired max.
#define array_SIZE GRID_SIZE*GRID_SIZE*4
int windowSize = 400;
int gridSize = GRID_SIZE;
int grid[GRID_SIZE+1][GRID_SIZE+1]; //0=empty,1=obstacle,2=marker, 3=dropLocation
int searchGrid[GRID_SIZE+1][GRID_SIZE+1]; //0=unvisited,1=visited
int carryingMarkerCount=0;
int pathX[GRID_SIZE*GRID_SIZE];
int pathY[GRID_SIZE*GRID_SIZE];
void drawObstacle(int x, int y,int darkOrLight);
void navigatePath(int pathX[],int pathY[],int pathLen);
void fillGrid(int x, int y);
void drawMarker(int x, int y);
void wallFollow();
void stageOne();
void stageTwo();
void stageThree();
void stageFour(int argC, char **argV);
void stageFive(int argC, char **argV);
void emptyCell(int x, int y);
void drawRobot(int x, int y,int rotation);
int canMoveForward();
void forward();
void right(); 
void left();
int atMarker();
void pickUpMarker();
void dropMarker();
int search(int x, int y,int arenaSize);
int bfsSearch(int x,int y,int arenaSize,int mode);

int main(int argc, char **argv)
{   
    srand((unsigned)time(NULL));
    gridSize=6;
    if(argc>=2){
        gridSize = atoi(argv[1]);
    }
    //for random grid/arena size uncomment below
    //gridSize = rand() % (11) +1;

    windowSize = (windowSize/gridSize)*gridSize;
    //make sure windowsize is multiple of grid size
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
    //stageThree();
    //stageFour(argc,argv);
    stageFive(argc,argv);
    return 0;
}

struct robot{
    int robotX;
    int robotY;
    int robotDirection; //1=up,2=right,3=down,4=left
};

struct robot bobby;

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

    //Generate random robot start position
    bobby.robotX = rand() % gridSize + 1;
    bobby.robotY = rand() % gridSize + 1;
    bobby.robotDirection = 1;
    drawRobot(bobby.robotX,bobby.robotY,bobby.robotDirection);
    //keep moving forward until at wall or marker. if at wall, then turn right
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
    int arenaSize = gridSize;
    //picks a random position along the edge of the arena.
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
    //random start position
    bobby.robotX = rand() % arenaSize + 1;
    bobby.robotY = rand() % arenaSize + 1;
    drawRobot(bobby.robotX,bobby.robotY,bobby.robotDirection);
    int counter=arenaSize*arenaSize+50;
    //follow the wall until at marker
    do{
        wallFollow();
    }while (atMarker() ==0);
    pickUpMarker();
    //continue to follow the wall until at a corner
    do{
       wallFollow();
     }while (!((bobby.robotX==1 && bobby.robotY==1)||(bobby.robotX==arenaSize && bobby.robotY==1)||(bobby.robotX==1 && bobby.robotY==arenaSize)||(bobby.robotX==arenaSize && bobby.robotY==arenaSize)));
    dropMarker();

    return;
}

void wallFollow(){
    if(canMoveForward()==1){
        forward();  
    }else
    {
        right();
    }
    sleep(500);
    return;
}

void stageThree()
{
    int arenaSize = gridSize;
    //draw marker in random position
    int xPos = rand() % arenaSize + 1;
    int yPos = rand() % arenaSize + 1;
    drawMarker(xPos,yPos);
    //random start position
    bobby.robotX = rand() % arenaSize + 1;
    bobby.robotY = rand() % arenaSize + 1;
    drawRobot(bobby.robotX,bobby.robotY,bobby.robotDirection);
    //use my bfs to find the route to the marker
    int pathLen = bfsSearch(bobby.robotX,bobby.robotY,arenaSize,2);
    navigatePath(pathX,pathY,pathLen);
    pickUpMarker();
    //set the corners as drop locations and bfs to the nearest
    grid[1][1]=3;
    grid[1][arenaSize]=3;
    grid[arenaSize][1]=3;
    grid[arenaSize][arenaSize]=3;
    int newPLen = bfsSearch(bobby.robotX,bobby.robotY,arenaSize,3);
    navigatePath(pathX,pathY,newPLen);
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
    if(argC >=4){
        noOfMarkers = atoi(argV[2]);
        noOfObstacles = atoi(argV[3]);
    }
    //if the arguments were invalid then set defaults
    if (arenaSize<2 || arenaSize>gridSize){
        arenaSize = 10;
    }
    if(noOfMarkers+noOfObstacles > (arenaSize*arenaSize)){
        noOfMarkers = 1;
        noOfObstacles = 2;
    }
    
    grid[1][1]=3; //set drop location(Before obstacle generation to avoid blocking the corners)
    grid[1][arenaSize]=3;
    grid[arenaSize][1]=3;
    grid[arenaSize][arenaSize]=3;
    //randomly generate markers and obstacles
    for(int i=0;i<noOfObstacles;i++){
        int xPos = rand() % arenaSize + 1;
        int yPos = rand() % arenaSize + 1;
        drawObstacle(xPos,yPos,0);
    }
    for(int i=0;i<noOfMarkers;i++){
        int xPos = rand() % arenaSize + 1;
        int yPos = rand() % arenaSize + 1;
        drawMarker(xPos,yPos);
    }
    //random robot start position
    bobby.robotX = rand() % arenaSize + 1;
    bobby.robotY = rand() % arenaSize + 1;
    drawRobot(bobby.robotX,bobby.robotY,bobby.robotDirection);
    //bfs to each marker
    do{
        int pathLen = bfsSearch(bobby.robotX,bobby.robotY,arenaSize,2);
        if(pathLen ==0){
            break;
        }        
        navigatePath(pathX,pathY,pathLen);
        pickUpMarker();
        int newPLen = bfsSearch(bobby.robotX,bobby.robotY,arenaSize,3);
        navigatePath(pathX,pathY,newPLen);
        dropMarker();
    }while (1);
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
    if(argC ==5){
        noOfMarkers = atoi(argV[2]);
        noOfObstacles = atoi(argV[3]);
        circleRadius = atoi(argV[4]);
    }
    if (arenaSize<2 || arenaSize>gridSize){
        arenaSize = 10;
    }
    if(noOfMarkers+noOfObstacles > (arenaSize*arenaSize)){
        noOfMarkers = 1;
        noOfObstacles = 2;
    }
    //Draw the circle arena by checking if a square is further than circle radius and drawing obstacle if so
    for(int x = 1; x <= arenaSize; x++) {
        for(int y = 1; y <= arenaSize; y++) {
            int centerX = (arenaSize + 1) / 2;
            int centerY = (arenaSize + 1) / 2;
            int distSquared = (x - centerX) * (x - centerX) + (y - centerY) * (y - centerY);
            if (distSquared > circleRadius * circleRadius) {
                drawObstacle(x, y, 1);
            }
        }
    }
    //randomly generate markers and obstacles, checking to ensure they are within the circle and not overlapping
    int obstaclesPlaced =0;
    while(obstaclesPlaced < noOfObstacles)
    {
        int xPos = rand() % arenaSize + 1;
        int yPos = rand() % arenaSize + 1;
        if(grid[xPos][yPos]==0){
            drawObstacle(xPos,yPos,0);
            obstaclesPlaced++;
        }
    }
    int markersPlaced =0;
    while(markersPlaced < noOfMarkers)
    {
        int xPos = rand() % arenaSize + 1;
        int yPos = rand() % arenaSize + 1;
        if(grid[xPos][yPos]==0){
            drawMarker(xPos,yPos);
            markersPlaced++;
        }
    }
    //random robot start position ensuring it isnt on an obstacle or marker
    int robotPlaced=0;
    while(robotPlaced==0)
    {
        int xPos = rand() % arenaSize + 1;
        int yPos = rand() % arenaSize + 1;
        if(grid[xPos][yPos]==0){
            bobby.robotX = xPos;
            bobby.robotY = yPos;
            robotPlaced=1;
        }
    }
    drawRobot(bobby.robotX,bobby.robotY,bobby.robotDirection);
    //use bfs to find and pick up each marker
    do{
        int pathLen = bfsSearch(bobby.robotX,bobby.robotY,arenaSize,2);
        if(pathLen ==0){
            break;
        }
        navigatePath(pathX,pathY,pathLen);
        pickUpMarker();
    }while (1);
    return;
}


int atMarker()
{
    if (grid[bobby.robotX][bobby.robotY] == 2) {
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
    //create array for the x/y coords of the triangle corners
    int xPositions[3];
    int yPositions[3];
    int gridPixelSize = windowSize/gridSize;
    int robotSize = (gridPixelSize)/2 - 2;
    int horiOffset = sin(M_PI/3)*robotSize;
    int vertOffset = cos(M_PI/3)*robotSize;
    int baseX = 5+(x*gridPixelSize)-(gridPixelSize/2);
    int baseY = 5+(y*gridPixelSize)-(gridPixelSize/2);
    //use trig to calculate the corner positions based on rotation
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
    //set colour and draw the triangle
    setColour(darkgray);
    fillPolygon(3,xPositions,yPositions);   
    sleep(100);
    //if it is carrying a marker display a small blue indicator
    if(carryingMarkerCount>0){
        setColour(blue);
        fillOval(baseX,baseY,gridPixelSize/4,gridPixelSize/4);
    }
    return;
}

int canMoveForward()
{
    //check if the "forward" cell can be moved into
    if(bobby.robotDirection ==1){
        if(bobby.robotY>1 && grid[bobby.robotX][bobby.robotY-1]!=1){
            return 1;
        }
    }else if(bobby.robotDirection ==2){
        if(bobby.robotX<gridSize && grid[bobby.robotX+1][bobby.robotY]!=1){
            return 1;
        }
    }else if(bobby.robotDirection ==3){
        if(bobby.robotY<gridSize && grid[bobby.robotX][bobby.robotY+1]!=1){
            return 1;
        }
    }else if(bobby.robotDirection ==4){
        if(bobby.robotX>1 && grid[bobby.robotX-1][bobby.robotY]!=1){
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
    if(bobby.robotDirection ==1){
        bobby.robotY--;
    }else if(bobby.robotDirection ==2){
        bobby.robotX++;
    }else if(bobby.robotDirection ==3){
        bobby.robotY++;
    }else if(bobby.robotDirection ==4){
        bobby.robotX--;
    }
    drawRobot(bobby.robotX,bobby.robotY,bobby.robotDirection);
    return;
}

void right(){
    //increase direction value and wrap if needed
    bobby.robotDirection++;
    if(bobby.robotDirection>4){
        bobby.robotDirection=1;
    }  
    drawRobot(bobby.robotX,bobby.robotY,bobby.robotDirection);   
    return;
}
void left(){
    //decrease direction value and wrap if needed
    bobby.robotDirection--;
    if(bobby.robotDirection<1){
        bobby.robotDirection=4;
    }
    drawRobot(bobby.robotX,bobby.robotY,bobby.robotDirection);
    return;
}

void drawMarker(int x, int y)
{
    //if not an obstacle draw marker and set grid value
    if(grid[x][y]==1){
        return;
    }
    background();
    if(grid[x][y]!=3){
        grid[x][y]=2;
    }
    setColour(blue);
    fillGrid(x,y);
    return;
}

void drawObstacle(int x, int y,int darkOrLight)
{
    //draw marker and set grid value
    background();
    grid[x][y]=1;
    if(darkOrLight==1){
        setRGBColour(100,0,0);
    }else{
        setColour(red);
    }
    fillGrid(x,y);
    return;
}

void pickUpMarker()
{
    // check if in same cell as marker and pick it up
    if(grid[bobby.robotX][bobby.robotY]==2){
        grid[bobby.robotX][bobby.robotY]=0;
        emptyCell(bobby.robotX,bobby.robotY);
        carryingMarkerCount++;
    }
    return;
}

void dropMarker()
{
    //drop marker if carrying one. dont set the cell to marker as it causes issues with pathfinding and the solution doest neet it to be picket up again
    if(carryingMarkerCount>0){
        drawMarker(bobby.robotX,bobby.robotY);
        carryingMarkerCount--;
    }
    drawRobot(bobby.robotX,bobby.robotY,bobby.robotDirection);
    return;
}

void fillGrid(int x, int y)
{
    // generic grid cell filler
    int cellWidth= windowSize/gridSize;
    fillRect(((x-1)*cellWidth)+7,((y-1)*cellWidth)+7,(cellWidth-3),(cellWidth-3));
    return;
}
void emptyCell(int x, int y)
{
    // generic grid cell emptier
    background();
    setColour(white);
    fillGrid(x,y);
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
int bfsSearch(int x,int y,int arenaSize,int mode)
{
    //mode = either navigating to a marker or a drop location
    //initialise empty array for the queue and parent tracking
    arenaSize=gridSize;
    int solvedLength =10000;
    int queueX[array_SIZE];
    int queueY[array_SIZE];
    int front = 0;
    int rear = 0;
    int parentX[GRID_SIZE+1][GRID_SIZE+1];
    int parentY[GRID_SIZE+1][GRID_SIZE+1];
    //-1 = unvisited,-2= root
    //set every node to unvisited
    for(int i=1;i<=arenaSize;i++){
        for(int j=1;j<=arenaSize;j++){
            parentX[i][j]=-1;
            parentY[i][j]=-1;
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
        if(currY>1 && grid[currX][currY-1]!=1 && parentX[currX][currY-1]==-1){
            //if not top row, not an obstacle and unvisited
            queueX[rear]=currX;
            queueY[rear]=currY-1;
            rear++;
            parentX[currX][currY-1]=currX;
            parentY[currX][currY-1]=currY;
            //add to queue and set parent to current cell.
        }
        if(currY<arenaSize && grid[currX][currY+1]!=1 && parentX[currX][currY+1]==-1){
            //if not bottom row, not an obstacle and unvisited
            queueX[rear]=currX;
            queueY[rear]=currY+1;
            rear++;
            parentX[currX][currY+1]=currX;
            parentY[currX][currY+1]=currY;
            //add to queue and set parent to current cell.
        }
        if(currX>1 && grid[currX-1][currY]!=1 && parentX[currX-1][currY]==-1){
            //if not left column, not an obstacle and unvisited
            queueX[rear]=currX-1;
            queueY[rear]=currY;
            rear++;
            parentX[currX-1][currY]=currX;
            parentY[currX-1][currY]=currY;
            //add to queue and set parent to current cell.
        }
        if (currX<arenaSize && grid[currX+1][currY]!=1 && parentX[currX+1][currY]==-1){
            //if not right column, not an obstacle and unvisited
            queueX[rear]=currX+1;
            queueY[rear]=currY;
            rear++;
            parentX[currX+1][currY]=currX;
            parentY[currX+1][currY]=currY;
            //add to queue and set parent to current cell.
        }
        //every unvisited cell has now been added to the queue.

       
    }

    //walk back the path now if the marker was found
    if(found==1){
        int pathLength=0;
        int currentX=markerX;
        int currentY=markerY;
  
        while(currentX!=-1 && currentX!=-2){
            //loop until the root is reached.
            pathX[pathLength]=currentX;
            pathY[pathLength]=currentY;
            pathLength++;
            if(parentX[currentX][currentY]==-2 && parentY[currentX][currentY]==-2){
                //if the parent is the root, exit the loop
                break;
            }
            //cX as otherwise the currentX would be overwritten before getting to currentY
            int cX = currentX;
            //set current to parent for next loop
            currentX=parentX[currentX][currentY];
            currentY=parentY[cX][currentY];
        }
        //reverse the path array.
        int j=0;
        int tempArrayX[GRID_SIZE*GRID_SIZE];
        int tempArrayY[GRID_SIZE*GRID_SIZE];
        for(int i=0;i<pathLength;i++){
            tempArrayX[i]=pathX[i];
            tempArrayY[i]=pathY[i];
        }
        for(int i=pathLength-1;i>=0;i--){
            pathX[j]=tempArrayX[i];
            pathY[j]=tempArrayY[i];
            j++;
        }
        //return distance to the nearest objective (marker/drop location)
        return pathLength;
    }
    //return 0 if no path found/possible
    return 0;
}

void navigatePath(int pathX[], int pathY[], int pathLength){
    for(int i=0;i<pathLength;i++){
       //find the direction relative to robot direction
        if(pathX[i]==bobby.robotX && pathY[i]==bobby.robotY){
            //same cell do nothing
            continue;
        }
        if(pathY[i]<bobby.robotY){
            //target cell is above
            if(bobby.robotDirection==2){
                left();
            }else{
                while(bobby.robotDirection!=1){
                    sleep(300);
                    right();
                }
            }
        }else if(pathY[i]>bobby.robotY){
            if(bobby.robotDirection==4){
                left();
            }else{
                while(bobby.robotDirection!=3){
                    sleep(300);
                    right();
                }
            }
        }else if(pathX[i]>bobby.robotX){
            if(bobby.robotDirection==3){
                left();
            }else{
                while(bobby.robotDirection!=2){
                    sleep(300);
                    right();
                }
            }
        }else if(pathX[i]<bobby.robotX){
            if(bobby.robotDirection==1){
                left();
            }else{
                while(bobby.robotDirection!=4){
                    sleep(300);
                    right();
                }
            }
        }
        forward();
        sleep(300);
    }
    
    return;
}