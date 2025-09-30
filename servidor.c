#include <stdio.h>      // Para entrada e saída padrão (printf, etc.)
#include <stdlib.h>     // Para funções como exit() e atoi()
#include <string.h>     // Para manipulação de strings (strcpy, bzero, etc.)
#include <unistd.h>     // Para close(), read(), write()
#include <sys/socket.h> // Para as funções de socket (socket, bind, listen, accept)
#include <arpa/inet.h>  // Para a estrutura sockaddr_in e funções de rede
#include <pthread.h>    // Para as funções de thread (pthread_create, etc.)

#define MAX_CLIENTS 10 // Número máximo de clientes (para o futuro)
#define BUFFER_SIZE 2048 // Tamanho do buffer para as mensagens

void *handle_client(void *arg) {
    // A primeira coisa a fazer é pegar o "pacote" (arg) e tirar a ferramenta de dentro.
    int client_socket = *((int*)arg);
    free(arg); // Lembre-se de jogar a "caixa" do pacote fora para não gerar lixo (memory leak).

    char buffer[BUFFER_SIZE];
    int bytes_read;

    // Dentro de handle_client, após pegar o client_socket
    while ( (bytes_read = read(client_socket, buffer, sizeof(buffer) - 1)) > 0 ) {
        // Cliente enviou uma mensagem!
        buffer[bytes_read] = '\0'; // Adiciona o terminador nulo para tratar como string
        printf("Cliente %d diz: %s", client_socket, buffer);
        // Futuramente, aqui virá a lógica para enviar essa mensagem aos outros clientes
    }

    close(client_socket);
    printf("Cliente %d desconectado.\n", client_socket);
    pthread_exit(NULL);
}
void main(int argc, char *argv[]){

    if(argc != 2){
        printf("Modo de uso: %s <numero_da_porta>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int porta = atoi(argv[1]);

    int server_socket;
    int client_socket;  

    struct sockaddr_in server_addr; 
    struct sockaddr_in client_addr;

    socklen_t addr_len = sizeof(client_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(porta);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(server_socket < 0){
        perror("Erro ao obter licença de socket.");
    }

    int aux = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    if(aux == -1){
        perror("Erro ao registrar endereço no mapa.");
    }

    listen(server_socket, MAX_CLIENTS);

    printf("servidor aguardando conexões. . .");

    while(1){
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if(client_socket < 0){
            perror("Erro ao aceitar conexão.");
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));

        int client_port = ntohs(client_addr.sin_port);
        
        printf("Conexão aceita do cliente %s na porta %d\n", client_ip, client_port);
    }

    pthread_t thread_id;
    int *p_client_socket = malloc(sizeof(int));
    *p_client_socket = client_socket;

    pthread_create(&thread_id, NULL, handle_client, (void *)p_client_socket);

    pthread_detach(thread_id);

    return;
}