CC := g++

CFLAGS := -std=c++11 -Wall

LNFLAG := -lncurses

SRCFILES := $(wildcard src/*.cpp)

all: $(SRCFILES:src/%.cpp=obj/%.o)
	$(CC) $(CFLAGS) obj/*.o $(LNFLAG) -o bin/saida

obj/%.o : src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf obj/*
	rm -rf bin/*

run:
	bin/saida