all: server_new client_new 

server_new: server_new.c
	gcc server_new.c -o server_new -pthread 


client_new: client_new.c
	gcc client_new.c -o   client_new

clean:
	rm server_new client_new  
