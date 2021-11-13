LIBS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
ifeq ($(OS),Windows)
	CXX := i686-w64-mingw32-g++.exe
else
	CXX := g++
endif
BIN= ./
SRC_DIR := .
OBJ_DIR := build/
SRC_FILES := $(wildcard *.cpp)
OBJ_FILES :=  $(patsubst %.cpp,%.o,$(SRC_FILES))
LDFLAGS :=
CPPFLAGS :=
CXXFLAGS := -std=gnu++14

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

%.o: %.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

all: prog

clean:
	rm *.o prog
prog: $(OBJ_FILES)
	$(CXX) $(LDFLAGS) -o $(BIN)$@ $^ $(LIBS)
