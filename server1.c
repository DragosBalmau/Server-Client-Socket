#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define IP "127.0.0.1"
#define PORT 8080
#define SIZE 300

void receiveData(int sockfd){

    char buffer[SIZE];
    int valrec, dimdata, valnum;

    FILE *fileOutput = fopen("data.out", "w");
    
    while (1) {

        valrec = recv(sockfd, buffer, SIZE, 0);
        valnum = recv(sockfd, &dimdata, sizeof(dimdata), 0);

        if (valrec <= 0 || valnum <= 0 || valrec != dimdata){
            break;
            return;
        }

    fprintf(fileOutput, "%s", buffer);
    bzero(buffer, SIZE);
    }

    return;
}

void receiveDataBinary(int sockfd){

    char buffer[SIZE];
    int valrec, dimdata, valnum;

    FILE *fileOutput = fopen("data.out", "wb");
    
    while (1) {

        valrec = recv(sockfd, buffer, SIZE, 0);
        valnum = recv(sockfd, &dimdata, sizeof(dimdata), 0);

        if (valrec <= 0 || valnum <= 0 || valrec != dimdata){
            break;
            return;
        }

    fwrite(buffer, 1, sizeof(buffer),  fileOutput);
    bzero(buffer, SIZE);
    }

    return;
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

    receiveDataBinary(new_sock);
    printf("[+]Data written in the file successfully.\n");

    return 0;
}