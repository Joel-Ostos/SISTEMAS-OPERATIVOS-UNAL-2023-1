
client: client.c server
	gcc client.c -o client

server: server.c conversor
	gcc server.c -o server

conversor: conversor.c 
	gcc conversor.c -o conversor

clean:
	rm -f client 
	rm -f server 
	rm -r conversor

.PHONY: all clean

