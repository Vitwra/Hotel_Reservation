#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Client
{
    char name[50];
    char cpf[15];
    char phone[15];
    char numReservation[10];
}Client;

//Básico
Client *addClient(const char *cpf, const char *name, const char *phone, const char *numReservation);

void saveClient(Client *c, FILE *arq);

Client *readClient(FILE *in);

void printClient(Client *c);

// Utilizadades
int lengthOfRegisterClient();

int qntOfRegisterClient(FILE *arq);

void shuffleClient(int *vet, int max, int swap);

void madeDisorderedBaseClient(FILE *arq, int length, int qntSwap);

void printDataBaseClient(FILE *arq);

//Ordenação e Busca
Client *linearSearchClient(const char *key, FILE *arq);

#endif