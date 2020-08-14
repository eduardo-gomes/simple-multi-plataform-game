export CXX=g++-10
export CXXFLAGS=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -g -fanalyzer -fanalyzer-verbosity=3
export LIBS=-lncurses
export DBG=-Og -g

client:
	$(MAKE) -C client

server:
	$(MAKE) -C server

.PHONY: client server