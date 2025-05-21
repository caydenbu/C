clean:
	rm -f server client

server: server.c
	gcc -o server server.c

client: client.c
	gcc client.c -o client -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo


run-server: server
	./server

run-client: client
	./client

