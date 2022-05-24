#include "Logic.h"
#include <stdexcept>


using VectorOfPairs = std::vector<std::pair<int, int>>;

VectorOfPairs Logic::getAvailableMovesForFigure(std::pair<int, int> figPos)
{
	int index = figPos.first + figPos.second * 8;
	const Figure* fig = (*board)[index];
	switch (fig->name) {
	case 'P':
		return availableMovesForPawn(figPos);
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

VectorOfPairs Logic::movesBeforeFigureOrEnd(std::pair<int,int> figPos, bool (*filter)(std::pair<int,int>), void (*iter)(std::pair<int,int>&)) {
	VectorOfPairs out;
	const Figure* fig = *board[pairToInd(figPos)];
	iter(figPos);
	for (std::pair<int,int> pos = figPos; filter(pos); iter(pos)) {
		if (isEnemy(fig, *board[pairToInd(pos)])) {
			out.push_back(std::pair<int, int>(pos));
			break;
		}
		if (isEmpty(*board[pairToInd(pos)]))
			out.push_back(std::pair<int, int>(pos));
		else
			break;
	}
	return out;
}

VectorOfPairs Logic::availableMovesForPawn(std::pair<int, int> figPos) {
	if (figPos.second == 7 || figPos.second == 0)
		throw std::invalid_argument("Pawn shouldn't be at last row");
	
	VectorOfPairs out;
	
	int index = pairToInd(figPos.first, figPos.second);
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
	if (figPos.first != 0 && isEnemy(*fig, fig[frontCell - sgn(frontCell)])) { // not on edge and diag left is enemy
		out.push_back(indToPair(index + frontCell - sgn(frontCell)));
	}
	if (figPos.first != 7 && isEnemy(*fig, fig[frontCell + sgn(frontCell)])) { // not on edge and diag right is enemy
		out.push_back(indToPair(index + frontCell + sgn(frontCell)));
	}
	return out;
}

VectorOfPairs Logic::availableMovesForRook(std::pair<int, int> figPos) {
	VectorOfPairs out;

	auto right = movesBeforeFigureOrEnd(figPos, [](auto pos) { return pos.first < 8; }, [](auto pos) { ++pos.first; });
	auto left = movesBeforeFigureOrEnd(figPos, [](auto pos) { return pos.first > 0; }, [](auto pos) { --pos.first; });
	auto down = movesBeforeFigureOrEnd(figPos, [](auto pos) { return pos.second < 8; }, [](auto pos) { ++pos.second; });
	auto up = movesBeforeFigureOrEnd(figPos, [](auto pos) { return pos.second > 0; }, [](auto pos) { --pos.second; });

	out.insert(out.end(), right.begin(), right.end());
	out.insert(out.end(), left.begin(), left.end());
	out.insert(out.end(), down.begin(), down.end());
	out.insert(out.end(), up.begin(), up.end());

	return out;
}

VectorOfPairs Logic::availableMovesForBishop(std::pair<int, int> figPos) {
	VectorOfPairs out;

	auto rightDown = movesBeforeFigureOrEnd(figPos, 
		[](auto pos) { return pos.first < 8 && pos.second < 8; }, 
		[](auto pos) { ++pos.first, ++pos.second; });
	auto leftUp = movesBeforeFigureOrEnd(figPos,
		[](auto pos) { return pos.first > 0 && pos.second > 0; }, 
		[](auto pos) { --pos.first, --pos.second; });
	auto rightUp = movesBeforeFigureOrEnd(figPos, 
		[](auto pos) { return pos.first < 8 && pos.second > 0; }, 
		[](auto pos) { ++pos.first, --pos.second; });
	auto leftDown = movesBeforeFigureOrEnd(figPos, 
		[](auto pos) { return pos.first > 0 && pos.second < 8; }, 
		[](auto pos) {  --pos.first, ++pos.second; });

	out.insert(out.end(), rightDown.begin(), rightDown.end());
	out.insert(out.end(), leftUp.begin(), leftUp.end());
	out.insert(out.end(), rightUp.begin(), rightUp.end());
	out.insert(out.end(), leftDown.begin(), leftDown.end());

	return out;
}

VectorOfPairs Logic::availableMovesForQueen(std::pair<int, int> figPos) {
	VectorOfPairs out;

	VectorOfPairs rookMoves = availableMovesForRook(figPos);
	VectorOfPairs bishopMoves = availableMovesForBishop(figPos);

	out.insert(out.end(), rookMoves.begin(), rookMoves.end());
	out.insert(out.end(), bishopMoves.begin(), bishopMoves.end());

	return out;
}

VectorOfPairs Logic::availableMovesForKnight(std::pair<int, int> figPos) {
	VectorOfPairs out;
	const Figure* fig = *board[pairToInd(figPos)];

	int options[4] = { -2, -1, 1, 2 };
	for (int x : options) {
		for (int y : options) {
			if (abs(x) != abs(y)) {
				int newX = figPos.first + x;
				int newY = figPos.second + y;
				if (newX < 8 && newX > 0 &&
					newY < 8 && newY > 0){
					const Figure* newFig = *board[pairToInd(newX, newY)];
					if (isEmpty(newFig) || isEnemy(fig, newFig))
						out.push_back(std::pair<int, int>(newX, newY));
				}
			}
		}
	}

	return out;
}