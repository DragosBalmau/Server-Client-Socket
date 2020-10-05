
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h>
#define PORT 8080
#define BUFSIZE 300
   
int main(int argc, char const *argv[]) 
{ 

    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        perror("\n Socket creation error \n"); 
        exit(EXIT_FAILURE);  
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)  
    { 
        perror("\nInvalid address/ Address not supported \n"); 
        exit(EXIT_FAILURE); 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        perror("\nConnection Failed \n"); 
        exit(EXIT_FAILURE);  
    }


    char buffer[BUFSIZE] = {0};

    buffer[0] = '\0';

    FILE *f = fopen("data.in", "r");

    size_t bytes_read; 

    while((bytes_read = fread(buffer, 1, BUFSIZE - 1, f)) > 0){

        char dataDimension[10];

        sprintf(dataDimension, "%d", (int)bytes_read);

		send(sock, dataDimension, 10, 0);

        e:

        send(sock, buffer, (int)bytes_read, 0);
        
        char ok[2];

        valread = recv(sock, ok, 2, 0);

        if(!strcmp(ok,"b"))
            goto e;

        memset(buffer, 0, (int)bytes_read);


    }

  
} 
