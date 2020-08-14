#include <string>
#include <list>

class fruit {
   public:
	unsigned int x, y;
	fruit(unsigned int x, unsigned int y);	//Need to anounce to clients
	~fruit();								//Need to anounce to clients
};

class player{
std::list<fruit> fruitsList;  //to be optimized
const unsigned int width, height;
};

class gameBoard{
unsigned int score, x, y;
std::string name;
};

struct pos {
	unsigned int x, y;
};