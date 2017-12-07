CC = gcc
SRC = $(wildcard src/*.c)
EXE = myMOVIE

all: dir output/$(EXE)

dir:
    mkdir -p output

output/$(EXE): $(SRC)
    $(CC) -o $@ $^

run: output/$(EXE)
    cd output; ./$(EXE);

clean:
    rm -f output/*
