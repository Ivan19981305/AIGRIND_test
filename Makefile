# Настройки компилятора и линковщика
CXX = g++
CXXFLAGS = -Wall -Wextra -Iinclude -std=c++17
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Имя исполняемого файла
TARGET = game

# Список объектных файлов
OBJECTS = src/main.o src/Player.o src/Bullet.o src/Enemy.o src/Bird.o src/Bomber.o src/Fighter.o src/Meteor.o

# Правило для сборки исполняемого файла
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Правила для компиляции исходных файлов в объектные
src/%.o: src/%.cpp include/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Правило для очистки проекта
clean:
	rm -f $(TARGET) $(OBJECTS)

# Правило по умолчанию
all: $(TARGET)