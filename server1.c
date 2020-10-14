#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define IP "127.0.0.1"
#define PORT 8080
#define SIZE 300


int check(int expresion, char* errormsg){

    if(expresion < 0)
    {
        perror(errormsg);
        return 0;

    }
    return 1;
}

void recvAll(int socket, void *buffer, size_t length, int target, FILE *fileOutput)
{
    size_t bytesRecv = 0;
    int sumOfBytesReceived = 0;

    while(1){

        check(bytesRecv = recv(socket, buffer, length, 0),"[-]Data not received");

        sumOfBytesReceived += bytesRecv;

        if(target != sizeof(int))
            fwrite(buffer, 1, sizeof(buffer[0]) * bytesRecv, fileOutput);

        if(sumOfBytesReceived == target)
                return;
    }

    
}

void receiveDataBinary(int sockfd, FILE *fileOutput){

    char buffer[SIZE];
    int dimFileInput = 0;

    recvAll(sockfd, &dimFileInput, sizeof(dimFileInput), sizeof(int), fileOutput);
    
    recvAll(sockfd, buffer, SIZE, dimFileInput, fileOutput);

}


int main(){

    int sockfd, new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[SIZE];

    check(sockfd = socket(AF_INET, SOCK_STREAM, 0), "[-]Socket failed");
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = inet_addr(IP);

    check(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)),"[-]Bind failed");
    printf("[+]Binding successfull.\n");

    check(listen(sockfd, 3) == 0,"[-]Listen failed");
    printf("[+]Listening....\n");

    addr_size = sizeof(new_addr);
    check(new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size),"[-]Accept failed");

    FILE *fileOutput = fopen("data.out", "wb");

    receiveDataBinary(new_sock, fileOutput);
    printf("[+]Data written in the file successfully.\n");

    fclose(fileOutput);
    close(sockfd);

    return 0;
}
