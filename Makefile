CC = gcc
TARGET = compiled
SOURCES = parser.c token.c

default: 
	$(CC) $(SOURCES) -o $(TARGET)
	./$(TARGET)

c:
	$(CC) $(SOURCES) -o $(TARGET)
	clear
	./$(TARGET)
