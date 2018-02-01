#include "Maze.h"
#include <iostream>
#include <string>


int main(){


Player Kate("Kate");
const int size = 5; 
Maze maze(size);
maze.NewGame(&Kate, 0);
while(!maze.IsGameOver()){
	maze.TakeTurn(&Kate);
}

std::cout << Kate.get_name() << " has " << Kate.get_lives() << " lives left and " 
		<< Kate.get_points() << " points." << std::endl;



return 0;
}