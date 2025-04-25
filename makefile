CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

all: edmons-karp fattest-path random-dfs

edmonds_karp: edmons-karp.cpp ford-fulkerson.h
	$(CXX) $(CXXFLAGS) -o $@ $<

fattest_path: fattest-path.cpp ford-fulkerson.h
	$(CXX) $(CXXFLAGS) -o $@ $<

random_dfs: random-dfs.cpp ford-fulkerson.h
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f edmons-karp fattest-path random-dfs *.o
