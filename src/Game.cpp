#include "Game.h"

Player::Player(Side side) {
	this->side = side;
}

bool Player::makeMove(const std::string &move) {
	return false;
}

Board::Board() {
	int i = 0;
	for (auto el : positions)
		el = defaultPositions[i++];
}