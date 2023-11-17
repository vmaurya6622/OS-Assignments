EXE=vector matrix

all: $(EXE) clean

%: %.cpp
	g++ -O3 -std=c++11 -o $@ $^ -lpthread
	./$@

clean:
	rm -f $(EXE)
