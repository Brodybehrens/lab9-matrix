CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
TARGET := lab9
SRC := main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

run: $(TARGET)
	./$(TARGET) sample_input.txt

clean:
	rm -f $(TARGET)

.PHONY: all run clean
