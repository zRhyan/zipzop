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


// Esta função será o nosso "Ouvido".
// Ela será executada por uma thread separada.
// O tipo de retorno 'void *' e o argumento 'void *' são padrão para funções de thread.
void *thread_recebimento(void *arg) {
    int socket_desc = *(int*)arg; // Recebemos o ID do socket pelo argumento.
    char server_reply[2000];      // Um "buffer" para guardar a mensagem que chegar.
    int read_size;                // Para guardar o número de bytes que recebemos.

    // Loop infinito para ficar constantemente esperando por mensagens.
    while( (read_size = recv(socket_desc, server_reply, 2000, 0)) > 0 ) {
        // Quando recv() retorna um valor > 0, significa que uma mensagem chegou.
        
        // Adicionamos um terminador de string para garantir que estamos lidando com texto.
        server_reply[read_size] = '\0';
        
        // Imprimimos a mensagem do amigo na tela.
        printf("Amigo: %s", server_reply);
        
        // Limpamos o buffer para a próxima mensagem.
        memset(server_reply, 0, sizeof(server_reply));
    }

    // Se o loop terminar, é porque a conexão foi perdida.
    if(read_size == 0) {
        printf("Servidor desconectou. Pressione ENTER para sair.\n");
    } else if(read_size == -1) {
        perror("recv falhou");
    }

    // Se a conexão cair, a thread termina.
    return 0;
}


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


    /////////////////////////////////////////////////////////////////////////////////////
    // LANÇANDO A THREAD DO "OUVIDO"
    /////////////////////////////////////////////////////////////////////////////////////
    
    pthread_t thread_id; // Variável para guardar o ID da nossa nova thread.

    // A chamada que cria e lança a nova thread!
    // 1º arg: O endereço da variável do ID.
    // 2º arg: Atributos especiais (não precisamos, então NULL).
    // 3º arg: O nome da função que a thread vai executar (nosso "Ouvido").
    // 4º arg: O argumento que vamos passar para a função (o ID do socket).
    if (pthread_create(&thread_id, NULL, thread_recebimento, (void*) &socket_desc) < 0) {
        perror("Nao foi possivel criar a thread");
        return 1;
    }

    // --- O LOOP PRINCIPAL SE TORNA A "VOZ" ---
    char message[1000]; // Buffer para a mensagem que vamos digitar.
    while (1) {
        // fgets lê uma linha inteira do teclado, incluindo o "Enter".
        fgets(message, 1000, stdin);

        // Se o envio falhar, provavelmente a conexão caiu.
        if (send(socket_desc, message, strlen(message), 0) < 0) {
            perror("Envio falhou");
            return 1;
        }
    }

    // O programa em teoria nunca chegará aqui por causa do loop infinito.
    close(socket_desc);


}