#CXX=g++-10
#CXXFLAGS=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -g -fanalyzer -fanalyzer-verbosity=3
#LIBS=-lncurses
#DBG=-Og -g

server.out: server.cpp
		$(CXX) $^ $(CXXFLAGS) $(LIBS) $(DBG) -o $@
