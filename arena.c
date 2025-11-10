#include "arena.h"
#include "graphics.h"

int windowSize = 700;
int gridSize = GRID_SIZE;
int grid[GRID_SIZE+1][GRID_SIZE+1] = {0}; //0=empty,1=obstacle,2=marker, 3=dropLocation

void initArena(int size)
{
    gridSize = size;
    if (gridSize < 2){
        gridSize = 6;
    }
    if(gridSize > GRID_SIZE){
        gridSize = GRID_SIZE;
    }
    windowSize = (windowSize/gridSize)*gridSize;
}

void drawArenaGrid()
{
    //make sure windowsize is multiple of grid size
    setWindowSize(windowSize + 10, windowSize + 10);
    background();
    setColour(red);
    drawLine(5, 5, windowSize + 5, 5);
    drawLine(5, 5, 5, windowSize + 5);
    drawLine(windowSize + 5, 5, windowSize + 5, windowSize + 5);
    drawLine(5, windowSize + 5, windowSize + 5, windowSize + 5);
    setColour(black);
    
    for (int i = 1; i < gridSize; i++) {
        int pos = i * (windowSize / gridSize) + 5;
        drawLine(pos, 5, pos, windowSize + 5);
        drawLine(5, pos, windowSize + 5, pos);
    }
}

void drawMarker(int x, int y)
{
    //if not an obstacle draw marker and set grid value
    if (grid[x][y] == 1) {
        return;
    }
    background();
    if (grid[x][y] != 3) {
        grid[x][y] = 2;
    }
    setColour(blue);
    fillGrid(x, y);
}

void drawObstacle(int x, int y, int darkOrLight)
{
    //draw marker and set grid value
    background();
    grid[x][y] = 1;
    if (darkOrLight == 1) {
        setRGBColour(100, 0, 0);
    } else {
        setColour(red);
    }
    fillGrid(x, y);
}

void fillGrid(int x, int y)
{
    // generic grid cell filler
    int cellWidth = windowSize / gridSize;
    fillRect(((x - 1) * cellWidth) + 7, ((y - 1) * cellWidth) + 7, 
             (cellWidth - 3), (cellWidth - 3));
}

void emptyCell(int x, int y)
{
    // generic grid cell emptier
    background();
    setColour(white);
    fillGrid(x, y);
}