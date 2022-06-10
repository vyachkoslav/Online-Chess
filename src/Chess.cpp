#include "Chess.h"

namespace Chess {
	Board::Board(size_t _width, const std::vector<Figure*>& startPositions) : 
		width(_width), positions(startPositions) {
		height = positions.size() / width;
	}
	bool Board::makeMove(const Move& move, Side side){
		if (move.size() > 0 && side == movingSide) {
			for (const Action& action : move) {
				auto& pos = positions[action.figureAtStart.posOnBoard];
				auto& dest = positions[action.figureAtDest.posOnBoard];

				if (!pos)
					return false;
				if (action.nameAfter == ' ') {
					pos = nullptr;
					dest = nullptr;
				}
				else {
					dest = pos;
					dest->moveCount++;
					dest->name = action.nameAfter;
					pos = nullptr;
				}
			}
			movingSide = movingSide == Side::White ? Side::Black : Side::White;

			moves.push_back(move);
			return true;
		}
		return false;
	}
	bool Board::undoMove(){
		return false;
	}
	bool Board::redoMove(){
		return false;
	}

	void Connection::setConnectionSettings(std::string ip, unsigned int port){

	}
	bool Connection::connect(){
		return false;
	}
	bool Connection::isConnected() const{
		return false;
	}
	void Connection::closeConnection(){

	}
	std::string Connection::getMessage(){
		return std::string();
	}
}