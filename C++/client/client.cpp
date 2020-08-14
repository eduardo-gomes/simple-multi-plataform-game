#include <curses.h>

#include <map>
#include <string>

#include "../log.hpp"
#include "gameLogic.hpp"
//#include "types.hpp"

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

	clientGame(unsigned int width, unsigned int height) : localGame(width, height), localPlayer(pos({0, 0}), "LocalPlayer") {
		logger::log("Initialized clientGame with %u x %u\n", width, height);
	}

	void movePlayerInDirection(char direction) {
		pos newPos;
		localPlayer.charDirectionToPos(direction, newPos);
		localGame.moveTo(localPlayer, newPos);
	}
	void handleInput(char input) {
		char inputDirection = inputToDirectionChar(input);
		sendMoveDirectionToServer(inputDirection);
	}

	virtual void display();

	void gameLoop() {
		bool continueGame = 1;
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
	pos gameSize = {10, 10};
	std::string playerName("player1");
	ncurses localClient(gameSize.x, gameSize.y);
	localClient.display();
	getch();
}