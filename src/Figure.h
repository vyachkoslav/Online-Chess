#pragma once

class Figure;

#include "pch.h"
#include "Game.h"

class Figure {
protected:
	Side side;
	std::string position;
	std::string sprite;
	unsigned moveCount = 0;

public:
	Figure(Side side) { this->side = side; }

	Side getSide() const { return side; }
	std::string getPosition() const { return position; }
	std::string getSprite() const { return sprite; }

	virtual std::vector<std::string> getAvailableMoves() = 0;
};

class Pawn : public Figure {};
class Knight : public Figure {};
class Bishop : public Figure {};
class Rook : public Figure {};
class Queen : public Figure {};
class King : public Figure {};