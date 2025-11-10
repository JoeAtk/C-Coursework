#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graphics.h"
#include "arena.h"
#include "robot.h"
#include "controller.h"


int main(int argc, char **argv)
{
    srand((unsigned)time(NULL));
    //args: 1=arena size, 2=stage no., 3=no of markers, 4=no of obstacles, 5=circle radius(for stage 5),6 = time to sleep, 7 = window size (if given)
    if(argc >=8){
        int winSize = atoi(argv[7]);
        if (winSize < 200){
            winSize = 200;
        }
        if (winSize > 1200){
            winSize = 1200;
        }
        windowSize = winSize;
    }
    if (argc>=7){
        int sTime = atoi(argv[6]);
        if (sTime < 0){
            sTime = 0;
        }
        if (sTime > 1000){
            sTime = 1000;
        }
        sleepTime = sTime;
    }
    int size = 6;
    if (argc >= 2) {
        size = atoi(argv[1]);
    }
    int stage =5;
    //default to stage 5
    if (argc >= 3) {
        stage = atoi(argv[2]);
    }
    //for random grid/arena size uncomment below
    //size = rand() % (11) +1;
    initArena(size);
    drawArenaGrid();
    if(stage ==1){
        stageOne();
    }
    else if(stage ==2){
        stageTwo();
    }
    else if(stage ==3){
        stageThree();
    }
    else if(stage ==4){
        stageFour(argc, argv);
    }
    else if(stage ==5){
        stageFive(argc, argv);
    }
    return 0;
}