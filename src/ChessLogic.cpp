#include "ChessLogic.h"

namespace Chess {
	ChessLogic::ChessLogic() : 
		GameLogic(8, std::vector<Figure*>{}) {
		positions = &board.getPositions();
	}

	GameState ChessLogic::CheckBoardState() const {
		return GameState::Unfinished;
	}

	std::vector<Move> ChessLogic::availableMovesForFigure(const Figure& figure) const {
		Pos2D figPos = indToPair(figure.posOnBoard);

		const Figure* fig = (*positions)[figure.posOnBoard];
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

		return std::vector<Move>();
	}

	bool inline ChessLogic::isEmpty(Figure* const& x) {
		return !x;
	}
	bool inline ChessLogic::isEnemy(Figure* const& x, Figure* const& y) {
		return !(isEmpty(x) || isEmpty(y)) && std::islower(x->name) != std::islower(y->name);
	}
	Pos2D inline ChessLogic::indToPair(size_t i) {
		return Pos2D(i % 8, i / 8);
	}
	size_t inline ChessLogic::pairToInd(size_t x, size_t y) {
		return x + y * 8;
	}
	size_t inline ChessLogic::pairToInd(Pos2D pos) {
		return pos.first + pos.second * 8;
	}
	int inline ChessLogic::sgn(int val) {
		return (0 < val) - (val < 0);
	}

	Figure* ChessLogic::getPassantFig() const{
		if (!board.canUndo())
			return nullptr;

		Move move = *(board.getCurrentMove() - 1);
		auto& action = move[0];
		auto& dest = (*positions)[action.figureAtDest.posOnBoard];
		if (dest) {
			long long startY = indToPair(action.figureAtStart.posOnBoard).second;
			long long endY = indToPair(action.figureAtDest.posOnBoard).second;

			if (std::tolower(dest->name) == 'p' && abs(endY - startY) == 2) {
				return dest;
			}
		}
		return nullptr;
	}

	Action ChessLogic::prepareAction(Pos2D pos, Pos2D dest) const {
		Figure def = Figure();

		const Figure first = *(*positions)[pairToInd(pos)];
		const Figure* secPtr = (*positions)[pairToInd(pos)];
		if (!secPtr) {
			secPtr = &def;
		}
		return Action(first, *secPtr, first.name);
	}
	Action ChessLogic::prepareAction(Pos2D pos, Pos2D dest, char newName) const {
		Figure def = Figure();

		const Figure first = *(*positions)[pairToInd(pos)];
		const Figure* secPtr = (*positions)[pairToInd(pos)];
		if (!secPtr) {
			secPtr = &def;
		}
		return Action(first, *secPtr, newName);
	}

	std::vector<Move> ChessLogic::movesBeforeFigureOrEnd(Pos2D figPos, bool (*filter)(Pos2D), void (*iter)(Pos2D*)) const {
		std::vector<Move> out;
		Figure* fig = (*positions)[pairToInd(figPos)];
		Pos2D pos = figPos;
		iter(&pos);
		for (; filter(pos); iter(&pos)) {
			Figure* other = (*positions)[pairToInd(pos)];
			if (isEmpty(other) || isEnemy(fig, (*positions)[pairToInd(pos)])) {
				out.push_back(Move{ prepareAction(figPos, Pos2D(pos)) });
				if (!isEmpty(other))
					break;
			}
			else {
				break;
			}

		}
		return out;
	}

	std::vector<Move> ChessLogic::moveAndTryPromote(Pos2D figPos, Pos2D toPos) const {
		std::vector<Move> out;

		if (!(toPos.second == 7 || toPos.second == 0)) {
			out.push_back(Move{ prepareAction(figPos, toPos) });
		}
		else { // promotion
			auto editFunc = toPos.second == 7 ? toupper : tolower;
			out.push_back(Move{ prepareAction(figPos, toPos, editFunc('Q')) });
			out.push_back(Move{ prepareAction(figPos, toPos, editFunc('N')) });
			out.push_back(Move{ prepareAction(figPos, toPos, editFunc('B')) });
			out.push_back(Move{ prepareAction(figPos, toPos, editFunc('R')) });
		}

		return out;
	}
	std::vector<Move> ChessLogic::availableMovesForPawn(Pos2D figPos) const {
		if (figPos.second == 7 || figPos.second == 0)
			throw std::invalid_argument("Pawn shouldn't be at last row");

		std::vector<Move> out;

		size_t index = pairToInd(figPos.first, figPos.second);
		std::vector<Figure*>::const_iterator fig = positions->begin() + index;
		const char& name = (*fig)->name;
		int frontCell = std::isupper(name) ? 8 : -8;

		if (isEmpty(fig[frontCell])) {
			Pos2D nextPos = indToPair(index + frontCell);
			auto moves = moveAndTryPromote(figPos, nextPos);
			std::copy(moves.begin(), moves.end(), std::back_inserter(out));
			if (((std::isupper(name) && figPos.second == 1) ||								// is at start pos
				(std::islower(name) && figPos.second == 6)) &&
				isEmpty(fig[frontCell * 2])) {
				out.push_back(Move{ prepareAction(figPos, indToPair(index + frontCell * 2)) });
			}
		}

		bool onLeftEdge = frontCell > 0 ? figPos.first == 0 : figPos.first == 7;
		bool onRightEdge = frontCell > 0 ? figPos.first == 7 : figPos.first == 0;

		Figure* passantFig = getPassantFig();
		if (!onLeftEdge && !isEmpty(fig[-1]) && isEnemy(*fig, fig[-1]) && fig[-1] == passantFig) { // passant
			Move actions;
			actions.push_back(prepareAction(figPos, indToPair(index + frontCell - 1)));
			Pos2D enemyPos = Pos2D(figPos.first - 1, figPos.second);
			actions.push_back(prepareAction(enemyPos, enemyPos, ' '));

			out.push_back(actions);
		}
		if (!onRightEdge && !isEmpty(fig[1]) && isEnemy(*fig, fig[1]) && fig[1] == passantFig) { // passant
			Move actions;
			actions.push_back(prepareAction(figPos, indToPair(index + frontCell + 1)));
			Pos2D enemyPos = Pos2D(figPos.first + 1, figPos.second);
			actions.push_back(prepareAction(enemyPos, enemyPos, ' '));

			out.push_back(actions);
		}

		if (!onLeftEdge && isEnemy(*fig, fig[frontCell - sgn(frontCell)])) { // not on edge and diag left is enemy
			auto moves = moveAndTryPromote(figPos, indToPair(index + frontCell - sgn(frontCell)));
			std::copy(moves.begin(), moves.end(), std::back_inserter(out));
		}
		if (!onRightEdge && isEnemy(*fig, fig[frontCell + sgn(frontCell)])) { // not on edge and diag right is enemy
			auto moves = moveAndTryPromote(figPos, indToPair(index + frontCell + sgn(frontCell)));
			std::copy(moves.begin(), moves.end(), std::back_inserter(out));
		}
		return out;
	}

