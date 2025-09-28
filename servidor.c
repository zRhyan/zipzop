#include <stdio.h>      // Para entrada e saída padrão (printf, etc.)
#include <stdlib.h>     // Para funções como exit() e atoi()
#include <string.h>     // Para manipulação de strings (strcpy, bzero, etc.)
#include <unistd.h>     // Para close(), read(), write()
#include <sys/socket.h> // Para as funções de socket (socket, bind, listen, accept)
#include <arpa/inet.h>  // Para a estrutura sockaddr_in e funções de rede
#include <pthread.h>    // Para as funções de thread (pthread_create, etc.)

#define MAX_CLIENTS 10 // Número máximo de clientes (para o futuro)
#define BUFFER_SIZE 2048 // Tamanho do buffer para as mensagens

