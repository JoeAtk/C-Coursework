#ifndef ARENA_H
#define ARENA_H

#define GRID_SIZE 65

extern int windowSize;
extern int gridSize;
extern int grid[GRID_SIZE+1][GRID_SIZE+1];

void initArena(int size);
void drawArenaGrid();
void drawMarker(int x, int y);
void drawObstacle(int x, int y, int darkOrLight);
void fillGrid(int x, int y);
void emptyCell(int x, int y);

#endif