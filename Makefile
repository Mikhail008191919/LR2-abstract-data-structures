CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2
SOURCES = main.cpp common.cpp $(wildcard structures/*.cpp) $(wildcard tasks/*.cpp)
HEADERS = $(wildcard *.h) $(wildcard structures/*.h) $(wildcard tasks/*.h)

all: lr2

lr2: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o lr2

test: lr2
	python3 tests/run_tests.py

clean:
	rm -f lr2

.PHONY: all test clean
