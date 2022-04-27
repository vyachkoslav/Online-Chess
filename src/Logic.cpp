#include "Logic.h"

using VectorOfPairs = std::vector<std::pair<int, int>>;

VectorOfPairs Logic::getAvailableMovesForFigure(std::pair<int, int> figPosition, bool hasMoven = false)
{
	int index = figPosition.first * 8 + figPosition.second;
	const char* fig = *board + index;
	switch (*fig) {
	case 'P':
		return availableMovesForPawn(figPosition, hasMoven);
	}
}

bool inline Logic::isEnemy(const char& x, const char& y) {
	return !(std::islower(x) && std::islower(y));
}
std::pair<int, int> inline Logic::indToPair(int i) {
	return std::pair<int, int>(i / 8, i % 8);
}

VectorOfPairs Logic::availableMovesForPawn(std::pair<int, int> figPosition, bool hasMoven) {
	std::vector<std::pair<int, int>> out;

	int index = figPosition.first * 8 + figPosition.second;
	const char* fig = *board + index;
	if (std::isupper(*fig) && index / 8 == 1) {
		out.push_back(indToPair(index + 8));
		out.push_back(indToPair(index + 16));
		return out;
	}
	if (fig[8] == ' ') {
		out.push_back(indToPair(index + 8));
	}
	if (index % 8 != 0 && isEnemy(*fig, fig[7])) { // not on edge and diag left is enemy
		out.push_back(indToPair(index + 7));
	}
	if (index % 8 != 7 && isEnemy(*fig, fig[9])) { // not on edge and diag right is enemy
		out.push_back(indToPair(index + 9));
	}
	return out;
}