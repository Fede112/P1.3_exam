CXX=c++
CXXFLAGS= -Wall -Wextra -std=c++11 -O3

DEBUG= -g -ggdb

SRC=main.cc
EXE=$(SRC:.cc=.x)

all: $(EXE)

%.x:%.cc
	$(CXX) $(CXXFLAGS) $< -o $@

debug: CXXFLAGS += $(DEBUG)
debug: all


clean:
	rm -rf *~ $(EXE)

.PHONY: clean all debug

