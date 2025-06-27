#include "room.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Room *addRoom(int roomNumber, const char *type, int capacity, float basePrice, const char *status)
{
    Room *r = ((Room *)malloc(sizeof(Room)));

    if (r)
    {
        memset(r, 0, sizeof(Room));

        r->roomNumber = roomNumber;
        strncpy(r->type, type, sizeof(r->type) - 1);
        r->capacity = capacity;
        r->basePrice = basePrice;
        strncpy(r->status, status, sizeof(r->status) - 1);
    }
    return r;
}

void saveRoom(Room *room, FILE *arq)
{

    fwrite(&room->roomNumber, sizeof(int), 1, arq);
    fwrite(room->type, sizeof(char), sizeof(room->type), arq);
    fwrite(&room->capacity, sizeof(int), 1, arq);
    fwrite(&room->basePrice, sizeof(float), 1, arq);
    fwrite(room->status, sizeof(char), sizeof(room->status), arq);
}

Room *readRoom(FILE *arq)
{
    Room *r = (Room *)malloc(sizeof(Room));

    if (fread(&r->roomNumber, sizeof(int), 1, arq) != 1)
    {
        free(r);
        return NULL;
    }
    fread(r->type, sizeof(char), sizeof(r->type), arq);
    fread(&r->capacity, sizeof(int), 1, arq);
    fread(&r->basePrice, sizeof(float), 1, arq);
    fread(r->status, sizeof(char), sizeof(r->status), arq);
    return r;
}

void printRoom(Room *room)
{
    printf("**********************************************\n");
    printf("Room number: %d\n", room->roomNumber);
    printf("Room type (Single, Double, Suite): %s\n", room->type);
    printf("Room capacity: %d\n", room->capacity);
    printf("Price diary: %f\n", room->basePrice);
    printf("Status (Free, Occupied): %s\n", room->status);
    printf("**********************************************\n");
}

int lengthOfRegisterRoom()
{
    return sizeof(int)          // RoomNumber
           + sizeof(char) * 20  // type
           + sizeof(int)        // Capacity
           + sizeof(float)      // basePrice
           + sizeof(char) * 10; // status
}

int qntOfRegisterRoom(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    int length = ftell(arq) / lengthOfRegisterRoom();
    return length;
}

void shuffleRoom(int *vet, int max, int swap)
{
    srand(time(NULL));

    for (int i = 0; i <= swap; i++)
    {
        int j = rand() % (max - 1);
        int k = rand() % (max - 1);
        int tmp = vet[j];
        vet[j] = vet[k];
        vet[k] = tmp;
    }
}

//cria base de dados baseada no numero do quarto
void createDisorderedRoomDatabase(FILE *arq, int length, int qntSwap)
{
    int vet[length];
    Room *r;

    for (int i = 0; i < length; i++)
        vet[i] = i + 1;

    shuffleRoom(vet, length, qntSwap);

    printf("\nGenerating database...\n");

    for (int i = 0; i < length; i++)
    {
        r = addRoom(vet[i], "Double", 3, 110.00, "Occupied");
        saveRoom(r, arq);
        free(r);
    }
}

void printDataBaseRoom(FILE *arq) {
    printf("Printing database...\n");

    rewind(arq);
    Room *r;

    while ((r = readRoom(arq)) != NULL)
        printRoom(r);

    free(r);
}

Room *linearSearchRoom(int key, FILE *arq)
{
    Room *r;
    clock_t start_time, end_time;
    double cpu_time_used;

    start_time = clock(); 

    rewind(arq);

    while ((r = readRoom(arq)) != NULL)
    {
        if (r->roomNumber == key)
        {
            end_time = clock();
            cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
            printf("\nEmployee found in %f seconds.\n", cpu_time_used);
            return r;
        }
        free(r);  
    }
    end_time = clock();                                                 
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC; 
    printf("\nRoom not found. Search time: %f seconds.\n", cpu_time_used);
    return NULL;
}

Room *binarySearchRoom(int numberRoom, FILE *arq, int start, int end)
{
    clock_t start_time, end_time;
    double cpu_time_used;
    start_time = clock();

    while (start <= end)
    {
        int middle = start + (end - start) / 2;
        fseek(arq, middle * lengthOfRegisterRoom(), SEEK_SET);

        Room *r = readRoom(arq);

        if (r->roomNumber == numberRoom)
        {
            end_time = clock();
            cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
            printf("Disco encontrado em %f segundos.\n", cpu_time_used);
            return r;
        }
        if (r->roomNumber > numberRoom)
        {
            end = middle - 1;
        }
        else
        {
            start = middle + 1;
        }
        free(r);
    }

    end_time = clock();                                                 // Captura o tempo final
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC; // Calcula o tempo de execução
    printf("Disco nao encontrado. Tempo de busca: %f segundos.\n", cpu_time_used);
    return NULL;
}

void changeRoom(FILE *arq)
{
    int id;
    printf("Enter an room to change: ");
    scanf("%d", &id);

    // Busca o quarto com base no numero do quarto
    Room *r = linearSearchRoom(id, arq);
    if (r == NULL)
    {
        printf("Number room %d not found!\n", id);
        return;
    }

    printf("Room found: \n");
    printRoom(r);

    // Pedir novos dados para atualizar o quarto do hotel
    printf("Enter the information: \n");
    printf("Room type: ");
    scanf("%s", r->type);
    printf("Capacity: ");
    scanf("%d", &r->capacity);
    printf("Base price: ");
    scanf("%f", &r->basePrice);

    // Reposiciona o cursor do arquivo para o início do registro
    long posicao = ftell(arq) - tamanhoRegistroDisco();
    fseek(arq, posicao, SEEK_SET);

    // Grava o room atualizado no arquivo
    saveRoom(r, arq);

    printf("Room successfuly alteraded!\n");

    // Libera a memória alocada
    free(r);
}

void bubbleSortRoom(FILE *arq, int length)
{
    Room *currentRoom = NULL;
    Room *nextRoom = NULL;
    int swapped;

    // Executa o Bubble Sort
    for (int i = 0; i < length - 1; i++)
    {
        swapped = 0;
        for (int j = 0; j < length - i - 1; j++)
        {
            // Posiciona o cursor no início do room atual
            fseek(arq, j * lengthOfRegisterRoom(), SEEK_SET);
            currentRoom = readRoom(arq);

            // Posiciona o cursor no início do próximo room
            fseek(arq, (j + 1) * lengthOfRegisterRoom(), SEEK_SET);
            nextRoom = readRoom(arq);

            // Compara os IDs dos quartos
            if (currentRoom->roomNumber > nextRoom->roomNumber)
            {
                // Troca os discos de lugar no arquivo
                fseek(arq, j * lengthOfRegisterRoom(), SEEK_SET);
                saveRoom(nextRoom, arq);

                fseek(arq, (j + 1) * lengthOfRegisterRoom(), SEEK_SET);
                saveRoom(currentRoom, arq);

                swapped = 1;
            }

            // Libera a memória alocada para os discos
            free(currentRoom);
            free(nextRoom);
        }

        // Se nenhuma troca foi feita, a lista já está ordenada
        if (!swapped)
        {
            break;
        }
    }
}