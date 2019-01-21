CXX=c++
CXXFLAGS= -Wall -Wextra -std=c++11 -O3

DEBUG= -g -ggdb
 
SRC=main.cc
EXE=$(SRC:.cc=.x)

all: $(EXE)

%.x:%.cc
	$(CXX) $(CXXFLAGS) $< -o $@

debug: CXXFLAGS += $(DEBUG) -DTEST
debug: all
debug: 
	valgrind ./${EXE} 

test: CXXFLAGS += -DTEST
test: all

benchmark: CXXFLAGS += -DBENCHMARK
benchmark: all
benchmark:
	./runner.sh

plot:
	./plotter.sh

clean:
	rm -rf *~ $(EXE) *.png ./DATA/*

.PHONY: clean all debug

