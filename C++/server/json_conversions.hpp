#include <vector>

#include "gameLogic.hpp"

enum MessageType { MOVMENT,
				   PLAYER_SET,
				   REMOVE,
				   DISCONNECT,
				   FRUIT,
				   INITIALIZE };

struct playerSetMsg {
	unsigned int score;
	pos position;
	std::string name, id;
	playerSetMsg(const player &);
	playerSetMsg();
};

struct playerMoveRequest {
	std::string name, id;
};

struct playerRmv {
	std::string id;
};

struct disconnectionRequest {
	std::string id;
};

struct fruitMsg {
	bool add;
	pos position;
};

struct initialize {
	pos size;
	std::vector<std::pair<std::string, player>> playersList;
	std::vector<fruit> fruitsList;
};

std::string toJsonToString(const playerSetMsg &);
playerSetMsg fromJsonToString(const std::string &);