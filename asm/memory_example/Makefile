build: main.asm
	yasm -f elf64 -g dwarf2 -l main.lst main.asm

link: main.o
	ld -o main.out main.o

run: main.out
	./main.out

clean: main.o main.out main.lst
	@rm main.o main.out main.lst

all: build link run

