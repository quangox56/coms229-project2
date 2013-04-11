all: showgen

showgen: showgen.o terrain.o
	mkdir -p bin
	g++ obj/showgen.o obj/terrain.o -o bin/showgen

terrain.o: src/terrain.cpp src/terrain.h src/common.h
	mkdir -p obj
	g++ -c src/terrain.cpp src/terrain.h src/common.h -o obj/terrain.o

showgen.o: src/showgen.cpp src/common.h
	mkdir -p obj
	g++ -c src/showgen.cpp src/common.h -o obj/showgen.o

clean:
	rm -rf obj
	rm -rf bin
