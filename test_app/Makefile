.PHONY: all, clean

INC_DIR := ./inc
SRC_DIR := ./src
APP_NAME := "test_app"

CC := g++
FLAGS := -Wall -lpthread

all:
	$(CC) main.cpp $(SRC_DIR)/*.cpp -I$(INC_DIR) -o $(APP_NAME)

clean:
	rm -rf $(APP_NAME)