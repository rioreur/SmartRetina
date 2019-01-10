WARNING_FLAGS = -Wall -Wextra -pedantic -Wformat\
				-Wshadow -Wpointer-arith\
				-Wmissing-prototypes -Wcast-align -pedantic-errors

CFLAGS = -std=c99 -m64 -O3 $(WARNING_FLAGS)
CC = gcc
CLIBS = -lm

TARGET = main
DEPENDENCIES = main.o BmpLib.o OutilsLib.o image.o region.o symetrie.o hsv.o retina.o

$(TARGET): $(TARGET).o $(DEPENDENCIES)
	@echo "Links edition :"
	$(CC) -o $(TARGET) $(DEPENDENCIES) $(CLIBS)
	rm -f *~ *.o
	@echo ""

%.o: %.c
	@echo "Compilation of $<"
	$(CC) $(CFLAGS) -c $<
	@echo ""

clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -f *~
