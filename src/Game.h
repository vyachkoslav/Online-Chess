#pragma once

class Game;
class Player;
struct Figure;
class Board;

enum class Side { white, black };

#include "pch.h"

class Game{

};

class Player {
public:
	Player(Side);
	Side getSide() const { return side; }
	bool makeMove(const std::pair<int,int> &, const std::pair<int, int>&, Board &);

private:
	Side side;
};

struct Figure {
	int moveCount = 0;
	char name = '\0';
};

class Board {
public:
	Board();
	Side getMovingSide() const { return movingSide; }
	bool makeMove(const std::pair<int, int>&, const std::pair<int, int>&, Side);
	std::vector<Figure*>* getPositions() { return &positions; };

private:
	std::string defaultFigures = "RNBQKBNRPPPPPPPPpppppppprnbqkbnr"; // Upper white
	std::string defaultPositions = "RNBQKBNRPPPPPPPP" + std::string(32, ' ') + "pppppppprnbqkbnr";
	Figure figures[32];
	std::vector<Figure*> positions;
	Side movingSide = Side::white;
};