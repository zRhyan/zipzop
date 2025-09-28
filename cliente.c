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