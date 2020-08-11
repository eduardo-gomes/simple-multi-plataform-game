#include <curses.h>

#include "gameLogic.hpp"

gameBoard game(16, 9);
player localPlayer(2, 2);

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
void processInputDirection(char direction) {
	pos newPos;
	localPlayer.charDirectionToPos(direction, newPos);
	game.TryMoveTo(localPlayer, newPos.x, newPos.y);
}
void processInput(char input) {
	char inputDirection = inputToDirectionChar(input);
	processInputDirection(inputDirection);
}
#define BORDER_PAIR 1
#define PLAYER_PAIR 2
#define FRUITS_PAIR 3
void dispFruits() {
	attron(COLOR_PAIR(FRUITS_PAIR));

	for(auto it = game.getFruitsListIteratorBegin(); it != game.getFruitsListIteratorEnd(); ++it)
		mvaddch(it->y + 1, it->x + 1, '#');

	attroff(COLOR_PAIR(FRUITS_PAIR));
}

void drawnBox(int x1, int y1, int x2, int y2) {
	mvhline(y1, x1, 0, x2-x1);
	mvhline(y2, x1, 0, x2-x1);
	mvvline(y1, x1, 0, y2-y1 + 1);
	mvvline(y1, x2, 0, y2-y1 + 1);
}
void drawGameBoard() {
	attron(COLOR_PAIR(BORDER_PAIR));

	pos size = game.getSize();

	drawnBox(0, 0, size.x + 1, size.y + 1);

	attroff(COLOR_PAIR(BORDER_PAIR));
}

void drawnPlayer() {
	attron(COLOR_PAIR(PLAYER_PAIR));

	pos pos = localPlayer.getPos();

	move(pos.y + 1, pos.x + 1);
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

int main() {
	initscr();
	curs_set(0);

	start_color();
	use_default_colors();
	init_pair(BORDER_PAIR, COLOR_WHITE, COLOR_WHITE);  //BORDER
	init_pair(PLAYER_PAIR, COLOR_YELLOW, -1);		   //Player
	init_pair(FRUITS_PAIR, COLOR_GREEN, COLOR_GREEN);  //Fruit

	noecho();

	bool continueGame = 1;
	game.spawnFruit();
	while (continueGame) {
		clear();
		displayGame();
		char input = getch();
		if (input == 27)
			continueGame = false;
		else
			processInput(input);
	}

	endwin();
}