main: main.o parser.o board.o block.o cell.o trie.o window.o
	g++ -L/usr/X11R6/lib main.o parser.o board.o block.o cell.o trie.o window.o -lX11 -o quadris

main.o: main.cc include/parser.h
	g++ -L/usr/X11R6/lib main.cc -lX11 -c

parser.o: include/parser.h parser.cc include/board.h include/trie.h
	g++ -L/usr/X11R6/lib parser.cc -lX11 -c

board.o: include/board.h board.cc include/cell.h include/block.h
	g++ -L/usr/X11R6/lib board.cc -lX11 -c

cell.o: include/cell.h cell.cc include/window.h
	g++ -L/usr/X11R6/lib cell.cc -lX11 -c

block.o: include/block.h block.cc include/PRNG.h include/window.h
	g++ -L/usr/X11R6/lib block.cc -lX11 -c

trie.o: include/trie.h trie.cc
	g++ -L/usr/X11R6/lib trie.cc -lX11 -c

window.o: include/window.h window.cc
	g++ -L/usr/X11R6/lib window.cc -lX11 -c

clean:
	rm *.o
	rm quadris
