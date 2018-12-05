WARNING_FLAGS = -Wall -Wextra -pedantic -Wformat\
				-Wshadow -Wpointer-arith\
				-Wmissing-prototypes -Wcast-align -pedantic-errors

CFLAGS = -std=c99 -m64 -O3 $(WARNING_FLAGS)

CLIBS = -lm

main: main.o  BmpLib.o OutilsLib.o image.o quadtree.o region.o
	@echo "Links edition :"
	gcc -o main main.o  BmpLib.o OutilsLib.o image.o quadtree.o region.o $(CLIBS)
	rm -f *~ *.o
	@echo ""

main.o:	main.c image.h
	@echo "Compilation main :"
	gcc $(CFLAGS) -c main.c
	@echo ""

region.o: region.c region.h image.h
	@echo "Compilation region :"
	gcc $(CFLAGS) -c region.c $(CLIBS)
	@echo ""

quadtree.o: quadtree.c quadtree.h image.h
	@echo "Compilation quadtree :"
	gcc $(CFLAGS) -c quadtree.c $(CLIBS)
	@echo ""

image.o: image.c image.h BmpLib.h
	@echo "Compilation image :"
	gcc $(CFLAGS) -c image.c $(CLIBS)
	@echo ""
	

BmpLib.o: BmpLib.c BmpLib.h OutilsLib.h
	@echo "Compilation BmpLib :"
	gcc $(CFLAGS) -c BmpLib.c
	@echo ""
	
OutilsLib.o: OutilsLib.c OutilsLib.h
	@echo "Compilation OutilsLib :"
	gcc $(CFLAGS) -c OutilsLib.c
	@echo ""

clean:
	rm -f *.o
	rm -f main
	rm -f *~
