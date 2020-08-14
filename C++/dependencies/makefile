.PHONY: json_submodule

json_submodule:
	git submodule update --init --checkout

preCompiledHeader: json/single_include/nlohmann/json.hpp.gch

json/single_include/nlohmann/json.hpp.gch: json/single_include/nlohmann/json.hpp
	$(CXX) $(CXXFLAGS) $(DBG) $^ -o $@