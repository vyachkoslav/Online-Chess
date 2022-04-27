#pragma once

#include "pch.h"
#include "Game.h"

class Logic {
public:
	Logic(const char(*board)[64]) { this->board = board; }

	std::vector<std::pair<int, int>> getAvailableMovesForFigure(std::pair<int, int>, bool);

private:
	const char(*board)[64];

	static bool inline isEnemy(const char&, const char&);
	static std::pair<int, int> inline indToPair(int);

	std::vector<std::pair<int, int>> availableMovesForPawn(std::pair<int, int>, bool);
	std::vector<std::pair<int, int>> availableMovesForRook(std::pair<int, int>);
	std::vector<std::pair<int, int>> availableMovesForKnight(std::pair<int, int>);
	std::vector<std::pair<int, int>> availableMovesForBishop(std::pair<int, int>);
	std::vector<std::pair<int, int>> availableMovesForQueen(std::pair<int, int>);
	std::vector<std::pair<int, int>> availableMovesForKing(std::pair<int, int>, bool);
};