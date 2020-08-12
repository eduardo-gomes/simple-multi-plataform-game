#include "log.hpp"

#include <stdexcept>
#include <cstdarg>

logger::logger(){
	outPut = fopen("/dev/pts/0", "w");
	if(outPut == nullptr) throw std::runtime_error("Can't open logfile");
}
logger::~logger(){
	fclose(outPut);
}
std::unique_ptr<logger> logger::instance;
logger& logger::getInstance(){
	if(!instance)
		instance = std::make_unique<logger>();
	return *instance;
}

int logger::log(const char* format, ...){
	va_list args;
	va_start(args, format);

	int ret = vfprintf(getInstance().outPut, format, args);  //this dont verify if file is valid

	va_end(args);
	return ret;
}