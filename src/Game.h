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
	bool passant = false;
};

struct Move {
	std::pair<int, int> pos;
	std::pair<int, int> dest;
	char name;
};

class Board {
public:
	Board();
	Side getMovingSide() const { return movingSide; }
	bool makeMove(const std::pair<int, int>&, const std::pair<int, int>&, Side);
	std::vector<Figure*>* getPositions() { return &positions; };

private:
	void clearPassants();
	std::string defaultFigures = "RNBQKBNRPPPPPPPPpppppppprnbqkbnr"; // Upper white
	std::string defaultPositions = "RNBQKBNRPPPPPPPP" + std::string(32, ' ') + "pppppppprnbqkbnr";
	Figure figures[32];
	std::vector<Figure*> positions;
	Side movingSide = Side::white;
};