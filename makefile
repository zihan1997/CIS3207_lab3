all: server client multiClient
server: Server.c
	gcc Server.c -o server -lpthread
client: Client.c
	gcc Client.c -o client
multiClient: Multi_Client.c
	gcc Multi_Client.c -o multiClient -lpthread
clean:
	rm server
	rm client
	rm multiClient
run:
	./server