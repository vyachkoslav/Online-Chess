#pragma once

#include "pch.h"
#include "Chess.h"

namespace Chess {
	std::vector<Move> ChessLogic::availableMovesForFigure(const Figure&) const {
		std::vector<Move> out;
		return out;
	}
	ChessLogic::GameState ChessLogic::CheckBoardState() const {
		return ChessLogic::GameState::Unfinished;
	}

	Board::Board(size_t _width, std::vector<Figure*> startPositions) : 
		width(_width), positions(startPositions) {}
	bool Board::makeMove(Move){
		return false;
	}
	bool Board::undoMove(){
		return false;
	}

}