#include "gameLogic.hpp"

#include <random>

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
	outFile = fopen("/dev/pts/1", "w");
	fprintf(outFile, "HaHa\n");
}

gameBoard::~gameBoard() {
	fclose(outFile);
}

void gameBoard::spawnFruit() {
	unsigned int x = randGen::GetUnsignedMod(width), y = randGen::GetUnsignedMod(height);
	fruitsList.emplace_back(x, y);
	fprintf(outFile, "spawned fruit at %u %u\n", x, y);
}
bool gameBoard::canMoveTo(unsigned int x, unsigned int y) const {
	if (x < width && y < height)
		return true;
	return false;
}
bool gameBoard::moveToAndReturnIfScored(unsigned int x, unsigned int y) {
	for(auto it = fruitsList.begin(); it != fruitsList.end(); ++it){
		if(it->x == x && it->y == y){
			fprintf(outFile, "colide with fruit, removing fruit and adding new\n");
			fruitsList.erase(it);
			spawnFruit();
			return 1;
		}
	}
	return 0;
}
void gameBoard::TryMoveTo(player& player, unsigned int x, unsigned int y) {
	if (canMoveTo(x, y)) {
		unsigned int score = moveToAndReturnIfScored(x, y);
		player.addScore(score);
		player.movedTo(x, y);
		fprintf(outFile, "moved to %u %u and scored %u\n", x, y, score);
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
}
player::player(unsigned int x, unsigned y) : x(x), y(y) {
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