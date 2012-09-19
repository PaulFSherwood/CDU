CFLAGS = -Wall -Werror -std=c99
LFLAGS = `sdl-config --libs` -lSDL -lSDL_image -lSDL_ttf -lSDL_net
OBJS   = main.o
PROG = cdu
CXX = gcc

# top-level rule to create the program.
all: $(PROG)

# compiling other source files.
%.o: src/%.c
	$(CXX) $(CFLAGS) -c -s $<

# linking the program.
$(PROG): $(OBJS)
	$(CXX) $(OBJS) -o $(PROG) $(LFLAGS)

# cleaning everything that can be automatically recreated with "make".
clean:
	rm $(PROG) *.o
