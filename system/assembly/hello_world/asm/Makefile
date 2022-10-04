build: hello_world.asm
	nasm -felf64 hello_world.asm

link: hello_world.o
	ld hello_world.o

run: a.out
	./a.out

clean:
	@rm hello_world.o a.out

all: build link run
