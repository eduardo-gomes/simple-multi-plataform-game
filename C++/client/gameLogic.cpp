#include "gameLogic.hpp"

#include <random>

#include "../log.hpp"

namespace randGen {
std::default_random_engine ranEng((uint_fast32_t)time(NULL));
unsigned int GetUnsignedMod(unsigned int mod) {
	unsigned int Random = ranEng() - ranEng.min();
	Random %= mod;
	return Random;
}
}  // namespace randGen

fruit::fruit(pos pos) : x(pos.x), y(pos.y) {
}  //Need to anounce to clients
fruit::~fruit() {
}  //Need to anounce to clients

//std::vector<fruit> **fruitsVectorBoard;//2d array of vectors
gameBoard::gameBoard(unsigned int width, unsigned int height) : width(width), height(height) {
	logger::log("HaHa\n");
}

void gameBoard::addFruit(pos){

}
void gameBoard::rmFruit(pos){

}
void gameBoard::moveTo(player& player, pos pos){
	if(canMoveTo(pos))
		player.moveTo(pos);
	else{
		logger::log("Cant move player %s to (%u, %u)\n", player.getName().c_str(), pos.x, pos.y);
	}
}
bool gameBoard::canMoveTo(pos pos) const {
	if (pos.x < width && pos.y < height)
		return true;
	return false;
}
pos gameBoard::getSize() const {
	return {width, height};
}
std::list<fruit>::const_iterator gameBoard::getFruitsListIteratorBegin() const {
	return fruitsList.begin();
}
std::list<fruit>::const_iterator gameBoard::getFruitsListIteratorEnd() const {
	return fruitsList.end();
}

void player::setScore(unsigned newScore) {
	score = newScore;
	logger::log("%s Scored! New score: %u\n", getName().c_str(), score);
}
player::player(pos pos, const std::string& name) : score(0), x(pos.x), y(pos.y), name(name) {
}

player::~player() {
}
pos player::getPos() const {
	return {x, y};
}
void player::moveTo(pos to) {
	x = to.x;
	y = to.y;
}
void player::charDirectionToPos(char direction, pos& pos) {
	switch (direction) {
		case 'L':
			pos = {x - 1, y};
			break;
		case 'R':
			pos = {x + 1, y};
			break;
		case 'U':
			pos = {x, y - 1};
			break;
		case 'D':
			pos = {x, y + 1};
			break;
		default:
			pos = {x, y};
			break;
	}
}