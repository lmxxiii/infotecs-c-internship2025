# Makefile

CXX = g++
CXXFLAGS = -std=c++17 -fPIC -shared

SRC = libjournal.cpp
OUT = libjournal.so

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OUT)
