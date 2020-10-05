#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080
#define BUFSIZE 300

int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int addrlen = sizeof(address); 
 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 

    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
	
  
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                           (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
    char buffer[BUFSIZE] = {0};

    char number[10] = {0};

    while(valread = recv(new_socket, number, 10, 0) > 0){
        
        int dataDimension;

        sscanf(number, "%d", &dataDimension);

        valread = recv(new_socket, buffer, dataDimension, 0);
        
        char ok[2];

        strcpy(ok,"a");

        if(dataDimension == strlen(buffer))
            send(new_socket, ok, 2, 0);
        else
            while(dataDimension != sizeof(buffer) - 1){

                strcpy(ok[0],"b");
                valread = recv(new_socket, buffer, dataDimension, 0);
                send(new_socket, ok, 2, 0);
            }

        printf("%s", buffer);
        
        memset(buffer, 0, dataDimension);

        number[0] = '\0';
    }

} 
