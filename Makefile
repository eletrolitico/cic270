CC = g++

GLLIBS = -lglut -lGLEW -lGL -lopenal -pthread

all: Application.cpp 
	$(CC) -g -Wall -I util -I lib ./*.cpp ./util/*.cpp -o Application $(GLLIBS)

clean:
	rm -f Application
