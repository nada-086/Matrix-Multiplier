CC=gcc
TARGET=matMultp

all:
	$(CC) main.c -o $(TARGET) -lpthread
	
clean:
	rm $(TARGET)
	
