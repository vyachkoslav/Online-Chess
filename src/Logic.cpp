#include "Logic.h"
#include <stdexcept>

using VectorOfPairs = std::vector<std::pair<int, int>>;

VectorOfPairs Logic::getAvailableMovesForFigure(std::pair<int, int> figPosition)
{
	int index = figPosition.first + figPosition.second * 8;
	const Figure* fig = (*board)[index];
	switch (fig->name) {
	case 'P':
		return availableMovesForPawn(figPosition);
	}
}

bool inline Logic::isEmpty(const Figure*& x) {
	return !x;
}
bool inline Logic::isEnemy(const Figure*& x, const Figure*& y) {
	return (isEmpty(x) || isEmpty(y)) || std::islower(x->name) != std::islower(y->name);
}
std::pair<int, int> inline Logic::indToPair(int i) {
	return std::pair<int, int>(i / 8, i % 8);
}
int inline Logic::pairToInd(int x, int y) {
	return x + y * 8;
}
int inline Logic::pairToInd(std::pair<int, int> pos) {
	return pos.first + pos.second * 8;
}
int inline Logic::sgn(int val) {
	return (0 < val) - (val < 0);
}

VectorOfPairs Logic::availableMovesForPawn(std::pair<int, int> figPosition) {
	if (figPosition.second == 7 || figPosition.second == 0)
		throw std::invalid_argument("Pawn shouldn't be at last row");
	
	std::vector<std::pair<int, int>> out;
	
	int index = pairToInd(figPosition.first, figPosition.second);
	const Figure** fig = board[index];
	const char& name = (*fig)->name;
	int frontCell = std::isupper(name) ? 8 : -8;

	if (isEmpty(fig[frontCell])) {
		out.push_back(indToPair(index + frontCell));
		if (((std::isupper(name) && index / 8 == 1) ||								// is at start pos
			(std::islower(name) && index / 8 == 6)) ||
			isEmpty(fig[frontCell * 2])) {
			out.push_back(indToPair(index + frontCell * 2));
		}
	}
	if (figPosition.first != 0 && isEnemy(*fig, fig[frontCell - sgn(frontCell)])) { // not on edge and diag left is enemy
		out.push_back(indToPair(index + frontCell - sgn(frontCell)));
	}
	if (figPosition.first != 7 && isEnemy(*fig, fig[frontCell + sgn(frontCell)])) { // not on edge and diag right is enemy
		out.push_back(indToPair(index + frontCell + sgn(frontCell)));
	}
	return out;
}

VectorOfPairs Logic::availableMovesForRook(std::pair<int, int> figPosition) {
	std::vector<std::pair<int, int>> out;
	for (int x = figPosition.first + 1; x < 8 && isEmpty(*board[pairToInd(x, figPosition.second)]); x++) {
		out.push_back(std::pair<int, int>(x, figPosition.second));
	}
	for (int x = figPosition.first - 1; x > 0 && isEmpty(*board[pairToInd(x, figPosition.second)]); x++) {
		out.push_back(std::pair<int, int>(x, figPosition.second));
	}
	for (int y = figPosition.second + 1; y < 8 && isEmpty(*board[pairToInd(figPosition.first, y)]); y++) {
		out.push_back(std::pair<int, int>(figPosition.first, y));
	}
	for (int y = figPosition.second - 1; y > 0 && isEmpty(*board[pairToInd(figPosition.first, y)]); y++) {
		out.push_back(std::pair<int, int>(figPosition.first, y));
	}
	return out;
}