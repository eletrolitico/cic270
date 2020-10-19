# Name of the project
PROJ_NAME=Application

# .c files
#C_SOURCE=$(wildcard ./*.cpp)
SOURCES := $(shell find . -name '*.cpp')

# Object files
#OBJ=$(subst .cpp,.o,$(subst ./objects/,$(C_SOURCE)))
OBJ=$(addprefix objects/,$(subst .cpp,.o,$(notdir $(SOURCES))))

# Compiler and linker
CC=g++

# Flags for compiler
CC_FLAGS = -g -c -Wall -I ./util -I ./lib

LDFLAGS = -lglut -lGLEW -lGL -lopenal -z muldefs

# Command used at clean target
RM = rm -rf

# Compilation and linking
all: objFolder $(PROJ_NAME) 
	@ echo 'Done!'

$(PROJ_NAME): $(OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@ $(LDFLAGS)
	@ echo 'Finished building binary: $@'
	@ echo ' '

objects/%.o: ./%.cpp
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '
objects/%.o: ./util/%.cpp
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '
	

objects/Application.o: ./Application.cpp
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '

objFolder:
	@ mkdir -p objects

clean:
	@ $(RM) ./objects/*.o $(PROJ_NAME) *~
	@ rmdir objects

.PHONY: all clean