#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define IP "127.0.0.1"
#define PORT 8080
#define SIZE 1024

struct thread_data_t {

    int new_socket;
    struct sockaddr_in new_addr;

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

    FILE *fileOutput = fopen("data.out", "wb");

    char buffer[SIZE];

    int dimFileInput = 0, dimFileOutput = 0;

    recvAll(sockfd, &dimFileInput, sizeof(dimFileInput), sizeof(int));

    int dataReceived = 0;
    
    while (1) {

        check(dataReceived = recv(sockfd, buffer, SIZE, 0),"[-]Data not received");

        dimFileOutput += dataReceived;

        fwrite(buffer, 1, sizeof(buffer[0]) * dataReceived, fileOutput);

        if(dimFileOutput == dimFileInput)
            return NULL;

        memset(buffer, 0, sizeof(buffer));
    }

    close(sockfd);
    pthread_exit(0);
    fclose(fileOutput);
   
    return NULL;

}

int main(){

    struct thread_data_t pthread_arg;
    pthread_t thread;
    int server_sock;

    struct sockaddr_in server_addr;
    socklen_t addr_size;
    int opt = 1;

    check(server_sock = socket(AF_INET, SOCK_STREAM, 0), "[-]Socket failed");
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = inet_addr(IP);

    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    check(bind(server_sock, (struct sockaddr*)&(server_addr), sizeof(server_addr)),"[-]Bind failed");
    printf("[+]Binding successfull.\n");

    check(listen(server_sock , 50) == 0,"[-]Listen failed");
    printf("[+]Listening....\n");

    while(1){

	    addr_size = sizeof(pthread_arg.new_addr);

	    check(pthread_arg.new_socket = accept(server_sock, (struct sockaddr*)&pthread_arg.new_addr, &addr_size),"[-]Accept failed");

	    pthread_create(&thread, NULL, receiveDataBinary, &pthread_arg.new_socket);
	  	
	    printf("[+]Data written in the file successfully with thread\n");

	}

    return 0;
}
