all: showgen sim-tui sim-gui

showgen: showgen.o terrain.o common.o conwaysGOL.o briansBrain.o wireWorld.o langtonsAnts.o
	mkdir -p bin
	g++ -g obj/showgen.o obj/terrain.o obj/common.o obj/conwaysGOL.o obj/briansBrain.o obj/wireWorld.o obj/langtonsAnts.o -o bin/showgen 

sim-tui: sim-tui.o terrain.o common.o conwaysGOL.o briansBrain.o wireWorld.o langtonsAnts.o
	mkdir -p bin
	g++ -g obj/sim-tui.o obj/terrain.o obj/common.o obj/conwaysGOL.o obj/briansBrain.o obj/wireWorld.o obj/langtonsAnts.o -lncurses -o bin/sim-tui

sim-gui: src/sim-gui/sim-gui.cpp src/sim-gui/grid.cpp src/sim-gui/optionsdialog.cpp terrain.o common.o conwaysGOL.o briansBrain.o wireWorld.o langtonsAnts.o
	mkdir -p bin
	cd src/sim-gui; qmake-qt4 -project; qmake-qt4 sim-gui.pro; make
	mv src/sim-gui/sim-gui ./bin/
	rm -f src/sim-gui/*.o

terrain.o: src/terrain.cpp src/terrain.h
	mkdir -p obj
	g++ -c -g src/terrain.cpp -o obj/terrain.o

conwaysGOL.o: src/conwaysGOL.cpp src/conwaysGOL.h
	mkdir -p obj
	g++ -c -g src/conwaysGOL.cpp -o obj/conwaysGOL.o

briansBrain.o: src/briansBrain.cpp src/briansBrain.h
	mkdir -p obj
	g++ -c -g src/briansBrain.cpp -o obj/briansBrain.o

wireWorld.o: src/wireWorld.cpp src/wireWorld.h
	mkdir -p obj
	g++ -c -g src/wireWorld.cpp -o obj/wireWorld.o
	
langtonsAnts.o: src/langtonsAnts.cpp src/langtonsAnts.h
	mkdir -p obj
	g++ -c -g src/langtonsAnts.cpp -o obj/langtonsAnts.o

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
	rm -f src/sim-gui/moc_*
	rm -f src/sim-gui/Makefile
	rm -f src/sim-gui/sim-gui.pro

test: all
	cd unittest; ./runtests.sh
