#pragma once

class Game;
class Player;
class Board;

enum class Side { white, black };

#include "pch.h"
#include "Figure.h"

class Game{

};

class Player {
public:
	Player(Side);
	Side getSide() const { return side; }
	bool makeMove(std::string);

private:
	Side side;
};

class Board {
public:
	Board();
	Side getMovingSide() const { return movingSide; }
	std::string toString() const;
	std::vector<Figure*> toArray() const;

private:
	std::vector<Figure*> positions;
	Side movingSide = Side::white;
};