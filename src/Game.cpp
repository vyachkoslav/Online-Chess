#include "Game.h"

Player::Player(Side side) {
	this->side = side;
}

bool Player::makeMove(const std::string &move) {
	return false;
}

Board::Board() {
	for (int i = 0; i < 32; i++)
		figures[i].name = defaultFigures[i];

	auto figIter = std::begin(figures);
	for (int i = 0; i < 64; i++) {
		if (defaultPositions[i] != ' ') {
			positions[i] = figIter++;
		}
		else {
			positions[i] = nullptr;
		}
	}
}