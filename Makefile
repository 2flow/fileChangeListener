
CXX=clang++
CXXFLAGS=-std=c++17 -Wall -pedantic
OBJ = src/main.cpp src/FileWatcher.cpp
INC_DIR = include

_OBJ = $(patsubst /%.cpp,$(bin/%.o),$(OBJ))
_INC_DIR = $(patsubst %,-I%,$(INC_DIR))

display_sync: $(_OBJ)
	$(_INC_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(_INC_DIR)

bin/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f display_sync

.PHONY: clean