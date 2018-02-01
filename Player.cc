
#include "Player.h"
#include <string>



Player::Player(const std::string name){
	name_ = name;
	pos_.row = 0;
	pos_.col = 0;
	points_ = 0;
	lives_ = 3; 
}

	// TODO: implement the following functions
	// You MUST implement the following functions
void Player::ChangePoints(const int x){
	points_ = points_ + x;
}

void Player::LoseLife(){
	lives_ -=1; 
}
void Player::GainLife(){
	lives_ +=1;
}

void Player::SetPosition(Position pos){
	pos_.row = pos.row;
	pos_.col = pos.col;
}
std::string Player::ToRelativePosition(Position other){
	if(pos_.row < other.row)
		return "Down";
	else if(pos_.row > other.row)
		return "Up";
	else if(pos_.col < other.col)
		return "Right";
	else 
		return "Left";	
}
 // std::string Player::Stringify(){

 // }
