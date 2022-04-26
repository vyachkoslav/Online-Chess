#pragma once

#include "pch.h"

enum class Side { white, black };

class Figure {
protected:
	Side side;
	std::string position;
	std::string sprite;

public:
	Side getSide() { return side; }
	std::string getPosition() { return position; }
	std::string getSprite() { return sprite; }

	virtual std::vector<std::string> getAvailableMoves() = 0;
};