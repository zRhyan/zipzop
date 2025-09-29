/* Arquivo: cliente.c */

// --- SEÇÃO 1: CABEÇALHOS ESSENCIAIS ---
// Você precisará de todos estes.
#include <stdio.h>      // Para printf, fgets, etc.
#include <string.h>     // Para manipulação de strings
#include <stdlib.h>     // Para exit()
#include <unistd.h>     // Para close()
#include <pthread.h>    // A biblioteca de threads
#include <sys/socket.h> // A biblioteca principal de sockets
#include <arpa/inet.h>  // Para manipulação de endereços de rede


int main(int argc, char *argv[]) {

    /////////////////////////////////////////////////////////////////////////////
    // argc é a "contagem de argumentos".
    // Se for diferente de 2 (que é o nome do programa + o IP), algo está errado.
    /////////////////////////////////////////////////////////////////////////////
    if (argc != 2) {
        printf("Modo de uso: %s <IP_DO_SERVIDOR>\n", argv[0]);
        return 1; // Encerra o programa com um código de erro.
    }

    
    ////////////////////////////////////////////////////////////////////////////
    // Criar o telefone (socket)
    ////////////////////////////////////////////////////////////////////////////
    int socket_desc; // Uma variável para guardar o ID do nosso socket.

    printf("Criando socket...\n");
    // AF_INET = Usar internet IPv4
    // SOCK_STREAM = Usar o protocolo TCP (confiável, como uma ligação)
    // 0 = Deixar o sistema escolher os detalhes
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    // Se o sistema não conseguiu nos dar um socket, socket_desc será -1.
    if (socket_desc == -1) {
        perror("Nao foi possivel criar o socket");
        return 1; // Encerra o programa.
    }
    printf("Socket criado com sucesso.\n");



    ///////////////////////////////////////////////////////////////////////////
    // Agenda de contatos (estrutura do endereco)
    ///////////////////////////////////////////////////////////////////////////
    struct sockaddr_in server_addr; // A "ficha de contato" do servidor.

    // Preenchendo a ficha...
    server_addr.sin_family = AF_INET; // Família de endereço: Internet IPv4
    
    // A porta que vocês combinaram.
    // htons() traduz o número da porta para um formato que a rede entende.
    server_addr.sin_port = htons(8888);
    
    // O IP que o usuário digitou.
    // inet_addr() traduz o IP (string de texto) para um formato numérico de rede.
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);


}