CXX := g++
CXXFLAGS := -std=c++17 -Iinclude -Wall -Wextra -O2

SRCDIR := src
gAMEDIR := $(SRCDIR)/games
GAMESRC := $(wildcard $(GAMEDIR)/*.cpp)
MENU := $(SRCDIR)/menu.cpp
TARGET := game.out

all: $(TARGET)

$(TARGET): $(MENU) $(GAMESRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) $(GAMESRC:.cpp=.o) $(MENU:.cpp=.o)

.PHONY: all clean
