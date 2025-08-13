SRC += src/*.c
FLG += -std=c99
FLG += -Wall
LNK += -lSDL3
BIN += raycaster

build:
	gcc $(SRC) $(FLG) $(LNK) -o $(BIN)

run:
	./$(BIN)

