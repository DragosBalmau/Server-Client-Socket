#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define IP "127.0.0.1"
#define PORT 8080
#define SIZE 1024

#define MAXPENDING 50

struct thread_data_t {

    int new_socket;
    struct sockaddr_in* new_addr;

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

void* receiveDataBinary(void* p_pthread_arg){

    struct thread_data_t pthread_arg = *((struct thread_data_t *)p_pthread_arg);

    FILE *fileOutput = fopen("data.out", "wb");

    char buffer[SIZE];

    int dimFileInput = 0, dimFileOutput = 0;

    recvAll(pthread_arg.new_socket, &dimFileInput, sizeof(dimFileInput), sizeof(int));

    int dataReceived = 0;
    
    while (1) {

        check(dataReceived = recv(pthread_arg.new_socket, buffer, SIZE, 0),"[-]Data not received");

        dimFileOutput += dataReceived;

        fwrite(buffer, 1, sizeof(buffer[0]) * dataReceived, fileOutput);

        if(dimFileOutput == dimFileInput)
            return NULL;

        memset(buffer, 0, sizeof(buffer));
    }

 
    //pthread_exit(0);
    fclose(fileOutput);
   
    return NULL;

}

int main(){

    int server_sock;
    struct sockaddr_in server_addr;

    check(server_sock = socket(AF_INET, SOCK_STREAM, 0), "[-]Socket failed");
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = inet_addr(IP);

    check(bind(server_sock, (struct sockaddr*)&(server_addr), sizeof(server_addr)),"[-]Bind failed");
    printf("[+]Binding successfull.\n");

    check(listen(server_sock , MAXPENDING) == 0,"[-]Listen failed");
    printf("[+]Listening....\n");

    while(1){

		pthread_t thread;

		struct sockaddr server_addr;
		socklen_t addr_size  = sizeof(server_addr);
		int new_sock;

		check(new_sock = accept(server_sock, (struct sockaddr*)&server_addr, &addr_size),"[-]Accept failed");

		struct thread_data_t *p_pthread_arg = malloc(sizeof(struct thread_data_t));
		p_pthread_arg->new_addr = malloc(sizeof(struct sockaddr));

		memcpy(p_pthread_arg->new_addr, &server_addr, addr_size);
		p_pthread_arg->new_socket = new_sock;

		pthread_create(&thread, NULL, receiveDataBinary, (void *) p_pthread_arg);
			
		printf("[+]Data written in the file successfully with thread\n");

		pthread_detach(thread);

	}

    return 0;
}