	std::vector<Move> ChessLogic::availableMovesForRook(Pos2D figPos) const {
		std::vector<Move> out;

		auto right = movesBeforeFigureOrEnd(figPos, [](auto pos) { return pos.first < 8; }, [](Pos2D* pos) { ++pos->first; });
		auto left = movesBeforeFigureOrEnd(figPos, [](auto pos) { return pos.first >= 0; }, [](Pos2D* pos) { --pos->first; });
		auto down = movesBeforeFigureOrEnd(figPos, [](auto pos) { return pos.second < 8; }, [](Pos2D* pos) { ++pos->second; });
		auto up = movesBeforeFigureOrEnd(figPos, [](auto pos) { return pos.second >= 0; }, [](Pos2D* pos) { --pos->second; });

		std::copy(right.begin(), right.end(), std::back_inserter(out));
		std::copy(left.begin(), left.end(), std::back_inserter(out));
		std::copy(down.begin(), down.end(), std::back_inserter(out));
		std::copy(up.begin(), up.end(), std::back_inserter(out));

		return out;
	}

	std::vector<Move> ChessLogic::availableMovesForBishop(Pos2D figPos) const {
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

		std::copy(rightDown.begin(), rightDown.end(), std::back_inserter(out));
		std::copy(leftUp.begin(), leftUp.end(), std::back_inserter(out));
		std::copy(rightUp.begin(), rightUp.end(), std::back_inserter(out));
		std::copy(leftDown.begin(), leftDown.end(), std::back_inserter(out));

		return out;
	}

	std::vector<Move> ChessLogic::availableMovesForQueen(Pos2D figPos) const {
		std::vector<Move> out;

		auto rookMoves = availableMovesForRook(figPos);
		auto bishopMoves = availableMovesForBishop(figPos);

		std::copy(rookMoves.begin(), rookMoves.end(), std::back_inserter(out));
		std::copy(bishopMoves.begin(), bishopMoves.end(), std::back_inserter(out));

		return out;
	}

	std::vector<Move> ChessLogic::availableMovesForKnight(Pos2D figPos) const {
		std::vector<Move> out;
		Figure* fig = (*positions)[pairToInd(figPos)];

		int options[4] = { -2, -1, 1, 2 };
		for (int x : options) {
			for (int y : options) {
				if (abs(x) != abs(y)) {
					size_t newX = figPos.first + x;
					size_t newY = figPos.second + y;
					if (newX < 8 && newX >= 0 &&
						newY < 8 && newY >= 0) {
						Figure* newFig = (*positions)[pairToInd(newX, newY)];
						if (isEmpty(newFig) || isEnemy(fig, newFig))
							out.push_back(Move{ prepareAction(figPos, Pos2D(newX, newY)) });
					}
				}
			}
		}

		return out;
	}

	std::vector<Move> ChessLogic::availableMovesForKing(Pos2D figPos) const {
		std::vector<Move> out;
		Figure* fig = (*positions)[pairToInd(figPos)];
		int options[3] = { -1, 0, 1 };
		for (int x : options) {
			for (int y : options) {
				if (!(x == 0 && y == 0)) {
					size_t newX = figPos.first + x;
					size_t newY = figPos.second + y;
					if (newX < 8 && newX >= 0 &&
						newY < 8 && newY >= 0) {
						Figure* newFig = (*positions)[pairToInd(newX, newY)];
						if (isEmpty(newFig) || isEnemy(fig, newFig))
							out.push_back(Move{ prepareAction(figPos, Pos2D(newX, newY)) });
					}
				}
			}
		}
		if (fig->moveCount == 0) {
			Figure* leftRook = (*positions)[pairToInd(0, figPos.second)];
			Figure* rightRook = (*positions)[pairToInd(7, figPos.second)];
			if (!isEmpty(leftRook) && leftRook->moveCount == 0) {
				bool isFree = true;
				for (int i = 1; i < 4; ++i) {
					if (!isEmpty((*positions)[pairToInd(i, figPos.second)])) {
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
					if (!isEmpty((*positions)[pairToInd(i, figPos.second)])) {
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
}