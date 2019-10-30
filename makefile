all: server client
server: Server.c
	gcc Server.c -o server -lpthread
client: Client.c
	gcc Client.c -o client
clean:
	rm server
	rm client
run:
	./server