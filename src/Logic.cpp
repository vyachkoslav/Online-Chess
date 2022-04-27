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
int inline Logic::sgn(int val) {
	return (0 < val) - (val < 0);
}

VectorOfPairs Logic::availableMovesForPawn(std::pair<int, int> figPosition, bool hasMoven) {
	std::vector<std::pair<int, int>> out;

	int index = figPosition.first * 8 + figPosition.second;
	const char* fig = *board + index;
	int frontCell = std::isupper(*fig) ? 8 : -8;

	if ((std::isupper(*fig) && index / 8 == 1) ||
		(std::islower(*fig) && index / 8 == 7)) { // is at start pos
		out.push_back(indToPair(index + frontCell));
		out.push_back(indToPair(index + frontCell * 2));
		return out;
	}
	if (fig[frontCell] == ' ') {
		out.push_back(indToPair(index + frontCell));
	}
	if (index % 8 != 0 && isEnemy(*fig, fig[frontCell - sgn(frontCell)])) { // not on edge and diag left is enemy
		out.push_back(indToPair(index + frontCell - sgn(frontCell)));
	}
	if (index % 8 != 7 && isEnemy(*fig, fig[frontCell + sgn(frontCell)])) { // not on edge and diag right is enemy
		out.push_back(indToPair(index + frontCell + sgn(frontCell)));
	}
	return out;
}