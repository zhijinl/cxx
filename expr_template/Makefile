
CXX=g++
CXXFLAGS=-msse2 -Wall -g -O3 -fopenmp -std=c++14 -Wno-deprecated-declarations
PROG=expr

all:
	$(CXX) $(CXXFLAGS) -o $(PROG) main.cc -I$(LOCAL_DIR)/include

.PHONY: clean

clean:
	rm -f *.o $(PROG)
