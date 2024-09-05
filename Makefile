CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

TARGET = game

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f $(TARGET)