CC = gcc
TARGET = compiled
SOURCES = k-parser.c token.c

default: 
	$(CC) $(SOURCES) -o $(TARGET)
	clear
	./$(TARGET)