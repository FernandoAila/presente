#ifndef PATHFINDING_H
#define PATHFINDING_H
#include <stdio.h>
#include "state.h"
#include <math.h>
#define REFRESH_AI 2 //Seconds between the path is calculated

void hub(state*sta,int start,int end,int tile_x,int tile_y);
//Initializes the pathfindig AI
void path_init(pathfinder *path,level lvl);
void pathfinder_find(pathfinder *path,int start,int end);
//Check the cell with the lowest score and pick it
int path_finder_lowest_in_open_set(pathfinder *path);
//Mark all Cells that are path as a path
void path_finder_reconstruct_path(pathfinder *path);
//Check if the set of cells discovered is empty
int path_finder_open_set_is_empty(pathfinder *path);
int path_finder_heuristic(pathfinder *path,int n);
void reset(pathfinder *path);






#endif
