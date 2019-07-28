all: server_new client_new check_systemcall

server_new: server_new.c
	gcc server_new.c -o server_new -pthread 


client_new: client_new.c
	gcc client_new.c -o   client_new

check_systemcall: check_systemcall.c
	gcc check_systemcall.c -o   check_systemcall


clean:
	rm server_new client_new check_systemcall 
