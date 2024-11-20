# Определяем переменные для путей
SRC_DIR = .
MODULES_DIR = ../../server/source/modules
BUILD_DIR = ../build
TARGET = unit

# Определяем компилятор и флаги компиляции
CXX = g++
CXXFLAGS = -std=c++11 -I/usr/include/UnitTest++
LDFLAGS = -L/usr/lib -lUnitTest++ -lcryptopp

# Получаем список всех файлов .cpp в директории modules и файл main.cpp
MODULES = $(wildcard $(MODULES_DIR)/*.cpp)
MAIN = $(SRC_DIR)/main.cpp

# Определяем все объектные файлы
OBJS = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(notdir $(MODULES) $(MAIN)))

# Цель по умолчанию
all: mkdir $(BUILD_DIR)/$(TARGET) clean

# Создание папки для объектных файлов и исполняемого файла
mkdir:
	mkdir -p $(BUILD_DIR)

# Сборка проекта
$(BUILD_DIR)/$(TARGET): $(OBJS)
	@$(CXX) -o $@ $^ $(LDFLAGS)
	@echo "BUILD SUCCESS!!!"

# Правило для компиляции объектного файла для main.cpp
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

# Правило для компиляции объектных файлов из modules
$(BUILD_DIR)/%.o: $(MODULES_DIR)/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Очистка сборки
clean:
	@rm -rf $(BUILD_DIR)/*.o
	@echo "CLEAN UP SUCCESS."

.PHONY: all clean 
