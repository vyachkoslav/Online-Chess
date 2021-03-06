#include "Logic.h"
#include <stdexcept>

std::vector<Move> Logic::getAvailableMovesForFigure(Pos2D figPos)
{
	int index = figPos.first + figPos.second * 8;
	const Figure* fig = (*board)[index];
	if (!fig)
		return std::vector<Move>();
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
Action Logic::prepareAction(Pos2D pos, Pos2D dest) {
	return Action(pos, dest, (*board)[pairToInd(pos)]->name);
}

std::vector<Move> Logic::movesBeforeFigureOrEnd(Pos2D figPos, bool (*filter)(Pos2D), void (*iter)(Pos2D*)) {
	std::vector<Move> out;
	Figure* fig = (*board)[pairToInd(figPos)];
	Pos2D pos = figPos;
	iter(&pos);
	for (; filter(pos); iter(&pos)) {
		Figure* other = (*board)[pairToInd(pos)];
		if (isEmpty(other) || isEnemy(fig, (*board)[pairToInd(pos)])) {
			out.push_back(Move{ prepareAction(figPos, Pos2D(pos)) });
			if(!isEmpty(other))
				break;
		}
		else {
			break;
		}
		
	}
	return out;
}

std::vector<Move> Logic::moveAndTryPromote(Pos2D figPos, Pos2D toPos) {
	std::vector<Move> out;

	if (!(toPos.second == 7 || toPos.second == 0)) {
		out.push_back(Move{ prepareAction(figPos, toPos) });
	}
	else { // promotion
		auto editFunc = toPos.second == 7 ? toupper : tolower;
		out.push_back(Move{ Action(figPos, toPos, editFunc('Q')) });
		out.push_back(Move{ Action(figPos, toPos, editFunc('N')) });
		out.push_back(Move{ Action(figPos, toPos, editFunc('B')) });
		out.push_back(Move{ Action(figPos, toPos, editFunc('R')) });
	}

	return out;
}
std::vector<Move> Logic::availableMovesForPawn(Pos2D figPos) {
	if (figPos.second == 7 || figPos.second == 0)
		throw std::invalid_argument("Pawn shouldn't be at last row");
	
	std::vector<Move> out;
	
	int index = pairToInd(figPos.first, figPos.second);
	std::vector<Figure*>::const_iterator fig = board->begin() + index;
	const char& name = (*fig)->name;
	int frontCell = std::isupper(name) ? 8 : -8;

	if (isEmpty(fig[frontCell])) {
		Pos2D nextPos = indToPair(index + frontCell);
		auto moves = moveAndTryPromote(figPos, nextPos);
		out.insert(out.end(), moves.begin(), moves.end());
		if (((std::isupper(name) && figPos.second == 1) ||								// is at start pos
			(std::islower(name) && figPos.second == 6)) &&
			isEmpty(fig[frontCell * 2])) {
			out.push_back(Move{ prepareAction(figPos, indToPair(index + frontCell * 2)) });
		}
	}
	bool onLeftEdge = frontCell > 0 ? figPos.first == 0 : figPos.first == 7;
	bool onRightEdge = frontCell > 0 ? figPos.first == 7 : figPos.first == 0;;
	if (!onLeftEdge && !isEmpty(fig[-1]) && isEnemy(*fig, fig[-1]) && fig[-1]->passant) { // passant
		Move actions;
		actions.push_back(prepareAction(figPos, indToPair(index + frontCell - 1)));
		Pos2D enemyPos = Pos2D(figPos.first - 1, figPos.second);
		actions.push_back(Action(enemyPos, enemyPos, ' '));

		out.push_back(actions);
	}
	if (!onRightEdge && !isEmpty(fig[1]) && isEnemy(*fig, fig[1]) && fig[1]->passant) { // passant
		Move actions;
		actions.push_back(prepareAction(figPos, indToPair(index + frontCell + 1)));
		Pos2D enemyPos = Pos2D(figPos.first + 1, figPos.second);
		actions.push_back(Action(enemyPos, enemyPos, ' '));

		out.push_back(actions);
	}
	if (!onLeftEdge && isEnemy(*fig, fig[frontCell - sgn(frontCell)])) { // not on edge and diag left is enemy
		auto moves = moveAndTryPromote(figPos, indToPair(index + frontCell - sgn(frontCell)));
		out.insert(out.end(), moves.begin(), moves.end());
	}
	if (!onRightEdge && isEnemy(*fig, fig[frontCell + sgn(frontCell)])) { // not on edge and diag right is enemy
		auto moves = moveAndTryPromote(figPos, indToPair(index + frontCell + sgn(frontCell)));
		out.insert(out.end(), moves.begin(), moves.end());
	}
	return out;
}

std::vector<Move> Logic::availableMovesForRook(Pos2D figPos) {
	std::vector<Move> out;

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

std::vector<Move> Logic::availableMovesForBishop(Pos2D figPos) {
	std::vector<Move> out;

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

std::vector<Move> Logic::availableMovesForQueen(Pos2D figPos) {
	std::vector<Move> out;

	auto rookMoves = availableMovesForRook(figPos);
	auto bishopMoves = availableMovesForBishop(figPos);

	out.insert(out.end(), rookMoves.begin(), rookMoves.end());
	out.insert(out.end(), bishopMoves.begin(), bishopMoves.end());

	return out;
}

std::vector<Move> Logic::availableMovesForKnight(Pos2D figPos) {
	std::vector<Move> out;
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
						out.push_back(Move{ prepareAction(figPos, Pos2D(newX, newY)) });
				}
			}
		}
	}

	return out;  
}

