#include <list>

class player;

struct pos {
	unsigned int x, y;
};

class fruit {

   public:
	unsigned int x, y;
	fruit(unsigned int x, unsigned int y);	//Need to anounce to clients
	~fruit();								//Need to anounce to clients
};

class gameBoard {
	std::list<fruit> fruitsList;  //to be optimized

	const unsigned int width, height;

   public:
	gameBoard(unsigned int width, unsigned int height);
	~gameBoard();
	void spawnFruit();
	bool canMoveTo(unsigned int x, unsigned int y) const;
	bool moveToAndReturnIfScored(unsigned int x, unsigned int y);
	std::list<fruit>::const_iterator getFruitsListIteratorBegin() const;
	std::list<fruit>::const_iterator getFruitsListIteratorEnd() const;
	pos getSize() const;
	void TryMoveToAndScore(player &player, unsigned int x, unsigned int y);
};

class player {
	unsigned int score, x, y;
	void addScore(unsigned add);
	void movedTo(unsigned int x, unsigned int y);

   public:
	player(unsigned x, unsigned y);
	~player();
	pos getPos() const;
	void charDirectionToPos(char direction, pos &pos);
	friend void gameBoard::TryMoveToAndScore(player &player, unsigned int x, unsigned int y);
};