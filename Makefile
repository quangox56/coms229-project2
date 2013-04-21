all: showgen sim-tui

showgen: showgen.o terrain.o common.o
	mkdir -p bin
	g++ -g obj/showgen.o obj/terrain.o obj/common.o -o bin/showgen 

sim-tui: sim-tui.o terrain.o common.o
	mkdir -p bin
	g++ -g obj/sim-tui.o obj/terrain.o obj/common.o -lncurses -o bin/sim-tui

terrain.o: src/terrain.cpp src/terrain.h
	mkdir -p obj
	g++ -c -g src/terrain.cpp -o obj/terrain.o

showgen.o: src/showgen.cpp 
	mkdir -p obj
	g++ -c -g src/showgen.cpp -o obj/showgen.o

sim-tui.o: src/sim-tui.cpp
	mkdir -p obj
	g++ -c -g src/sim-tui.cpp -o obj/sim-tui.o

common.o: src/common.h src/common.cpp
	mkdir -p obj
	g++ -c -g src/common.cpp -o obj/common.o

clean:
	rm -rf obj
	rm -rf bin
	rm -f core.*
	rm -rf test/generatedaut
	rm -rf test/showgenoutput

test: all
	python test/fuzzy.py test bin
