all: mstdriver
  
mstdriver: msttest.cpp mst.h mst.cpp
	g++ -std=c++11 -o mstdriver msttest.cpp mst.cpp

clean:
	rm -f mstdriver
