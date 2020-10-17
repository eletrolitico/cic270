CC = g++

LIBS = -lglut -lGLEW -lGL -lopenal
FLAGS = -g -Wall
INCLUDE = -I util -I lib

all: Application.cpp 
	$(CC) $(FLAGS) $(INCLUDE) ./util/*.cpp ./*.cpp  -o Application $(LIBS) -z muldefs

clean:
	rm -f Application
