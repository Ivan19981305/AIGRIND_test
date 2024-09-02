CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -lsfml-graphics -lsfml-window -lsfml-system

TARGET = game

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)