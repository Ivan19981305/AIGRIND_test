CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -I include -I include/Enemy

TARGET = game

all: $(TARGET)

$(TARGET): $(wildcard src/*.cpp) $(wildcard src/Enemy/*.cpp)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f $(TARGET)