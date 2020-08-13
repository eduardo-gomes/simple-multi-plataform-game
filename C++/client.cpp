#include <curses.h>

#include <map>
#include <string>

#include "gameLogic.hpp"
#include "log.hpp"

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

	void addNewPlayer(const std::string & name){
		pos newPos = {2, 2};//game.randomEmptyPos();
		player newPlayer(newPos.x, newPos.y, name);
		playersList.emplace(name, std::move(newPlayer));
	}

	void removePlayer(const std::string& name){
		playersList.erase(name);
	}
};

char inputToDirectionChar(char input) {
	switch (input) {
		case 'a':
			return 'L';
		case 'd':
			return 'R';
		case 'w':
			return 'U';
		case 's':
			return 'D';
		default:
			return 0;
	}
}

class clientGame {
   protected:
	gameBoard localGame;  //(40, 20);
	player localPlayer;	  //(2, 2);

	clientGame(unsigned int width, unsigned int height) : localGame(width, height), localPlayer(0, 0) {
		logger::log("Initialized clientGame with %u x %u\n", width, height);
	}

	void processInputDirection(char direction) {
		pos newPos;
		localPlayer.charDirectionToPos(direction, newPos);
		localGame.TryMoveToAndScore(localPlayer, newPos.x, newPos.y);
	}
	void handleInput(char input) {
		char inputDirection = inputToDirectionChar(input);
		processInputDirection(inputDirection);
	}
	void recvFromServer() {
	}

	virtual void display();

	void gameLoop() {
		bool continueGame = 1;
		localGame.spawnFruit();
		while (continueGame) {
			clear();
			display();
			char input = getch();
			if (input == 27)
				continueGame = false;
			else
				handleInput(input);
			recvFromServer();
		}
	}
};
class ncurses : public clientGame {
#define BORDER_PAIR 1
#define PLAYER_PAIR 2
#define FRUITS_PAIR 3
	void dispFruits() {
		attron(COLOR_PAIR(FRUITS_PAIR));

		for (auto it = localGame.getFruitsListIteratorBegin(); it != localGame.getFruitsListIteratorEnd(); ++it)
			mvaddch((int)it->y + 1, (int)it->x + 1, '#');

		attroff(COLOR_PAIR(FRUITS_PAIR));
	}

	void drawnBox(int x1, int y1, int x2, int y2) {
		mvhline(y1, x1, 0, x2 - x1);
		mvhline(y2, x1, 0, x2 - x1);
		mvvline(y1, x1, 0, y2 - y1 + 1);
		mvvline(y1, x2, 0, y2 - y1 + 1);
	}
	void drawGameBoard() {
		attron(COLOR_PAIR(BORDER_PAIR));

		pos size = localGame.getSize();

		drawnBox(0, 0, (int)size.x + 1, (int)size.y + 1);

		attroff(COLOR_PAIR(BORDER_PAIR));
	}

	void drawnPlayer() {
		attron(COLOR_PAIR(PLAYER_PAIR));

		pos pos = localPlayer.getPos();

		move((int)pos.y + 1, (int)pos.x + 1);
		printw("P");

		attroff(COLOR_PAIR(PLAYER_PAIR));
	}

	void displayGame() {
		drawGameBoard();
		dispFruits();
		drawnPlayer();

		move(-1, -1);
		printw("");
		refresh();
	}

   public:
	ncurses(unsigned int width, unsigned int height) : clientGame(width, height) {
		initscr();
		curs_set(0);

		start_color();
		use_default_colors();
		init_pair(BORDER_PAIR, COLOR_WHITE, COLOR_WHITE);  //BORDER
		init_pair(PLAYER_PAIR, COLOR_YELLOW, -1);		   //Player
		init_pair(FRUITS_PAIR, COLOR_GREEN, COLOR_GREEN);  //Fruit

		noecho();
	}
	~ncurses() {
		endwin();
	}

	void display() override {
		displayGame();
	}
};

void clientGame::display() {
}

int main() {
	server localServer;
	pos gameSize = localServer.getSize();
	std::string playerName("player1");
	localServer.addNewPlayer(playerName);
	ncurses localClient(gameSize.x, gameSize.y);
	localClient.display();
	getch();
}