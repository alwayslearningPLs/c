build_server: main.c
	clang main.c -o main

run_server: main
	./main

clean_server: main
	@rm main

all_server: build_server run_server clean_server

build_client: client.c
	clang client.c -o client

run_client: client
	./client ::1 8080 -udp

clean_client: client
	@rm client

all_client: build_client run_client clean_client
