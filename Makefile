# Variáveis
CXX = g++
CXXFLAGS = -std=c++17 -I. -I./data_structures
SOURCES = main.cpp $(wildcard data_structures/*/*.cpp)
TARGET = myProg

# Regra principal
all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

# Limpar o executável
clean:
	rm -f $(TARGET)