#include <stdio.h>
#include <stdlib.h>
#include "client.h"
#include <string.h>
#include <time.h>
#include <math.h>

// Adiciona novo cliente
Client *addClient(const char *cpf, const char *name, const char *phone, const char *numReservation){
    Client *c = (Client *)malloc(sizeof(Client));
    
    if (c)
    {
        memset(c, 0, sizeof(Client)); // Zera a memoria
        
        // Copia valores para os campos do disco
        strncpy(c->name, name, sizeof(c->name) - 1);
        strncpy(c->cpf, cpf, sizeof(c->cpf) - 1);
        strncpy(c->phone, phone, sizeof(c->phone) - 1);
        strncpy(c->numReservation, numReservation, sizeof(c->numReservation) - 1);
    }
    return c;
}

// Salva cliente no arquivo arq, na posição do cursor
void saveClient(Client *c, FILE *arq) {
    fwrite(c->cpf, sizeof(char), sizeof(c->cpf), arq);
    fwrite(c->name, sizeof(char), sizeof(c->name), arq);
    fwrite(c->phone, sizeof(char), sizeof(c->phone), arq);
    fwrite(c->numReservation, sizeof(char), sizeof(c->numReservation), arq);
}

//Le um cliente do arquivo na posição atual
Client *readClient(FILE *in) {
    Client *c = (Client *) malloc(sizeof(Client));

    if (fread(c->cpf, sizeof(char), sizeof(c->cpf), in) != sizeof(c->cpf))
    {
        free(c);
        return NULL;
    }
    fread(c->name, sizeof(char), sizeof(c->name), in);
    fread(c->phone, sizeof(char), sizeof(c->phone), in);
    fread(c->numReservation, sizeof(char), sizeof(c->numReservation), in);
    
    return c;
}

//imprime cliente
void printClient(Client *c){
    printf("**********************************************\n");
    printf("Name: %s\n", c->name);
    printf("CPF: %s\n", c->cpf);
    printf("Phone: %s\n", c->phone);
    printf("Number of Reservation: %s\n", c->numReservation);
    printf("**********************************************\n");
}

//Retorna o tamanho do cadastro do cliente em bytes
int lengthOfRegisterClient() {
    return sizeof(char) * 50   //cpf
        + sizeof(char) * 15    //nome
        + sizeof(char) * 15    //telefone
        + sizeof(char) * 10;   //Numero da reserva
}

//Retorna a quantidade de registros no arquivo
int qntOfRegisterClient(FILE *arq) {
    fseek (arq, 0, SEEK_END);
    int length = ftell(arq) / lengthOfRegisterClient();
    return length;
}

//Embaralha base de dados
void shuffleClient(int *vet, int max, int swap) {
    srand(time(NULL));

    for (int i = 0; i < swap; i++)
    {
        int j = rand() % max;
        int k = rand() % max;
        
        int tmp = vet[j];
        vet[j] = vet[k];
        vet[k] = tmp;
    }
}

// Gera nome aleatório com 5 letras
void generateRandomName(char *name, int length) {
    for (int i = 0; i < length; i++) {
        name[i] = 'a' + rand() % 26;
    }
    name[length] = '\0';
}

// Cria base de dados desordenada
void madeDisorderedBaseClient(FILE *arq, int length, int qntSwap) {
    int vet[length];
    Client *c;

    // Inicializa aleatoriedade
    srand(time(NULL));

    for (int i = 0; i < length; i++)
        vet[i] = i + 1;

    shuffleClient(vet, length, qntSwap);

    printf("\nGenerating database...\n");

    for (int i = 0; i < length; i++) {
        char name[6];
        char cpf[15];
        char phone[12];
        char numReservation[17];

        // Gera nome aleatório
        generateRandomName(name, 5);

        // Gera CPF falso com 11 dígitos numéricos
        snprintf(cpf, sizeof(cpf), "%011d", rand() % 1000000000 + i * 17);

        // Gera telefone fictício
        snprintf(phone, sizeof(phone), "99999-%04d", rand() % 10000);

        // Número de reserva aleatório
        snprintf(numReservation, sizeof(numReservation), "R%04d", rand() % 10000);

        // Cria e salva o cliente
        c = addClient(cpf, name, phone, numReservation);
        saveClient(c, arq);
        free(c);
    }
}

//Imprime a base de dados
void printDataBaseClient(FILE *arq){
    printf("\nPrinting database...\n");

    rewind(arq);
    Client *c;

    while ((c = readClient(arq)) != NULL){
        printClient(c);
        free(c); //Libera logo após imprimir
    }
    }

//Algoritmo de Busca Sequencial/linear
Client *linearSearchClient(const char *key, FILE *arq) {
    Client *c;
    int find = 0;
    clock_t start_time, end_time;
    double cpu_time_used;

    start_time = clock(); //tempo inicial

    rewind(arq);

    while ((c = readClient(arq)) != NULL)
    {
        if (strcmp(c->cpf, key) == 0)
        {
            end_time = clock();
            cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
            printf("\nClient found in %f seconds.\n", cpu_time_used);
            return c;
        }
        free(c);
    }

    end_time = clock(); //tempo final
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC; //Calcula o tempo de execução
    printf("\nClient not found. Search time: %f seconds.\n", cpu_time_used);
    return NULL;
}

//Algoritmo de Busca Binária
Client *binarySearchClient(FILE *arq, const char *key, int start, int end) {
    clock_t start_time, end_time;
    double cpu_time_used;

    start_time = clock(); // Captura o tempo inicial

    while (start <= end) {
        int middle = start + (end - start) / 2;
        fseek(arq, middle * lengthOfRegisterClient(), SEEK_SET);

        Client *c = readClient(arq);

        int cmp = strcmp(c->cpf, key);

        if (cmp == 0) {
            end_time = clock(); 
            cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC; 
            printf("\nClient found in %f seconds.\n", cpu_time_used);
            return c;
        }
        if (cmp > 0) {
            end = middle - 1;
        } else {
            start = middle + 1;
        }
        free(c);
    }

    end_time = clock(); // Captura o tempo final
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC; // Calcula o tempo de execução
    printf("\nClient not found. Search time %f seconds.\n", cpu_time_used);
    return NULL;
}

//Algoritmo Bubble Sort para ordenar a base de dados
void bubbleSortClient(FILE *arq, int length){
    Client *currentClient = NULL;
    Client *nextClient = NULL;
    int swapped;

    for (int i = 0; i < length - 1; i++) {
        swapped = 0;
        for (int j = 0; j < length - i - 1; j++) {
            // Posiciona o cursor no início do cliente atual
            fseek(arq, j * lengthOfRegisterClient(), SEEK_SET);
            currentClient = readClient(arq);

            // Posiciona o cursor no início do próximo cliente
            fseek(arq, (j + 1) * lengthOfRegisterClient(), SEEK_SET);
            nextClient = readClient(arq);

            // Compara os CPF dos clientes 
            if (strcmp(currentClient->cpf, nextClient->cpf) > 0) {
                // Troca o registro dos clientes de lugar no arquivo
                fseek(arq, j * lengthOfRegisterClient(), SEEK_SET);
                saveClient(nextClient, arq);

                fseek(arq, (j + 1) * lengthOfRegisterClient(), SEEK_SET);
                saveClient(currentClient, arq);

                swapped = 1;
            }

            // Libera a memória alocada para os clientes
            free(currentClient);
            free(nextClient);
        }

        // Se nenhuma troca foi feita, a lista já está ordenada
        if (!swapped) {
            break;
        }
    }
}