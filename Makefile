SRC := $(wildcard src/*.cpp)
BIN := $(patsubst src/%.cpp, bin/%.out, $(SRC))

all: $(BIN)

bin/%.out: src/%.cpp
	g++ $< -o $@ -O3 -pthread
