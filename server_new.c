#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
 

clock_t start , end ;
//the thread function
void *client_connection(void *);
 
int main(int argc , char *argv[]){
    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);

    if (socket_desc == -1)
        printf("Could not create socket");
    
    puts("Socket created");
     
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 9999 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0){
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
     
    //Accept and incoming connection
    //puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    pthread_t thread_id[10];
    int k= 0 ;
    int pass[5] = {1234 , 4321 , 5700 , 5306 , 7300} ;
    

    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){   
        int m , x ;
        read(client_sock , &m , sizeof(int));
        for(x=0 ; x<5 ; x++){
            if(m == pass[x])
                break ;
        }
        if( x < 5){
            puts("Connection accepted\n");
            write(client_sock , &m , sizeof(int));
         
            if( pthread_create( &thread_id[k] , NULL ,  client_connection , (void*) &client_sock) < 0){
                perror("could not create thread\n");
                return 1;
            }
            if( k >= 10){
                k = 0 ;
                while(k < 10){
                    pthread_join(thread_id[k++], NULL);
                }
                k =  0 ;
            }
        }
        else{
            puts("Connection rejected Unauthorised user\n");
            char *msg  =  "No user with this password is authorised" ;
            int m = -1 ;
            write(client_sock , &m , sizeof(int));
            write(client_sock, msg , strlen(msg));

            break ;
        }
        
    }
    if (client_sock < 0)
    {
        perror("accept failed");

        return 1;
    }
     
    return 0;
}
 

void *client_connection(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];
     
    while(1){
        sleep(1) ;
        message = "\n1. Echo request and reply\n2. Application Layer RTT management\n3. File uploading \n4. Exit\n";
        write(sock , message , strlen(message));
     
        char choice[1000];
        read(sock,choice,1000);
        printf("choice  %s ",choice);
	    int cond=0 , k=0;double trip[100];
        if(choice[0]=='1'){
	       do{
            read_size = recv(sock , client_message , 2000 , 0) ;
            start = clock() ;
            sleep(1) ;
            client_message[read_size] = '\0';
            printf("message recieved is : %s \n" , client_message) ;
            write(sock , client_message , strlen(client_message));
        
            read_size = recv(sock , client_message , 2000 , 0) ;
            end  = clock() ; 

            //clear the message buffer
            memset(client_message, 0, 2000);

            double d = ((double)(end-start))/CLOCKS_PER_SEC ;
            read(sock,&cond,sizeof(cond));
            write(sock , &d , sizeof(d));
	    }while(cond==1);
            
    }

        else if(choice[0]=='2'){
            int i , k ;
            read(sock,&k,sizeof(k));
            double d[k] ;

            for(i=0 ; i<k ; i++){
                start = clock() ;
                read_size = recv(sock , client_message , 2000 , 0) ;
       
                client_message[read_size] = '\0';
        
                write(sock , client_message , strlen(client_message));

                memset(client_message, 0, 2000);
                //sleep(1) ;

                end  = clock() ; 
                d[i] = ((double)(end-start))/CLOCKS_PER_SEC ;
                
            }
            double d1 = 0 ;
            for(i=0 ; i<k ; i++){
            printf("rtt : %lf\n", d[i]) ;
            d1 += d[i] ;
            }
            d1 /= k ;
            write(sock , &d1 , sizeof(double));
       
        }

        else if(choice[0]=='3'){
            double d ;
            char buffer[1024] ;
            start = clock() ;
            char* fr_name = "/home/sushant/Desktop/receive.txt";
            FILE *fr = fopen(fr_name, "a");
            if(fr == NULL)
                printf("File %s Cannot be opened file on server.\n", fr_name);

            else{
                bzero(buffer, 1024); 
                int fr_block_sz = 0;
                
                while(fr_block_sz = recv(sock , buffer,1024, 0)){
                    if(fr_block_sz < 0)
                        perror("Error receiving file from client to server.\n");
                    
                    int write_sz = fwrite(buffer , sizeof(char), fr_block_sz, fr);
                    if(write_sz < fr_block_sz)
                        perror("File write failed on server.\n");
                    
                    else if(fr_block_sz)
                        break;
                    
                    bzero(buffer, 1024);
                }
                end  = clock() ; 
                printf("File recieved from client \n");
                d = ((double)(end-start))/CLOCKS_PER_SEC ;
                write(sock , &d , sizeof(double));
                fclose(fr);
            }
        }
    }

    return 0;
} 
