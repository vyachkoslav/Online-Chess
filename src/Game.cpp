#include "Game.h"

Player::Player(Side side) {
	this->side = side;
}

bool Player::makeMove(std::string move) {
	return false;
}


Board::Board() {

}
std::string Board::toString() const {
	return "";
}
std::vector<Figure*> Board::toArray() const {
	return std::vector<Figure*>();
}
