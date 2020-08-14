#include "gameLogic.hpp"

#include <random>

#include "log.hpp"

namespace randGen {
std::default_random_engine ranEng((uint_fast32_t)time(NULL));
unsigned int GetUnsignedMod(unsigned int mod) {
	unsigned int Random = ranEng() - ranEng.min();
	Random %= mod;
	return Random;
}
}  // namespace randGen

fruit::fruit(unsigned int x, unsigned int y) : x(x), y(y) {
}  //Need to anounce to clients
fruit::~fruit() {
}  //Need to anounce to clients

//std::vector<fruit> **fruitsVectorBoard;//2d array of vectors
gameBoard::gameBoard(unsigned int width, unsigned int height) : width(width), height(height) {
	logger::log("HaHa\n");
}

void gameBoard::spawnFruit() {
	unsigned int x = randGen::GetUnsignedMod(width), y = randGen::GetUnsignedMod(height);
	fruitsList.emplace_back(x, y);
	logger::log("spawned fruit at %u %u\n", x, y);
}
bool gameBoard::canMoveTo(unsigned int x, unsigned int y) const {
	if (x < width && y < height)
		return true;
	return false;
}
bool gameBoard::moveToAndReturnIfScored(unsigned int x, unsigned int y) {
	for (auto it = fruitsList.begin(); it != fruitsList.end(); ++it) {
		if (it->x == x && it->y == y) {
			logger::log("colide with fruit, removing fruit and adding new\n");
			fruitsList.erase(it);
			spawnFruit();
			return 1;
		}
	}
	return 0;
}
void gameBoard::TryMoveToAndScore(player& player, unsigned int x, unsigned int y) {
	if (canMoveTo(x, y)) {
		unsigned int score = moveToAndReturnIfScored(x, y);
		player.addScore(score);
		player.movedTo(x, y);
		logger::log("moved to %u %u\n", x, y);
	}
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

void player::addScore(unsigned add) {
	score += add;
	if (add) logger::log("Scored! New score: %u\n", score);
}
player::player(unsigned int x, unsigned int y) : score(0), x(x), y(y), name("local") {
}
player::player(unsigned int x, unsigned int y, const std::string& name) : score(0), x(x), y(y), name(name) {
}

player::~player() {
}
pos player::getPos() const {
	return {x, y};
}
void player::movedTo(unsigned int toX, unsigned int toY) {
	x = toX;
	y = toY;
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