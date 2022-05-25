#include "Logic.h"
#include <stdexcept>

VectorOfPairs Logic::getAvailableMovesForFigure(Pos2D figPos)
{
	int index = figPos.first + figPos.second * 8;
	const Figure* fig = (*board)[index];
	if (!fig)
		return VectorOfPairs();
	switch (std::tolower(fig->name)) {
	case 'p':
		return availableMovesForPawn(figPos);
		break;
	case 'r':
		return availableMovesForRook(figPos);
		break;
	case 'n':
		return availableMovesForKnight(figPos);
		break;
	case 'b':
		return availableMovesForBishop(figPos);
		break;
	case 'q':
		return availableMovesForQueen(figPos);
		break;
	case 'k':
		return availableMovesForKing(figPos);
		break;
	}
	
}

bool inline Logic::isEmpty(Figure* const & x) {
	return !x;
}
bool inline Logic::isEnemy(Figure* const & x, Figure* const & y) {
	return !(isEmpty(x) || isEmpty(y)) && std::islower(x->name) != std::islower(y->name);
}
Pos2D inline Logic::indToPair(int i) {
	return Pos2D(i % 8, i / 8);
}
int inline Logic::pairToInd(int x, int y) {
	return x + y * 8;
}
int inline Logic::pairToInd(Pos2D pos) {
	return pos.first + pos.second * 8;
}
int inline Logic::sgn(int val) {
	return (0 < val) - (val < 0);
}

VectorOfPairs Logic::movesBeforeFigureOrEnd(Pos2D figPos, bool (*filter)(Pos2D), void (*iter)(Pos2D*)) {
	VectorOfPairs out;
	Figure* fig = (*board)[pairToInd(figPos)];
	iter(&figPos);
	for (Pos2D pos = figPos; filter(pos); iter(&pos)) {
		Figure* other = (*board)[pairToInd(pos)];
		if (isEmpty(other)) {
			out.push_back(Pos2D(pos));
		}
		else if (isEnemy(fig, (*board)[pairToInd(pos)])) {
			out.push_back(Pos2D(pos));
			break;
		}
		else {
			break;
		}
		
	}
	return out;
}

VectorOfPairs Logic::availableMovesForPawn(Pos2D figPos) {
	if (figPos.second == 7 || figPos.second == 0)
		throw std::invalid_argument("Pawn shouldn't be at last row");
	
	VectorOfPairs out;
	
	int index = pairToInd(figPos.first, figPos.second);
	std::vector<Figure*>::const_iterator fig = board->begin() + index;
	const char& name = (*fig)->name;
	int frontCell = std::isupper(name) ? 8 : -8;

	if (isEmpty(fig[frontCell])) {
		out.push_back(indToPair(index + frontCell));
		if (((std::isupper(name) && figPos.second == 1) ||								// is at start pos
			(std::islower(name) && figPos.second == 6)) &&
			isEmpty(fig[frontCell * 2])) {
			out.push_back(indToPair(index + frontCell * 2));
		}
	}
	bool onLeftEdge = frontCell > 0 ? figPos.first == 0 : figPos.first == 7;
	bool onRightEdge = frontCell > 0 ? figPos.first == 7 : figPos.first == 0;;
	if (!onLeftEdge && !isEmpty(fig[-1]) && isEnemy(*fig, fig[-1]) && fig[-1]->passant) { // passants
		out.push_back(indToPair(index + frontCell - sgn(frontCell)));
	}
	if (!onRightEdge && !isEmpty(fig[1]) && isEnemy(*fig, fig[1]) && fig[1]->passant) {
		out.push_back(indToPair(index + frontCell + sgn(frontCell)));
	}
	if (!onLeftEdge && isEnemy(*fig, fig[frontCell - sgn(frontCell)])) { // not on edge and diag left is enemy
		out.push_back(indToPair(index + frontCell - sgn(frontCell)));
	}
	if (!onRightEdge && isEnemy(*fig, fig[frontCell + sgn(frontCell)])) { // not on edge and diag right is enemy
		out.push_back(indToPair(index + frontCell + sgn(frontCell)));
	}
	return out;
}

