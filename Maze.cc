#include "Maze.h"
#include <vector>
#include <string>
#include <stdlib.h>
#include <ctime>

/*
* Returns the unicode emoji icons corresponding to the SquareType
* @param sq is the SquareType of the square we want the unicode emoji for
* @return the unicode as a string
*/
std::string SquareTypeStringify(SquareType sq){
	switch(sq) {
		case SquareType::Wall: return " \xE2\x9D\x8C ";
		case SquareType::Exit: return " \xF0\x9F\x9A\xA9 ";
		case SquareType::Empty: return " \xE2\xAC\x9C ";
		case SquareType::Human: return " \xE2\x9B\x84 ";
		case SquareType::Treasure: return " \xE2\xAD\x90 ";
		case SquareType::Health: return " \xF0\x9F\x92\x96 ";
		case SquareType::Hammer: return " \xF0\x9F\x94\xA8 ";
		case SquareType::Trap: return " \xF0\x9F\x94\xA5 ";

		default: return "";
	}

 }

 
 /*
 * Converts the players choice to a Direction, handles invalid input case
 * @param choice is the direction the player inputs 
 * @param p is a pointer to the player object
 * @return is the Direction corresponding to the players choice 
*/
Direction checkChoice(std::string choice, Player* p ){

	if(!choice.compare("Up") || !choice.compare("up"))
		return Direction::Up;
 	else if(!choice.compare("Down") || !choice.compare("down"))
		return Direction::Down;
 	else if(!choice.compare("Left") || !choice.compare("left"))
 		return Direction::Left;
 	else if(!choice.compare("Right") || !choice.compare("right"))
 		return Direction::Right;
 	else{
 		return Direction::Invalid;
	}

}

/*
* Converts Direction to a Position 
* @param dir is the direction the player wants to move
* @param p is a pointer to the player object
* @return the Position of the desired direction
*/
Position DirectionToPosition(Direction dir, Player* p){
	Position current = p->get_position();
	Position new_position = current;

	switch(dir){
		case Direction::Up: 
			new_position.row -=1;
			return new_position;
		case Direction::Down: 
			new_position.row +=1;
			return new_position;
		case Direction::Right: 
			new_position.col +=1;
			return new_position;
		case Direction::Left: 
			new_position.col -=1;
			return new_position;
		default: return current;
	}

	
}
/*
* Constructor sets private variables, allocates space in vector arr_ and 
* sets the values for the board
* @param rows is the number of rows in theboard
* @param columns is the number of columns in the board
*/
Board::Board(const int rows, const int columns){
	rows_ = rows;
	cols_ = columns;
	arr_.reserve(rows * columns);
	int random_number;
	std::srand(std::time(nullptr));
	
	for(int r = 0; r < rows_; r++){
		for(int c = 0; c < cols_; c++){
			if(r == 0 && c == 0)
				SetValue(0,0, SquareType::Human);
			else if(r == rows_-1 && c == cols_-1)
				SetValue(r, c, SquareType::Exit);
			else{
				random_number = rand() % 50; 
				if(random_number <= 9)
					SetValue(r, c, SquareType::Wall);
				else if(random_number >= 10 && random_number <=14)
					SetValue(r, c, SquareType::Treasure);
				else if(random_number >=15 && random_number <=16)
					SetValue(r, c, SquareType::Health);
				else if(random_number >= 17 && random_number <=18)
					SetValue(r, c, SquareType::Hammer);
				else if(random_number >=19 && random_number <=20)
					SetValue(r,c, SquareType::Trap);
				else{
					SetValue(r, c, SquareType::Empty);
				}
			}
		}
	}	
}

/*
* Retrieves the value of the board at a given location
* @param row is the row of the location
* @param col is the column of the location
* @return is the SquareType of the given location
*/
SquareType Board::GetValue(int row, int col){
	return arr_[(row * rows_) + col];
}

/*
* Sets the SquareType of the given (row,column) location
* @param row the row of the location to be set
* @param col the column of the location to be set 
*/
void Board::SetValue(int row, int col, SquareType value){
	arr_[(row * rows_) + col] = value;
}

/*
* Get all possible moves that the player can make without hitting a Wall
* @param p is a pointer to the player object
* @return is a vector of Positions that the player can move to without hitting a Wall
*/
std::vector<Position> Board::GetMoves(Player *p){
	std::vector<Position> moves; 
	Position current = p->get_position();
	Position right;
	right.row = current.row;
	right.col = current.col + 1;
	Position left;
	left.row = current.row;
	left.col = current.col - 1; 
	Position up;
	up.row = current.row - 1;
	up.col = current.col;
	Position down;
	down.row = current.row + 1; 
	down.col = current.col;

	if(right.col < cols_){
		if(GetValue(right.row, right.col) != SquareType::Wall)
			moves.push_back(right);
	}	
	if(left.col >= 0){
		if(GetValue(left.row, left.col) != SquareType::Wall)
			moves.push_back(left);
	}
	if(up.row >= 0){
		if(GetValue(up.row, up.col) != SquareType::Wall)
			moves.push_back(up);

	}
	if(down.row < rows_){
		if(GetValue(down.row, down.col) != SquareType::Wall)
			moves.push_back(down);
	}

	return moves;
}

