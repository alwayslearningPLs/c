build: main.c
	clang main.c -o ./main

run: main
	./main 192.168.56.10 8080

clean: main
	@rm main

all: build run clean
