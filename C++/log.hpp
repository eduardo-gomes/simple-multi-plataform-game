#include <cstdio>
#include <memory>

class logger {
	FILE* outPut;
	static std::unique_ptr<logger> instance;
	static logger& getInstance();
	public:
	logger();
	~logger();
	static int log(const char *format, ...);
};