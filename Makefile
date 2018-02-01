CXX = g++
CXXFLAGS	= -std=c++11 -ggdb -Wall

all: maze

clean:
	rm Player.o Maze.o maze

maze: Player.o Maze.o
	$(CXX) $(CXXFLAGS) main.cc Player.o Maze.o -o maze

Player.o: Player.cc
	$(CXX) $(CXXFLAGS) -c Player.cc

Maze.o: Maze.cc
	$(CXX) $(CXXFLAGS) -c Maze.cc
