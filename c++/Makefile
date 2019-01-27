CXX=c++
CXXFLAGS= -I./include -Wall -Wextra -std=c++11 -O3
LDFLAGS=
DEBUG= -g -ggdb
 
EXE=main.x
OBJS=main.o src/binarytree.o 

default: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $^ -o $@  $(LDFLAGS)

%.o: %.cc
	$(CXX) -c $< -o $@  $(CXXFLAGS) 

main.o: include/binarytree.h include/iterator.h 
src/binarytree.o: include/binarytree.h include/iterator.h

debug: CXXFLAGS += $(DEBUG) -DTEST
debug: default
debug: 
	valgrind ./${EXE} 

test: CXXFLAGS += -DTEST
test: default
	./main.x

benchmark: CXXFLAGS += -DBENCHMARK
benchmark: default
benchmark:
	./runner.sh

plot:
	./plotter.sh

clean:
	rm -rf $(OBJS) *~ $(EXE) *.png ./DATA/*

.PHONY: default debug test benchmark clean
