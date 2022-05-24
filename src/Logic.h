#pragma once

#include "pch.h"
#include "Game.h"

class Logic {
public:
	Logic(const Figure* (*board)[64]) { this->board = board; }

	std::vector<std::pair<int, int>> getAvailableMovesForFigure(std::pair<int, int>);

private:
	const Figure* (*board)[64];

	static bool inline isEmpty(const Figure*&);
	static bool inline isEnemy(const Figure*&, const Figure*&);
	static std::pair<int, int> inline indToPair(int);
	static int inline sgn(int);
	static int inline pairToInd(int, int);
	static int inline pairToInd(std::pair<int, int>);

	std::vector<std::pair<int, int>> availableMovesForPawn(std::pair<int, int>);
	std::vector<std::pair<int, int>> availableMovesForRook(std::pair<int, int>);
	std::vector<std::pair<int, int>> availableMovesForKnight(std::pair<int, int>);
	std::vector<std::pair<int, int>> availableMovesForBishop(std::pair<int, int>);
	std::vector<std::pair<int, int>> availableMovesForQueen(std::pair<int, int>);
	std::vector<std::pair<int, int>> availableMovesForKing(std::pair<int, int>);
};