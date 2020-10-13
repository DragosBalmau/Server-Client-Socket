#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define IP "127.0.0.1"
#define PORT 8080
#define SIZE 300

void sendData(FILE *fileInput, int sockfd){

    char data[SIZE] = {0};

    while(fgets(data, SIZE, fileInput) != NULL) {

        int valsend = 0, valnum = 0;
        //printf("%s\n", data);

        valsend = send(sockfd, data, sizeof(data), 0);
        valnum = send(sockfd, &valsend, sizeof(valsend), 0);

        if (valsend == -1 || valnum == -1) {

            perror("[-]Error in sending file.");
            exit(1);
        }

    bzero(data, SIZE);
    }
}

void sendDataBinary(FILE *fileInput, int sockfd){

    char data[SIZE] = {0};

    while(fread(data, 1, SIZE, fileInput) != NULL) {

        int valsend = 0, valnum = 0;
        //printf("%s\n", data);

        valsend = send(sockfd, data, sizeof(data), 0);
        valnum = send(sockfd, &valsend, sizeof(valsend), 0);

        if (valsend == -1 || valnum == -1) {

            perror("[-]Error in sending file.");
            exit(1);
        }

    bzero(data, SIZE);
    }
}

int check(int expresion, char* errormsg){

    if(expresion < 0)
    {
        perror(errormsg);
        return 0;

    }
    return 1;
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

      FILE *fileInput = fopen("binar.bin", "r");
      if (fileInput == NULL) {
          perror("[-]Reading file failed");
          exit(1);
      }

    sendDataBinary(fileInput, sockfd);
    printf("[+]File data sent successfully.\n");

    printf("[+]Closing the connection.\n");
    close(sockfd);

    return 0;
}