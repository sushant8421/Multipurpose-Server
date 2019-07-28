# Multipurpose-Server
# Implementing services like 1.Echo request reply 2.Application layer RTT measurement 3.File upload

In one tab :
compile using : gcc server_new.c -o server_new -pthread

run by : ./server_new

In new tab parallely :

compile using : gcc client_new.c -o client_new -pthread

run by : ./client_new


1.Server is providing some services that will be send to client when it will connect to server using connect()
funtion.There can be more than one client that can access the same server at same time , the client is authenticated using password , server has provided some predefined password to authorised users.

2.There will 3 choices provided by server to client 1. For echo request and reply, 2. For RTT measurement,3. For uploading  a file and other than this anything pressed will make exit from the program.

3.In first choice the sender will send  a string and server will store it and send back the same file and also record the round trip time , after that client will be given a choice to continue with this or not, if he wants to exit then the time in each round will be printed.

4.In second choice the client will be asked the message and the number of iteration to find the round trip time.

5.In third choice the client will be asked for the file path to be uploaded on the server , after uploading is done server will send the time taken by it for uplaoding.
