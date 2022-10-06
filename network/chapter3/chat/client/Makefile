build: main.c
	clang main.c -o main

run: main
	./main ::1 8080

clean: main
	@rm main

all: build run clean
