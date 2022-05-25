#include "Game.h"

Player::Player(Side side) {
	this->side = side;
}

bool Player::makeMove(const Action &action, Board &board) {
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
	for (Figure fig : figures) {
		fig.passant = false;
	}
}

bool Board::makeMove(const Action &action, Side side) {
	auto x = action.pos;
	auto y = action.dest;
	if (side == movingSide) {
		clearPassants();
		if (action.name == ' ') {
			positions[x.first + x.second * 8] = nullptr;
			positions[y.first + y.second * 8] = nullptr;
		}
		else {
			positions[y.first + y.second * 8] = positions[x.first + x.second * 8];
			positions[y.first + y.second * 8]->moveCount++;
			positions[y.first + y.second * 8]->name = action.name;
			positions[x.first + x.second * 8] = nullptr;
		}
		movingSide = movingSide == Side::white ? Side::black : Side::white;
		if (positions[y.first + y.second * 8]) {
			if (std::tolower(positions[y.first + y.second * 8]->name) == 'p' && abs(y.second - x.second) == 2) {
				positions[y.first + y.second * 8]->passant = true;
			}
		}
		return true;
	}
	return false;
}