CC = gcc
CINC = -Iinclude
CFLAGS = -g -lJakestering

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

.PHONY: all
all: $(BIN_DIR)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $< -c $(CFLAGS) $(CINC) -o $@

$(OBJ_DIR)/utils.o: $(SRC_DIR)/utils.c
	$(CC) $< -c $(CFLAGS) $(CINC) -o $@

$(OBJ_DIR)/flash.o: $(SRC_DIR)/flash.c
	$(CC) $< -c $(CFLAGS) $(CINC) -o $@

$(OBJ_DIR)/command.o: $(SRC_DIR)/command.c
	$(CC) $< -c $(CFLAGS) $(CINC) -o $@

$(BIN_DIR): always $(OBJ_DIR)/main.o
	mkdir -p $(BIN_DIR)
	$(CC) $(OBJ_DIR)/main.o $(CFLAGS) $(CINC) -o $(BIN_DIR)/$@

.PHONY: clean
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
	clear

.PHONY: always
always:
	mkdir -p $(OBJ_DIR)
