#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <iostream>
#include <string>


struct Position {
	int row;
	int col;
};


class Player {
public:
	// TODO: implement
	Player(const std::string name);  // constructor
	
	// These are already implemented for you
	std::string get_name() const {return name_; }  // inline member function
	int get_lives() const {return lives_; }  // inline member function
	int get_points() const {return points_; }  // inline member function
	Position get_position() const {return pos_; }  // inline member function

	// TODO: implement the following functions
	// You MUST implement the following functions
	void ChangePoints(const int x);  // regular member functions
	void LoseLife();
	void GainLife();
	void SetPosition(Position pos);

	// You may want to implement these functions as well
	// ToRelativePosition is a function we used to translate positions
	// into directions relative to the player (up, down, etc)
	std::string ToRelativePosition(Position other);
	std::string Stringify();

	// You may add other functions as needed

private:
	std::string name_;
	int lives_;
	int points_;
	Position pos_;

	// You may add other data as needed

}; // class Player

#endif  // _PLAYER_H_