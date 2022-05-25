#pragma once

#include "pch.h"
#include "Game.h"

using Pos2D = std::pair<int, int>;
using VectorOfPairs = std::vector<Pos2D>;

class Logic {
public:
	Logic(const std::vector<Figure*> *board) { this->board = board; }

	VectorOfPairs getAvailableMovesForFigure(Pos2D);

private:
	const std::vector<Figure*>* board;

	static bool inline isEmpty(Figure* const &);
	static bool inline isEnemy(Figure* const &, Figure* const &);
	static Pos2D inline indToPair(int);
	static int inline sgn(int);
	static int inline pairToInd(int, int);
	static int inline pairToInd(Pos2D);
	
	VectorOfPairs movesBeforeFigureOrEnd(Pos2D, bool (*)(Pos2D), void (*)(Pos2D&));
	VectorOfPairs availableMovesForPawn(Pos2D);
	VectorOfPairs availableMovesForRook(Pos2D);
	VectorOfPairs availableMovesForKnight(Pos2D);
	VectorOfPairs availableMovesForBishop(Pos2D);
	VectorOfPairs availableMovesForQueen(Pos2D);
	VectorOfPairs availableMovesForKing(Pos2D);
};