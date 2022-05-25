#pragma once

#include "pch.h"
#include "Game.h"

using Pos2D = std::pair<int, int>;
using VectorOfPairs = std::vector<Pos2D>;
using Move = std::vector<Action>;

class Logic {
public:
	Logic(const std::vector<Figure*> *board) { this->board = board; }

	std::vector<Move> getAvailableMovesForFigure(Pos2D);

private:
	const std::vector<Figure*>* board;

	static bool inline isEmpty(Figure* const &);
	static bool inline isEnemy(Figure* const &, Figure* const &);
	static Pos2D inline indToPair(int);
	static int inline sgn(int);
	static int inline pairToInd(int, int);
	static int inline pairToInd(Pos2D);

	Action prepareAction(Pos2D, Pos2D);
	
	std::vector<Move> movesBeforeFigureOrEnd(Pos2D, bool (*)(Pos2D), void (*)(Pos2D*));
	std::vector<Move> availableMovesForPawn(Pos2D);
	std::vector<Move> availableMovesForRook(Pos2D);
	std::vector<Move> availableMovesForKnight(Pos2D);
	std::vector<Move> availableMovesForBishop(Pos2D);
	std::vector<Move> availableMovesForQueen(Pos2D);
	std::vector<Move> availableMovesForKing(Pos2D);
};