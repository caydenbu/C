clean:
	rm -f server client

server: server.c
	gcc server.c -o server -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo

client: client.c component/components.c
	gcc client.c component/components.c -o client -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo


run-server: server
	./server

run-client: client
	./client

