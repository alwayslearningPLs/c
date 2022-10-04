build: hello_world.c
	clang -o hello_world.o ./hello_world.c

run: hello_world.o
	./hello_world.o

clean: hello_world.o
	@rm hello_world.o

all: build run clean
