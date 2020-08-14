export CXX=g++-10
export CXXFLAGS=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -g -fanalyzer -fanalyzer-verbosity=3
export INCLUDE="-I$(abspath dependencies/json/single_include)"
export LIBS=-lncurses
export DBG=-Og -g

client: preCompiledHeader
	$(MAKE) -C client

server: preCompiledHeader
	$(MAKE) -C server

preCompiledHeader:
	$(MAKE) -C dependencies preCompiledHeader

.PHONY: client server preCompiledHeader