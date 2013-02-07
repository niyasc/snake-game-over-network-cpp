CC = g++
EXE = snake
LIBS = -lGL -lGLU -lGLEW -lglut -lm
SRCS = snake.cpp
OBJS = $(SRCS:.c=.o)
$(EXE): $(OBJS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)
$(OBJS): $(HDRS) Makefile
clean:
	rm -f core $(EXE) *.o
