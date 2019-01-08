WARNING_FLAGS = -Wall -Wextra -pedantic -Wformat\
				-Wshadow -Wpointer-arith\
				-Wmissing-prototypes -Wcast-align -pedantic-errors

CFLAGS = -std=c99 -m64 -O3 $(WARNING_FLAGS)

CLIBS = -lm

main: main.o  BmpLib.o OutilsLib.o image.o region.o symetrie.o retina.o
	@echo "Links edition :"
	gcc -o main main.o  BmpLib.o OutilsLib.o image.o region.o symetrie.o retina.o $(CLIBS)
	rm -f *~ *.o
	@echo ""

main.o:	main.c image.h
	@echo "Compilation main :"
	gcc $(CFLAGS) -c main.c
	@echo ""

retina.o: retina.c retina.h symetrie.h
	@echo "Compilation retina"
	gcc $(CFLAGS) -c retina.c $(CLIBS)
	@echo ""

symetrie.o: symetrie.c symetrie.h image.h
	@echo "Compilation symetrie :"
	gcc $(CFLAGS) -c symetrie.c $(CLIBS)
	@echo ""

region.o: region.c region.h image.h
	@echo "Compilation region :"
	gcc $(CFLAGS) -c region.c $(CLIBS)
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
