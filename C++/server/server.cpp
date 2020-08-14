#include <map>
#include <string>

#include "gameLogic.hpp"
#include "../log.hpp"

class server {
	gameBoard game;
	std::map<std::string, player> playersList;

   public:
	server() : game(40, 20) {
		logger::log("Initialized gameServer \n");
	}

	void processInputDirection(player& player, char direction) {
		pos newPos;
		player.charDirectionToPos(direction, newPos);
		game.TryMoveToAndScore(player, newPos.x, newPos.y);
	}

	void recvDirectionFromPlayer(const std::string& who, char dire) {
		std::map<std::string, player>::iterator player = playersList.find(who);
		if (player != playersList.end())
			processInputDirection(player->second, dire);
	}

	pos getSize() {
		return game.getSize();
	}

	void addNewPlayer(const std::string& name) {
		pos newPos = {2, 2};  //game.randomEmptyPos();
		player newPlayer(newPos.x, newPos.y, name);
		playersList.emplace(name, std::move(newPlayer));
	}

	void removePlayer(const std::string& name) {
		playersList.erase(name);
	}
};

int main(){
	
}
