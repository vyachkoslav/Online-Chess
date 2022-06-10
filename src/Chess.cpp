#include "Chess.h"

namespace Chess {
	Board::Board(size_t _width, const std::vector<Figure*>& startPositions) : 
		width(_width), positions(startPositions) {
		height = positions.size() / width;
	}
	bool Board::makeMove(Move){
		return false;
	}
	bool Board::undoMove(){
		return false;
	}
}