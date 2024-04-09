CC = gcc
CINC = -Iinclude
CFLAGS = -g -lJakestering

INC_DIR = include
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
BUILD_DIR = build

.PHONY: all
all: $(BIN_DIR)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $< -c $(CFLAGS) $(CINC) -o $@

$(OBJ_DIR)/flash.o: $(SRC_DIR)/flash.c
	$(CC) $< -c $(CFLAGS) $(CINC) -o $@

$(OBJ_DIR)/command.o: $(SRC_DIR)/command.c
	$(CC) $< -c $(CFLAGS) $(CINC) -o $@

$(BIN_DIR): always $(OBJ_DIR)/main.o $(OBJ_DIR)/flash.o $(OBJ_DIR)/command.o
	mkdir -p $(BIN_DIR)
	$(CC) $(OBJ_DIR)/main.o $(OBJ_DIR)/flash.o $(OBJ_DIR)/command.o $(CFLAGS) $(CINC) -o $@/flash


.PHONY: build

build: $(BUILD_DIR)

$(BUILD_DIR): create $(SRC_DIR)/main.c $(SRC_DIR)/flash.c $(SRC_DIR)/command.c
	$(CC) -fPIC -shared $(SRC_DIR)/main.c $(SRC_DIR)/flash.c $(SRC_DIR)/command.c $(CINC) $(CFLAGS) -o $@/libFlash.so

.PHONY: install
install:
	sudo cp $(INC_DIR)/* /usr/include
	sudo cp $(BUILD_DIR)/libFlash.so /usr/lib

.PHONY: uninstall
uninstall:
	sudo rm /usr/include/flash.h
	sudo rm /usr/include/commandHelper.h
	sudo rm /usr/lib/libFlash.so

.PHONY: clean
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR) $(BUILD_DIR)
	clear

.PHONY: always
always:
	mkdir -p $(OBJ_DIR)

.PHONY: create
create:
	mkdir -p $(BUILD_DIR)
