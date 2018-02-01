#include "Maze.h"
#include <vector>
#include <string>
#include <stdlib.h>
#include <ctime>

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

// TODO: you MUST implement the following functions
SquareType Board::GetValue(int row, int col){
	return arr_[(row * rows_) + col];
}
void Board::SetValue(int row, int col, SquareType value){
	arr_[(row * rows_) + col] = value;
}
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


bool Board::MovePlayer(Player *p, int r, int c){
	
	SquareType sq = GetValue(r,c);
	if(sq == SquareType::Wall)
		return false;
	
	else if(sq == SquareType::Treasure)
		p->ChangePoints(100);

	else if(sq == SquareType::Exit)
		p->ChangePoints(1);
	
	else if(sq == SquareType::Health)
		p->GainLife();
	else if(sq == SquareType::Hammer){
		/* Get Rid of all the Walls */
		for(int r = 0; r < rows_; r++){
			for(int c = 0; c < cols_; c++){
				if(GetValue(r,c) == SquareType::Wall)
					SetValue(r, c, SquareType::Empty);
			}
		}
		

	}
	
	else if(sq == SquareType::Trap)
		p->LoseLife();
		
	Position current = p->get_position();
	SetValue(current.row, current.col, SquareType::Empty);
	Position new_position;
	new_position.row = r;
	new_position.col = c;
	p->SetPosition(new_position);
	SetValue(new_position.row, new_position.col, SquareType::Human);
	
	return true; 
	}

	

 
 SquareType Board::GetExitOccupant(){
 	return GetValue(rows_-1, cols_-1);
 }

 void Board::PrintBoard(){
 	for(int r = 0; r < rows_; r++){
		for(int c = 0; c < cols_; c++){
			std::cout << SquareTypeStringify(GetValue(r,c));
		}
	std::cout << "" << std::endl;
	}
 }

// 	// TODO: implement these functions
Maze::Maze(const int size){
	size_ = size;
	board_ = new Board(size, size);
	turn_count_ = 0;
}
void Maze::NewGame(Player *human, const int enemies){
	players_.push_back(human);

}
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
Player * Maze::GetNextPlayer(){
	return players_.front();
}

bool Maze::IsGameOver(){
	Player* p = players_.front();
	Position current = p->get_position();
	if(current.row == size_-1 && current.col == size_-1){
		board_->PrintBoard(); /* print board one last time */
		return true;
	}
	else if(board_->GetMoves(p).empty()){
		std::cout << "There are no more moves, GAME OVER. " << std::endl;
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









