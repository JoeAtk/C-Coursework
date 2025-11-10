#ifndef CONTROLLER_H
#define CONTROLLER_H

int search(int x, int y, int arenaSize);
void wallFollow();
int bfsSearch(int x, int y, int arenaSize, int mode);
void navigatePath(int pathX[], int pathY[], int pathLength);

void stageOne();
void stageTwo();
void stageThree();
void stageFour(int argC, char **argV);
void stageFive(int argC, char **argV);

#endif