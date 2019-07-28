#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <sys/time.h>


int main(){
    int socket_id, val;
    struct sockaddr_in client_addr;
    char buffer[10240];
    socket_id = socket(AF_INET, SOCK_STREAM, 0);

    printf("Enter the port number ");
    int port;
    scanf("%d", &port);
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr =  INADDR_ANY; 
    client_addr.sin_port = htons(port);

    if(connect(socket_id, (struct sockaddr*)&client_addr, sizeof(client_addr)) == 0){
        int pass ;
        printf("\nEnter the password : ") ;
        scanf("%d" , &pass) ;
        write(socket_id,&pass,sizeof(pass));

        read(socket_id,&pass,sizeof(pass));

        if(pass != -1)
            printf("\nCONNECT STATE: Connected to Server on port %d with password %d\n", port , pass);

        else{
            printf("Connection to server failed !\n");
            char msg[50] ;
            int r = recv(socket_id , msg , 50 , 0) ;
            msg[r] = '\0' ;
            printf("%s\n" , msg) ;
            return 0;
        }
        sleep(1) ;
    }

    while(1){   

        int size = read(socket_id,buffer,1024);
        buffer[size] = '\0' ;
        printf("services provided by server : %s\n",buffer);

        int s ;
        char choice[1000];
        printf("enter the service you want to use : ") ;
        scanf("%s" , choice) ;

        write(socket_id,choice,strlen(choice));
    
        int cond=1 , k = 0; double d[100] = {0} ;
   
        if(choice[0]=='1'){
            do{
                printf("\nEnter the Message to server: ");
                bzero(buffer,1024);
                scanf("%s", buffer);

                write(socket_id,buffer,strlen(buffer));
                bzero(buffer,1024);
    			read(socket_id,buffer,1024);
				write(socket_id,buffer,strlen(buffer));

				printf("Message from server: %s\n",buffer);
                printf("for continue press 1 otherwise any key : ");
                scanf("%d",&cond);
                write(socket_id,&cond,sizeof(cond));
                read(socket_id,&d[k++],sizeof(d));
                bzero(buffer,1024);
            }while(cond==1);

            int i ;
            for(i=0 ; i<k ; i++)
                printf("round time of %d round is  : %.6lf \n" ,i, d[i]) ;
        }

        else if(choice[0]=='2'){

            int i = 0 , k , b;
            double d ;
            printf("Enter the message size and iterations : \n");
            scanf("%d%d", &b , &k);
            write(socket_id,&k,sizeof(k));

            for(i=0 ; i<k ; i++){
                printf("\n iteration %d \n" , i);
                write(socket_id,buffer,b);
                read(socket_id,buffer,b);
            }
            read(socket_id,&d,sizeof(double));
            printf("\navg rtt is : %.6lf \n\n" , d) ;

        }
        else if(choice[0]=='3'){
            double d ;
            char fname[50] ;
            printf("enter the file name to be send : \n") ;
            scanf("%s" , fname) ;

            char* fp_name = fname;
            char sdbuf[1024]; 
            FILE *fp = fopen(fp_name, "r");
            if(fp == NULL){
                printf("ERROR: File %s not found.\n", fp_name);
              
            }

            bzero(sdbuf, 1024); 
            int file_block_size; 

            while((file_block_size = fread(sdbuf, sizeof(char), 1024, fp))>0){
                if(send(socket_id ,sdbuf, file_block_size, 0) < 0){
                    printf("ERROR: Failed to send file %s.\n", fp_name);
                    break;
                }
                bzero(sdbuf, 1024);
            }
            printf("Ok File %s from Client was Sent!\n", fp_name);
        
            read(socket_id,&d,sizeof(double));
            printf("avg rtt is : %.6lf \n\n" , d) ;
        }
        else
            break ;

        sleep(1) ;
    
    }

    close(socket_id);
    return 0;   
}

