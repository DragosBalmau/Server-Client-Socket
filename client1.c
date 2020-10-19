#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define IP "127.0.0.1"
#define PORT 8080
#define SIZE 1024


int getFileDim(FILE *fileInput){

    fseek(fileInput, 0L, SEEK_END);
    int sizeofFile = ftell(fileInput);
    fseek(fileInput, 0L, SEEK_SET);

    return sizeofFile;
}

int sendAll(int socket, const void *buffer, size_t length, int flag)
{
    size_t bytesSent;
    const char *copyBuffer = buffer;

    while (length > 0)
    {
        bytesSent = send(socket, copyBuffer, length, flag);

        if (bytesSent <= 0)
            return -1;
        copyBuffer += bytesSent;
        length -= bytesSent;
    }
    return 0;
}


int check(int expresion, char* errormsg){

    if(expresion < 0)
    {
        perror(errormsg);
        return 0;

    }
    return 1;
}

void sendDataBinary(int sockfd){


    FILE *fileInput = fopen("binar.bin", "rb");

    if (fileInput == NULL) {
      perror("[-]Reading file failed");
      exit(1);
    }

    char data[SIZE] = {0};

    int dimFile = getFileDim(fileInput);

    check(sendAll(sockfd, &dimFile, sizeof(dimFile), 0), "[-]Dim fileInput not sent");

    //printf("%d\n", dimFile);

    int howmuchread = 0;

    while(check(howmuchread = fread(data, 1, SIZE, fileInput),"[-]Data not read")) {

        if(howmuchread == 0)
            return;

        check(sendAll(sockfd, data, sizeof(data[0]) * howmuchread, 0),"[-]Error in sending file.");
        
        //printf("%d\n", howmuchread);

        memset(data, 0, sizeof(data));
    }

    fclose(fileInput);

    
}

int main(){


    int sockfd;
    struct sockaddr_in server_addr;

    check(sockfd = socket(AF_INET, SOCK_STREAM, 0),"[-]Socket failed");
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = inet_addr(IP);

    check(connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)),"[-]Connection to server failed");
    printf("[+]Connected to Server.\n");

    sendDataBinary(sockfd);

    printf("[+]File data sent successfully.\n");

    printf("[+]Closing the connection.\n");
    close(sockfd);

    return 0;
}
