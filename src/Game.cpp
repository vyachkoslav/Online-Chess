#include "Game.h"

bool Player::makeMove(const std::vector<Action>&action, Board &board) {
	return board.makeMove(action, side);
}

Action::Action(std::pair<int, int> x, std::pair<int, int> y, char n) : 
	pos(x), dest(y), name(n) {}

Board::Board() {
	for (int i = 0; i < 32; i++)
		figures[i].name = defaultFigures[i];

	auto figIter = std::begin(figures);
	for (int i = 0; i < 64; i++) {
		if (defaultPositions[i] != ' ') {
			positions.push_back(figIter++);
		}
		else {
			positions.push_back(nullptr);
		}
	}
}

void Board::clearPassants() {
	for (Figure& fig : figures) {
		fig.passant = false;
	}
}

bool Board::makeMove(const std::vector<Action>& action, Side side) {
	
	if (side == movingSide) {
		clearPassants();
		for (Action act : action) {
			auto& pos = positions[act.pos.first + act.pos.second * 8];
			auto& dest = positions[act.dest.first + act.dest.second * 8];
			if (act.name == ' ') {
				pos = nullptr;
				dest = nullptr;
			}
			else {
				dest = pos;
				dest->moveCount++;
				dest->name = act.name;
				pos = nullptr;
			}
			if (dest) {
				if (std::tolower(dest->name) == 'p' && abs(act.dest.second - act.pos.second) == 2) {
					dest->passant = true;
				}
			}
		}
		movingSide = movingSide == Side::white ? Side::black : Side::white;
		
		return true;
	}
	return false;
}