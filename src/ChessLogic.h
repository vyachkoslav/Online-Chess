#pragma once

namespace BoardGame {
	class ChessLogic;
}

#include "pch.h"
#include "BoardGame.h"

namespace BoardGame {
	class ChessLogic : public GameLogic {
	public:
		ChessLogic();

		virtual std::vector<Move> availableMovesForFigure(const Figure&) const;
		virtual std::vector<Move> availableMovesForFigure(size_t) const;
		virtual GameState CheckBoardState() const;
		virtual Board* getBoard() override { return board; }
	private:
		const std::string defaultPositions = "RNBQKBNRPPPPPPPP" + std::string(32, ' ') + "pppppppprnbqkbnr";

		Board* board;
		const std::vector<Figure*>* positions;
		std::vector<Figure*> figures;

		static bool inline isEmpty(Figure* const&);
		static bool inline isEnemy(Figure* const&, Figure* const&);

		static Pos2D inline indToPair(size_t);
		static size_t inline pairToInd(size_t, size_t);
		static size_t inline pairToInd(Pos2D);

		static int inline sgn(int);

		Figure* getPassantFig() const;

		Action prepareAction(Pos2D, Pos2D) const;
		Action prepareAction(Pos2D, Pos2D, char) const;

		std::vector<Move> movesBeforeFigureOrEnd(Pos2D, bool (*)(Pos2D), void (*)(Pos2D*)) const;
		std::vector<Move> moveAndTryPromote(Pos2D, Pos2D) const;
		std::vector<Move> availableMovesForPawn(Pos2D) const;
		std::vector<Move> availableMovesForRook(Pos2D) const;
		std::vector<Move> availableMovesForKnight(Pos2D) const;
		std::vector<Move> availableMovesForBishop(Pos2D) const;
		std::vector<Move> availableMovesForQueen(Pos2D) const;
		std::vector<Move> availableMovesForKing(Pos2D) const;
	};
}