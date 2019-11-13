CFLAGS = -std=c++11 -c -Wall -I./include/ -I./include/Piece/ -I./include/Board/ -I./include/Resolver/ -I./include/ThreadPool/ 

bin/tetravex : src/main.cpp build/Piece/piece.o build/Board/board.o build/Resolver/resolver.o build/ThreadPool/threadPool.o build/ThreadPool/threadPoolSpecial.o build/ThreadPool/threadPoolSpecial2.o
	g++ $^ -o bin/tetravex -std=c++11 -Wall -I./include/ -I./include/Piece/ -I./include/Board/ -I./include/Resolver/ -I./include/ThreadPool/   -lsfml-graphics -lsfml-window -lsfml-system -pthread -O3


build/Piece/piece.o : src/Piece/piece.cpp include/Piece/piece.h
	g++ src/Piece/piece.cpp -o build/Piece/piece.o $(CFLAGS) -O3

build/Board/board.o : src/Board/board.cpp include/Board/board.h
	g++ src/Board/board.cpp -o build/Board/board.o $(CFLAGS) -O3

build/Resolver/resolver.o : src/Resolver/resolver.cpp include/Resolver/resolver.h
	g++ src/Resolver/resolver.cpp -o build/Resolver/resolver.o $(CFLAGS) -O3

build/ThreadPool/threadPool.o : src/ThreadPool/threadPool.cpp include/ThreadPool/threadPool.h
	g++ src/ThreadPool/threadPool.cpp -o build/ThreadPool/threadPool.o $(CFLAGS) -O3

build/ThreadPool/threadPoolSpecial.o : src/ThreadPool/threadPoolSpecial.cpp include/ThreadPool/threadPoolSpecial.h
	g++ src/ThreadPool/threadPoolSpecial.cpp -o build/ThreadPool/threadPoolSpecial.o $(CFLAGS) -O3

build/ThreadPool/threadPoolSpecial2.o : src/ThreadPool/threadPoolSpecial2.cpp include/ThreadPool/threadPoolSpecial2.h
	g++ src/ThreadPool/threadPoolSpecial2.cpp -o build/ThreadPool/threadPoolSpecial2.o $(CFLAGS) -O3

clean:
	find . -type f -name '*.o' -delete


