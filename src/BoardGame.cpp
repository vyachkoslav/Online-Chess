#include "BoardGame.h"
#include <stdexcept>

namespace BoardGame {
	void Team::setAllies(Team* first, Team* second, bool shareAllies) {
		if (!shareAllies) {
			first->addAlly(second);
			second->addAlly(first);
		}
		else {
			auto firstAllies = first->getAllies();
			auto secondAllies = second->getAllies();
			for (auto& ally : firstAllies) {
				setAllies(second, ally, true);
			}
			for (auto& ally : secondAllies) {
				setAllies(first, ally, true);
			}
			setAllies(first, second);
		}
	}
	void Team::setEnemies(Team* first, Team* second, bool withSharedAllies) {
		if (!withSharedAllies) {
			first->removeAlly(second);
			second->removeAlly(first);
		}
		else {
			auto firstAllies = first->getAllies();
			auto secondAllies = second->getAllies();

			for (auto& fAlly : firstAllies) {
				for (auto& sAlly : firstAllies) {
					if (fAlly == sAlly) {
						setEnemies(first, sAlly, true);
						setEnemies(second, fAlly, true);
					}
				}
			}
			setEnemies(first, second);
		}
	}
	bool Team::isAlly(Team* team) const {
		if (!team)
			return false;
		return std::find(allies.begin(), allies.end(), team) != allies.end();
	}
	void Team::addAlly(Team* enemy, bool shared) {
		bool isValid = enemy && this != enemy;
		if (!isValid)
			return;

		if (!isAlly(enemy)) {
			allies.push_back(enemy);
			if (shared)
				sharedAllies.push_back(enemy);
		}
		else if (!shared)
			tryRemoveSharedAlly(enemy);
	}
	void Team::removeAlly(Team* ally, bool shared) {
		bool isValid = ally && this != ally;
		if (!isValid)
			return;

		if (isAlly(ally)) {
			if (shared) {
				if(tryRemoveSharedAlly(ally))
					allies.erase(std::find(allies.begin(), allies.end(), ally));
			}
			else
				allies.erase(std::find(allies.begin(), allies.end(), ally));
		}
	}
	bool Team::tryRemoveSharedAlly(Team* ally) {
		auto sharedAllyIter = std::find(sharedAllies.begin(), sharedAllies.end(), ally);
		bool isShared = sharedAllyIter != sharedAllies.end();
		if (isShared) {
			sharedAllies.erase(sharedAllyIter);
		}
		return isShared;
	}


	Board::Board(size_t _width, const std::vector<Figure*>& startPositions) :
		width(_width), positions(startPositions) {
		height = positions.size() / width;

		if(height * width != startPositions.size())
			throw std::invalid_argument("Board is not symmetrical");
	}
	Board::~Board() {
		for (auto& move : moves)
			delete move;
	}
	bool Board::makeMove(const Move& move){
		if (move.size() > 0) {
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
					dest->posOnBoard = action.figureAtDest.posOnBoard;
					pos = nullptr;
				}
			}

			Move* savedMove = new Move(move);
			moves.push_back(savedMove);
			currentMove = moves.end() - 1;
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