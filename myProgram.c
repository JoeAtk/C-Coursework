#include <stdio.h>
#include "graphics.h"


int windowSize = 400;
int gridSize = 10;
void drawObstacle(int x, int y);
int main()
{   
    background();
    //int windowSize = 400;
    //int gridSize = 10; 
    setWindowSize(windowSize+10, windowSize+10);
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

    drawObstacle(5,5);




    return 0;
}

void drawObstacle(int x, int y)
{
    foreground();
    setColour(red);
    fillRect((x*gridSize)-5,(y*gridSize)-5,windowSize/gridSize,windowSize/gridSize);
    return;
}
