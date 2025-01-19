# Makefile

CXX = g++
CXXFLAGS = -std=c++17 -I../libs

SRC = main.cpp
OUT = app

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ -L../libs -ljournal

run: $(OUT)
	LD_LIBRARY_PATH=../libs ./$(OUT)

# Clean up
clean:
	rm -f $(OUT)
