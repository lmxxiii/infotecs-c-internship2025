# Makefile

CXX = g++
CXXFLAGS = -std=c++17 -fPIC -shared

SRC = journallib.cpp
OUT = journallib.so

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OUT)