VectorOfPairs Logic::availableMovesForRook(Pos2D figPos) {
	VectorOfPairs out;

	auto right = movesBeforeFigureOrEnd(figPos, [](auto pos) { return pos.first < 8; }, [](Pos2D* pos) { ++pos->first; });
	auto left = movesBeforeFigureOrEnd(figPos, [](auto pos) { return pos.first >= 0; }, [](Pos2D* pos) { --pos->first; });
	auto down = movesBeforeFigureOrEnd(figPos, [](auto pos) { return pos.second < 8; }, [](Pos2D* pos) { ++pos->second; });
	auto up = movesBeforeFigureOrEnd(figPos, [](auto pos) { return pos.second >= 0; }, [](Pos2D* pos) { --pos->second; });

	out.insert(out.end(), right.begin(), right.end());
	out.insert(out.end(), left.begin(), left.end());
	out.insert(out.end(), down.begin(), down.end());
	out.insert(out.end(), up.begin(), up.end());

	return out;
}

VectorOfPairs Logic::availableMovesForBishop(Pos2D figPos) {
	VectorOfPairs out;

	auto rightDown = movesBeforeFigureOrEnd(figPos, 
		[](auto pos) { return pos.first < 8 && pos.second < 8; }, 
		[](auto pos) { ++pos->first, ++pos->second; });
	auto leftUp = movesBeforeFigureOrEnd(figPos,
		[](auto pos) { return pos.first >= 0 && pos.second >= 0; }, 
		[](auto pos) { --pos->first, --pos->second; });
	auto rightUp = movesBeforeFigureOrEnd(figPos, 
		[](auto pos) { return pos.first < 8 && pos.second >= 0; }, 
		[](auto pos) { ++pos->first, --pos->second; });
	auto leftDown = movesBeforeFigureOrEnd(figPos, 
		[](auto pos) { return pos.first >= 0 && pos.second < 8; }, 
		[](auto pos) {  --pos->first, ++pos->second; });

	out.insert(out.end(), rightDown.begin(), rightDown.end());
	out.insert(out.end(), leftUp.begin(), leftUp.end());
	out.insert(out.end(), rightUp.begin(), rightUp.end());
	out.insert(out.end(), leftDown.begin(), leftDown.end());

	return out;
}

VectorOfPairs Logic::availableMovesForQueen(Pos2D figPos) {
	VectorOfPairs out;

	VectorOfPairs rookMoves = availableMovesForRook(figPos);
	VectorOfPairs bishopMoves = availableMovesForBishop(figPos);

	out.insert(out.end(), rookMoves.begin(), rookMoves.end());
	out.insert(out.end(), bishopMoves.begin(), bishopMoves.end());

	return out;
}

VectorOfPairs Logic::availableMovesForKnight(Pos2D figPos) {
	VectorOfPairs out;
	Figure* fig = (*board)[pairToInd(figPos)];

	int options[4] = { -2, -1, 1, 2 };
	for (int x : options) {
		for (int y : options) {
			if (abs(x) != abs(y)) {
				int newX = figPos.first + x;
				int newY = figPos.second + y;
				if (newX < 8 && newX >= 0 &&
					newY < 8 && newY >= 0){
					Figure* newFig = (*board)[pairToInd(newX, newY)];
					if (isEmpty(newFig) || isEnemy(fig, newFig))
						out.push_back(Pos2D(newX, newY));
				}
			}
		}
	}

	return out;  
}

VectorOfPairs Logic::availableMovesForKing(Pos2D figPos) {
	VectorOfPairs out;
	Figure* fig = (*board)[pairToInd(figPos)];
	int options[3] = { -1, 0, 1};
	for (int x : options) {
		for (int y : options) {
			if (!(x == 0 && y == 0)) {
				int newX = figPos.first + x;
				int newY = figPos.second + y;
				if (newX < 8 && newX > 0 &&
					newY < 8 && newY > 0) {
					Figure* newFig = (*board)[pairToInd(newX, newY)];
					if (isEmpty(newFig) || isEnemy(fig, newFig))
						out.push_back(Pos2D(newX, newY));
				}
			}
		}
	}
	
	return out;
}