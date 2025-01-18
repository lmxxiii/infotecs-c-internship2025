# Makefile

CXX = g++
CXXFLAGS = -I../libs

SRC = main.cpp
OUT = app

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) -o $@ $^ -L../libs -ljournal

# Clean up
clean:
	rm -f $(OUT)
