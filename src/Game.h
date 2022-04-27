#pragma once

class Game;
class Player;
class Board;

enum class Side { white, black };

#include "pch.h"

class Game{

};

class Player {
public:
	Player(Side);
	Side getSide() const { return side; }
	bool makeMove(const std::string &);

private:
	Side side;
};

class Board {
public:
	Board();
	Side getMovingSide() const { return movingSide; }
	const char *getPositions() const { return positions; };

private:
	std::string defaultPositions = "RNBQKBNRPPPPPPPP" + std::string(32, ' ') + "pppppppprnbqkbnr";
	char positions[64];
	Side movingSide = Side::white;
};