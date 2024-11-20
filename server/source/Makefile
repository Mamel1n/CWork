# Определяем переменные для путей
SRC_DIR = .
MODULES_DIR = $(SRC_DIR)/modules
TARGET = server
BUILD_DIR = ../build

# Определяем компилятор и флаги компиляции
CXX = g++
CXXFLAGS = -std=c++11 -lcryptopp

# Получаем список всех файлов .cpp в директории modules
MODULES = $(wildcard $(MODULES_DIR)/*.cpp)
MAIN = $(SRC_DIR)/main.cpp

# Определяем все объектные файлы
OBJS = $(MODULES:.cpp=.o) $(MAIN:.cpp=.o)

# Цель по умолчанию
all: $(BUILD_DIR)/$(TARGET) clean

# Сборка проекта
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)
	@echo "BUILD SUCCESS!!!"

# Правило для компиляции объектного файла main.cpp
$(MAIN:.cpp=.o): $(MAIN)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Правило для компиляции объектных файлов из modules
$(MODULES_DIR)/%.o: $(MODULES_DIR)/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Очистка сборки
clean:
	@rm -f $(SRC_DIR)/*.o $(MODULES_DIR)/*.o $(SRC_DIR)/$(TARGET)
	@echo "CLEAN UP SUCCESS."

.PHONY: all clean
