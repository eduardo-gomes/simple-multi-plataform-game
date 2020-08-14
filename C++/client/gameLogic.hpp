#include <list>
#include <string>

struct pos {
	unsigned int x, y;
};

class fruit {
   public:
	unsigned int x, y;
	explicit fruit(pos);	//Need to anounce to clients
	~fruit();								//Need to anounce to clients
};

class gameBoard {
	std::list<fruit> fruitsList;  //to be optimized

	const unsigned int width, height;

   public:
	gameBoard(unsigned int width, unsigned int height);
	~gameBoard() = default;
	void addFruit(pos);
	void rmFruit(pos);
	std::list<fruit>::const_iterator getFruitsListIteratorBegin() const;
	std::list<fruit>::const_iterator getFruitsListIteratorEnd() const;
	pos getSize() const;
	bool canMoveTo(pos pos) const;
	void moveTo(player &player, pos);
};

class player {
	unsigned int score, x, y;
	std::string name;

   public:
	const std::string & getName();
	void setScore(unsigned set);
	void moveTo(pos);
	player(pos, const std::string &name);
	~player();
	pos getPos() const;
	void charDirectionToPos(char direction, pos &pos);
};