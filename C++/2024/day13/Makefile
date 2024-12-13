run: solution
	./solution input.txt

test: solution
	./solution test.txt

solution: solution.cpp
	clang++ -std=c++20 -o solution solution.cpp -I/opt/homebrew/Cellar/z3/4.13.3/include -L/opt/homebrew/Cellar/z3/4.13.3/lib/ -lz3
