#CXX=g++-10
#CXXFLAGS=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -g -fanalyzer -fanalyzer-verbosity=3
#LIBS=-lncurses
#DBG=-Og -g

server.out: server.cpp gameLogic.cpp ../log.cpp json_conversions.o
	$(CXX) $^ $(CXXFLAGS) $(INCLUDE) $(LIBS) $(DBG) -o $@

json_conversions.o: json_conversions.cpp
	$(CXX) $< $(CXXFLAGS) $(INCLUDE) $(LIBS) $(DBG) -c -o $@
