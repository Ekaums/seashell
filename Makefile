# Define the compiler and target executable
CXX := g++
TARGET := bin/main

# Use pkg-config to get SDL2 compilation and linking flags
SDL2FLAGS := $(shell pkg-config --cflags sdl2)
SDL2LIBS := $(shell pkg-config --libs sdl2) -lSDL2_image -lSDL2_ttf

# Compilation flags
CXXFLAGS := -Wall -g -std=c++17 -Iinclude $(SDL2FLAGS)

# Source and object files
SRCDIR := src
BINDIR := bin
SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp, $(BINDIR)/%.o, $(SRCS))

# Build target executable
$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) -o $(TARGET) $(OBJS) $(SDL2LIBS)

# Rule to compile .cpp files into .o files
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(TARGET) $(BINDIR)/*.o