std::vector<Move> Logic::availableMovesForKing(Pos2D figPos) {
	std::vector<Move> out;
	Figure* fig = (*board)[pairToInd(figPos)];
	int options[3] = { -1, 0, 1};
	for (int x : options) {
		for (int y : options) {
			if (!(x == 0 && y == 0)) {
				int newX = figPos.first + x;
				int newY = figPos.second + y;
				if (newX < 8 && newX >= 0 &&
					newY < 8 && newY >= 0) {
					Figure* newFig = (*board)[pairToInd(newX, newY)];
					if (isEmpty(newFig) || isEnemy(fig, newFig))
						out.push_back(Move{ prepareAction(figPos, Pos2D(newX, newY)) });
				}
			}
		}
	}
	if (fig->moveCount == 0) {
		Figure* leftRook = (*board)[pairToInd(0, figPos.second)];
		Figure* rightRook = (*board)[pairToInd(7, figPos.second)];
		if (!isEmpty(leftRook) && leftRook->moveCount == 0) {
			bool isFree = true;
			for (int i = 1; i < 4; ++i) {
				if (!isEmpty((*board)[pairToInd(i, figPos.second)])) {
					isFree = false;
					break;
				}
			}
			if (isFree) {
				Move actions;
				actions.push_back(prepareAction(figPos, Pos2D(2, figPos.second)));
				Pos2D rookPos = Pos2D(0, figPos.second);
				actions.push_back(prepareAction(rookPos, Pos2D(3, figPos.second)));

				out.push_back(actions);
			}
		}
		if (!isEmpty(rightRook) && rightRook->moveCount == 0) {
			bool isFree = true;
			for (int i = 5; i < 7; ++i) {
				if (!isEmpty((*board)[pairToInd(i, figPos.second)])) {
					isFree = false;
					break;
				}
			}
			if (isFree) {
				Move actions;
				actions.push_back(prepareAction(figPos, Pos2D(6, figPos.second)));
				Pos2D rookPos = Pos2D(7, figPos.second);
				actions.push_back(prepareAction(rookPos, Pos2D(5, figPos.second)));

				out.push_back(actions);
			}
		}
	}
	return out;
}