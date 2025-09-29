#include <stdio.h>      // Para entrada e saída padrão (printf, etc.)
#include <stdlib.h>     // Para funções como exit() e atoi()
#include <string.h>     // Para manipulação de strings (strcpy, bzero, etc.)
#include <unistd.h>     // Para close(), read(), write()
#include <sys/socket.h> // Para as funções de socket (socket, bind, listen, accept)
#include <arpa/inet.h>  // Para a estrutura sockaddr_in e funções de rede
#include <pthread.h>    // Para as funções de thread (pthread_create, etc.)

#define MAX_CLIENTS 10 // Número máximo de clientes (para o futuro)
#define BUFFER_SIZE 2048 // Tamanho do buffer para as mensagens

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("Modo de uso: %s <numero_da_porta>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int porta = atoi(argv[1]);

    int server_socket;
    int client_socket;  

    struct sockaddr_in server_addr; 
    struct sockaddr_in client_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(porta);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

}