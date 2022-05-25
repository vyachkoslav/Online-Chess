#include "Game.h"

Player::Player(Side side) {
	this->side = side;
}

bool Player::makeMove(const std::pair<int, int> &x, const std::pair<int, int> &y, Board &board) {
	return board.makeMove(x, y, side);
}

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

bool Board::makeMove(const std::pair<int, int>& x, const std::pair<int, int>& y, Side side) {
	if (side == movingSide) {
		clearPassants();
		positions[y.first + y.second * 8] = positions[x.first + x.second * 8];
		positions[y.first + y.second * 8]->moveCount++;
		positions[x.first + x.second * 8] = nullptr;
		movingSide = movingSide == Side::white ? Side::black : Side::white;

		if (std::tolower(positions[y.first + y.second * 8]->name) == 'p' && abs(y.second - x.second) == 2) {
			positions[y.first + y.second * 8]->passant = true;
		}

		return true;
	}
	return false;
}