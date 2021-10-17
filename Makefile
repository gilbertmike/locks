SRC := $(wildcard src/*.cpp)
BIN := $(patsubst src/%.cpp, bin/%.out, $(SRC))

run: $(BIN)
	for x in bin/*; do ./$$x; done

all: $(BIN)

bin/%.out: src/%.cpp
	g++ $< -o $@ -O3 -pthread

clean:
	rm bin/*
