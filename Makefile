CXX = g++
CXXFLAGS = -std=c++11 -Wall
TARGET = lab7
SOURCES = lab7.cpp
HEADERS = lab7.h

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

.PHONY: all clean