SRC += src/main.c
FLG += -std=c99
FLG += -Wall
LNK += -lraylib
BIN += raylib_raycaster

build:
	gcc $(SRC) $(FLG) $(LNK) -o $(BIN)

run:
	./$(BIN)

