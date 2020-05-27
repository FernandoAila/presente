#include "pathfinding.h"

void hub(state *sta,int start,int end,int tile_x,int tile_y){
	if (sta->pt.calls==0)
	{
		sta->pt.calls =60 * REFRESH_AI;
		sta->pt.start=start;
		sta->pt.end=end;
		pathfinder_find(&sta->pt,start,end);
	}
	sta->pt.calls -= 1;
}

void path_init(pathfinder *path,level lvl){
	for (int  i = 0; i < MAX_CELLS; i++)
	{
		path->g_score[i]=0;
		path->f_score[i]=0;
		path->parent[i]=0;
		path->state[i]=1;
		path->is_wall[i]=0;
	}
	path->calls=0;
	path->rows=lvl.size_x;
	path->cols=lvl.size_y;
	path->start=0;
	path->end=0;
	//Get all walls of level
	for (int x = 0; x <path->rows; x++)
	{
		for (int y = 0; y < path->cols; y++)
		{
			if(level_get(&lvl,x,y)==35){
				path->is_wall[x*path->cols+y]=1;
			}
		}

	}
	
}
void pathfinder_find(pathfinder *path,int start, int end){
	path->state[start]=path->state[start]|2;
	int run;
	int current;
	int count;
	run = 1;
	while (run == 1) {
		current = 0;
		count = path->cols*path->rows;
		current = path_finder_lowest_in_open_set(path);
		if (current == path->end) {
			path_finder_reconstruct_path(path);
			run = 0;
		}
		else if (path_finder_open_set_is_empty(path) == 1) {
			run = 0;
		}
		else {
			int neighbors[4]; //A cell in the matrix will always have 4 or less neighbors

			int tmp_g_score;
			//Mark current cell as visited and not reachable
			path->state[current] = path->state[current] & ~2;
			path->state[current] = path->state[current] | 4;
			//Check if exists a cell in the left of the current cell
			if (current % path->cols == 0){
				neighbors[0] = -1;
			} 
			else {
				neighbors[0] = current - 1;
				}
		// Get value of the cell above the current one.
			neighbors[1] = current - path->cols;
			/* Check if exists a cell in the right of the current cell */
			if ((current + 1) % path->cols == 0) {
				neighbors[2] = -1;
			} 
			else {
				neighbors[2] = current + 1;
				}
			/* Bottom */
			neighbors[3] = current + path->cols;
			//  For each neighbors of current cell, /
			tmp_g_score = 0;
			for(int j = 0; j < 4;j++){
				int n;
				n = neighbors[j];
				if (n > -1 && n < count && (path->state[n] & 4) == 0) {
					if (path->is_wall[n] == 1) {
						path->state[n] = path->state[n] | 4;
					} 
					else {
						//Distances between neighbor cells are 1
						tmp_g_score = path->g_score[current] + 1;
						//If not discovered OR recent Local score is less than old local score 
						if ((path->state[n] & 2) == 0|| tmp_g_score < path->g_score[n]) {
							//Current cell will be parent of the neighbor
							path->parent[n] = current;
							//Asign recent calculated local score to the neighbor cell
							path->g_score[n] = tmp_g_score;
							//Calculte GLOBAL score (Local score - heuristic)
							path->f_score[n] = tmp_g_score + path_finder_heuristic(path,n);
							path->state[n] = path->state[n] | 2;
						}
					}
				}
			}
		}
	}
	
}
int path_finder_lowest_in_open_set(pathfinder *path){
	int lowest_f;
	int current_lowest;
	int count;
	count = path->rows*path->cols;
	lowest_f = count;
	current_lowest = 0;
	for (int i =0;i < count;i++) {
		//check if node is discovered
		if ((path->state[i] & 2) ==2) {
			
			if (path->f_score[i] < lowest_f) {
				lowest_f = path->f_score[i];
				
				current_lowest = i;
			}
		}
	}
	return current_lowest;

}
void path_finder_reconstruct_path(pathfinder *path){
	int i;
	i = path->end;
	while (i != path->start) {
		if (path->parent[i] != path->start) {
			//Mark parent node as path
			path->state[path->parent[i]] = path->state[path->parent[i]]  | 8;
		}
		i = path->parent[i];
	}
}
int path_finder_open_set_is_empty(pathfinder *path){
	int empty;
	int i;
	empty = 1;
	i = 0;
	while (i < path->cols * path->rows && empty == 1) {
		//Check if exists a node that is discovered
		if ((path->state[i] & 2) == 2) {
			empty = 0;
		}
		i = i + 1;
	}
	return empty;
}
int path_finder_heuristic(pathfinder *path ,int cell){
	int cell_y;
	int cell_x;
	int end_y;
	int end_x;
	int dx;
	int dy;
	cell_y = cell / path->cols;
	cell_x = cell - (cell_y * path->cols);
	end_y = path->end / path->cols;
	end_x = path->end - (end_y * path->cols);
	//We get the distance of current cell and the end cell  
	if (cell_x > end_x) {
		dx = cell_x - end_x;
	} 
	else {
		dx = end_x - cell_x;
	}
	if (cell_y > end_y) {
		dy = cell_y - end_y;
	} 
	else {
		dy = end_y - cell_y;
	}
	return dx + dy;
}
void reset(pathfinder *path){
	for (int  i = 0; i < MAX_CELLS; i++)
	{
    	path->f_score[i]=0;
    	path->g_score[i]=0;
    	path->parent[i]=0;
    	path->state[i]=1;
	}
	path->start=0;
	path->end=0;
}