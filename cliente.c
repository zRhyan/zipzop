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
    
    // Essa estrutura guarda tudo que o kernel precisa saber para se conectar 
    // a um endereço IPv4: família (IPv4), porta e o endereço IP (além de um campo de preenchimento).
    struct sockaddr_in server_addr; // A "ficha de contato" do servidor.

    // Preenchendo a ficha...
    server_addr.sin_family = AF_INET; // Família de endereço: Internet IPv4, trocar para af_INET6 para IPv6
    
    // A porta que vocês combinaram.
    // htons() traduz o número da porta para um formato que a rede entende, por exemplo: 8888 em hex é 0x22B8.
    server_addr.sin_port = htons(8888);
    
    // O IP que o usuário digitou.
    // inet_addr() traduz o IP (string de texto) para um formato numérico de rede, por exemplo: "192.168.1.10" → bytes de rede 0xC0 0xA8 0x01 0x0A  
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    // Fazer a chamada: funcao connect()
    printf("Conectando ao servidor...\n");
    
    // Tentamos nos conectar.
    // Passamos nosso socket, o endereço do servidor (com um "cast" para o tipo genérico)
    // e o tamanho da estrutura de endereço.
    if (connect(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        // Se connect() retornar um número menor que 0, deu erro.
        perror("Conexao falhou");
        return 1;
    }

    printf("Conectado com sucesso!\n");


}