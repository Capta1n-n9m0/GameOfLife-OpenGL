ARGS		=
EXECUTABLE	=gameOfLife
CC			=gcc
CFLAGS		=-Wall -Werror --pedantic -std=c2x -g
LDFLAGS		=-lGL -lGLU -lglut -lm
SOURCES		=$(wildcard *.c)
HEADERS		=$(wildcard *.h)
OBJECTS		=$(SOURCES:.c=.o)


all: $(EXECUTABLE)
	./$(EXECUTABLE) $(ARGS)

build: $(EXECUTABLE)

clean_run: $(EXECUTABLE)
	./$(EXECUTABLE) $(ARGS)
	rm $(OBJECTS)

valgrind: $(EXECUTABLE)
	valgrind --leak-check=full -v ./$(EXECUTABLE) $(ARGS)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $(EXECUTABLE) $(LDFLAGS)

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	-rm $(OBJECTS) $(EXECUTABLE)

# prepare: $(SOURCES) $(HEADERS)
# 	python3 prepare_file.py $(SOURCES) $(HEADERS) makefile test.txt ex3test.txt