/*
* Moves player to new position and carries out the functionality of the corresponding 
* square that they move to. 
* @param p is a pointer to the player object
* @param r is the row that the player will be moved to 
* @param c is the column that the player will be moved to 
* @return is true if the move is successful or false if the move is invalid, i.e.
* the desired space is of type Wall.
*/
bool Board::MovePlayer(Player *p, int r, int c){
	
	SquareType sq = GetValue(r,c);
	if(sq == SquareType::Wall)
		return false;
	
	else if(sq == SquareType::Treasure){
		std::cout<< "Nice Work! You found a treasure and gained 100 points!" << std::endl;
		p->ChangePoints(100);
		std::cout<< p->get_name() << " has " << p->get_points() << " points!" << std::endl;
	}

	else if(sq == SquareType::Exit){
		p->ChangePoints(1);

	}
	
	else if(sq == SquareType::Health){
		std::cout<< "Nice Work! You Gained a life!" << std::endl;
		p->GainLife();
		std::cout<< p->get_name() << " has " << p->get_lives() << " lives left!" << std::endl;
	
	}
	else if(sq == SquareType::Hammer){
		/* Get Rid of all the Walls */
		std::cout<< "You picked up a hammer! Say goodbye to all the walls!"<< std::endl;
		for(int r = 0; r < rows_; r++){
			for(int c = 0; c < cols_; c++){
				if(GetValue(r,c) == SquareType::Wall)
					SetValue(r, c, SquareType::Empty);
			}
		}
	}
	
	else if(sq == SquareType::Trap){
		std::cout<< "Uh Oh, you are on Fire. You have lost a life!" << std::endl;
		p->LoseLife();
		std::cout << p->get_name() << " has " << p->get_lives() << " lives left!" << std::endl;
	}
		
	Position current = p->get_position();
	SetValue(current.row, current.col, SquareType::Empty);
	Position new_position;
	new_position.row = r;
	new_position.col = c;
	p->SetPosition(new_position);
	SetValue(new_position.row, new_position.col, SquareType::Human);
	
	return true; 
	}

	

 /*
* Retrieves the SquareType of the Exit Square 
* @result is the SquareType of the Exit Square
*/
 SquareType Board::GetExitOccupant(){
 	return GetValue(rows_-1, cols_-1);
 }

/*
* Iterates through the vector of SquareTypes and prints the board to stdout
*/
 void Board::PrintBoard(){
 	for(int r = 0; r < rows_; r++){
		for(int c = 0; c < cols_; c++){
			std::cout << SquareTypeStringify(GetValue(r,c));
		}
	std::cout << "" << std::endl;
	}
 }

/*
* Maze constructor that sets the size of the game, instantiates a board object
* with this size. Also, sets turn count to 0
*
*/
Maze::Maze(const int size){
	size_ = size;
	board_ = new Board(size, size);
	turn_count_ = 0;
}

/*
* Populates a vector of Player objects that are currently playing the maze
* @param human is a pointer to the player object
* @param enemies is an unused variable at this point that could be used
* to add a number of enemy players to the maze 
*/
void Maze::NewGame(Player *human, const int enemies){
	players_.push_back(human);

}

/*
* The driving function of the maze interaction with the player
* TakeTurn is responsible for printing the board, displaying what
* moves the player can make and dealing with the player input
* @param p is a pointer to the player object
*/
void Maze::TakeTurn(Player *p){
	std::string choice; 

	board_->PrintBoard();

	std::vector<Position> moves = board_->GetMoves(p);
	std::cout << p->get_name() << " can move: ";
	for(Position pos: moves)
		std::cout << p->ToRelativePosition(pos) << " ";
	std::cout << "" << std::endl; 
	std::cout << "Please Enter Your Choice Here: ";
	std::cin  >> choice;
	
	Direction dir = checkChoice(choice,p);
	while(dir == Direction::Invalid){
		std::cout<<"Please Enter Your Choice Here: "; 
 		std::cin >> choice;
 		dir = checkChoice(choice,p);
	}
	
	Position new_position = DirectionToPosition(dir, p);
	if(!board_->MovePlayer(p, new_position.row, new_position.col))
	 	std::cout << "That is not a valid move" << std::endl;
	

}

/*
* Currently not used in the implementation, but could be used for a 
* multi-player maze 
* @return a pointer to the next player in the player vector
*
*/
Player * Maze::GetNextPlayer(){
	return players_.front();
}

/*
* Checks if game is over.
* Game can be over if (1) the player is at the exit (2) the player is stuck
* and there are no more moves to be made or (3) The player has no more lives
* @returns true if the game is over due to one of these conditions and false
* if the none of the game over conditions are met
*
*/
bool Maze::IsGameOver(){
	Player* p = players_.front();
	Position current = p->get_position();
	if(current.row == size_-1 && current.col == size_-1){
		board_->PrintBoard(); /* print board one last time */
		std::cout << "CONGRATS, YOU WIN!"<< std::endl;
		return true;
	}
	else if(board_->GetMoves(p).empty()){
		std::cout << "GAME OVER: NO MORE MOVES " << std::endl;
		p->LoseLife();
		return true;
	}
	else if(p->get_lives() == 0){
		std::cout << "GAME OVER: NO MORE LIVES!" << std::endl;
		return true;
	}
	else
		return false;

}









