#pragma once

class Game;
class Player;
struct Figure;
struct Action;
class Board;

enum class Side { white, black };

#include "pch.h"

class Game{

};

class Player {
public:
	Player(Side s) : side(s) {}
	Side getSide() const { return side; }
	bool makeMove(const Action&, Board &);

private:
	const Side side;
};

struct Figure {
	int moveCount = 0;
	char name = '\0';
	bool passant = false;
};

struct Action {
	Action(std::pair<int, int>, std::pair<int, int>, char);
	std::pair<int, int> pos;
	std::pair<int, int> dest;
	char name;
};

class Board {
public:
	Board();
	Side getMovingSide() const { return movingSide; }
	bool makeMove(const Action&, Side);
	const std::vector<Figure*>* getPositions() const { return &positions; };

private:
	void clearPassants();
	std::string defaultFigures = "RNBQKBNRPPPPPPPPpppppppprnbqkbnr"; // Upper white
	std::string defaultPositions = "RNBQKBNRPPPPPPPP" + std::string(32, ' ') + "pppppppprnbqkbnr";
	Figure figures[32];
	std::vector<Figure*> positions;
	Side movingSide = Side::white;
};