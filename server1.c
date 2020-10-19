#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define IP "127.0.0.1"
#define PORT 8080
#define SIZE 1024
#define NUM_CLIENT 1

struct thread_data_t {

    pthread_t thread;
    int tid;
    int server_sock;
    int new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;

};


int check(int expresion, char* errormsg){

    if(expresion < 0)
    {
        perror(errormsg);
        return 0;

    }
    return 1;
}

void recvAll(int socket, void *buffer, size_t length, int target)
{
    size_t bytesRecv = 0;
    int sumOfBytesReceived = 0;

    while(1){

        check(bytesRecv = recv(socket, buffer, length, 0),"[-]Data not received recAll");

        sumOfBytesReceived += bytesRecv;

        buffer += bytesRecv;
        length -= bytesRecv;

        if(sumOfBytesReceived == target)
                return;

    }
 
}

void* receiveDataBinary(void* p_sockfd){

    int sockfd = *((int *)p_sockfd);

    printf("TEST\n");

    FILE *fileOutput = fopen("data.out", "wb");

    char buffer[SIZE];

    int dimFileInput = 0, dimFileOutput = 0;

    recvAll(sockfd, &dimFileInput, sizeof(dimFileInput), sizeof(int));

    printf("%d\n", dimFileInput);

    int dataReceived = 0;
    
    while (1) {

        check(dataReceived = recv(sockfd, buffer, SIZE, 0),"[-]Data not received");

        dimFileOutput += dataReceived;

        fwrite(buffer, 1, sizeof(buffer[0]) * dataReceived, fileOutput);

        //printf("%s\n", buffer);

        if(dimFileOutput == dimFileInput)
            return NULL;

        memset(buffer, 0, sizeof(buffer));
    }

    close(sockfd);
    fclose(fileOutput);
    return NULL;

}

int main(){

    struct thread_data_t thr_data[NUM_CLIENT];

    int opt = 1;
    
    for (int i = 0; i < NUM_CLIENT; ++i) {

        thr_data[i].tid = i;
        check(thr_data[i].server_sock = socket(AF_INET, SOCK_STREAM, 0), "[-]Socket failed");
        printf("[+]Server socket created successfully on thread %d.\n", thr_data[i].tid);

        thr_data[i].server_addr.sin_family = AF_INET;
        thr_data[i].server_addr.sin_port = PORT;
        thr_data[i].server_addr.sin_addr.s_addr = inet_addr(IP);

        setsockopt(thr_data[i].server_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

        check(bind(thr_data[i].server_sock, (struct sockaddr*)&(thr_data[i].server_addr), sizeof(thr_data[i].server_addr)),"[-]Bind failed");
        printf("[+]Binding successfull.\n");

        check(listen(thr_data[i].server_sock , 3) == 0,"[-]Listen failed");
        printf("[+]Listening....\n");

        thr_data[i].addr_size = sizeof(thr_data[i].new_addr);
        check(thr_data[i].new_sock = accept(thr_data[i].server_sock, (struct sockaddr*)&(thr_data[i].new_addr), &(thr_data[i].addr_size)),"[-]Accept failed");

        pthread_create(&(thr_data[i].thread), NULL, receiveDataBinary, &(thr_data[i].server_sock));
        
        //close(thr_data[i].server_sock);
    }

    printf("[+]Data written in the file successfully.\n");

    for (int i = 0; i < NUM_CLIENT; ++i) //block pana termina fiecare thread
        pthread_join(thr_data[i].thread, NULL);

    return 0;
}#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define IP "127.0.0.1"
#define PORT 8080
#define SIZE 1024
#define NUM_CLIENT 1

struct thread_data_t {

    pthread_t thread;
    int tid;
    int server_sock;
    int new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;

};


int check(int expresion, char* errormsg){

    if(expresion < 0)
    {
        perror(errormsg);
        return 0;

    }
    return 1;
}

void recvAll(int socket, void *buffer, size_t length, int target)
{
    size_t bytesRecv = 0;
    int sumOfBytesReceived = 0;

    while(1){

        check(bytesRecv = recv(socket, buffer, length, 0),"[-]Data not received recAll");

        sumOfBytesReceived += bytesRecv;

        buffer += bytesRecv;
        length -= bytesRecv;

        if(sumOfBytesReceived == target)
                return;

    }
 
}

void* receiveDataBinary(void* p_sockfd){

    int sockfd = *((int *)p_sockfd);

    printf("TEST\n");

    FILE *fileOutput = fopen("data.out", "wb");

    char buffer[SIZE];

    int dimFileInput = 0, dimFileOutput = 0;

    recvAll(sockfd, &dimFileInput, sizeof(dimFileInput), sizeof(int));

    printf("%d\n", dimFileInput);

    int dataReceived = 0;
    
    while (1) {

        check(dataReceived = recv(sockfd, buffer, SIZE, 0),"[-]Data not received");

        dimFileOutput += dataReceived;

        fwrite(buffer, 1, sizeof(buffer[0]) * dataReceived, fileOutput);

        //printf("%s\n", buffer);

        if(dimFileOutput == dimFileInput)
            return NULL;

        memset(buffer, 0, sizeof(buffer));
    }

    close(sockfd);
    fclose(fileOutput);
    return NULL;

}

int main(){

    struct thread_data_t thr_data[NUM_CLIENT];

    int opt = 1;
    
    for (int i = 0; i < NUM_CLIENT; ++i) {

        thr_data[i].tid = i;
        check(thr_data[i].server_sock = socket(AF_INET, SOCK_STREAM, 0), "[-]Socket failed");
        printf("[+]Server socket created successfully on thread %d.\n", thr_data[i].tid);

        thr_data[i].server_addr.sin_family = AF_INET;
        thr_data[i].server_addr.sin_port = PORT;
        thr_data[i].server_addr.sin_addr.s_addr = inet_addr(IP);

        setsockopt(thr_data[i].server_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

        check(bind(thr_data[i].server_sock, (struct sockaddr*)&(thr_data[i].server_addr), sizeof(thr_data[i].server_addr)),"[-]Bind failed");
        printf("[+]Binding successfull.\n");

        check(listen(thr_data[i].server_sock , 3) == 0,"[-]Listen failed");
        printf("[+]Listening....\n");

        thr_data[i].addr_size = sizeof(thr_data[i].new_addr);
        check(thr_data[i].new_sock = accept(thr_data[i].server_sock, (struct sockaddr*)&(thr_data[i].new_addr), &(thr_data[i].addr_size)),"[-]Accept failed");

        pthread_create(&(thr_data[i].thread), NULL, receiveDataBinary, &(thr_data[i].server_sock));
        
        //close(thr_data[i].server_sock);
    }

    printf("[+]Data written in the file successfully.\n");

    for (int i = 0; i < NUM_CLIENT; ++i) //block pana termina fiecare thread
        pthread_join(thr_data[i].thread, NULL);

    return 0;
}
