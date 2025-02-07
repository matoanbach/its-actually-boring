# Makefile for Assignment 3 - Performance Testing of a Custom Database

# Compiler
CC = g++

# Compiler flags:
# -I. : Include current directory for header files
# -Wall : Enable all warnings
# -std=c++11 : Use C++11 standard
CFLAGS = -I. -Wall -std=c++11

# List all source files
FILES = AVLTree.cpp timer.cpp AVLTestSuite.cpp

# Name of the final executable
TARGET = avlTree

# Rule to build the executable from the source files
$(TARGET): $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $(TARGET)

# "all" target builds the executable
all: $(TARGET)

# Clean target: remove the executable and any object files
clean:
	rm -f $(TARGET) *.o
