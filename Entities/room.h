#ifndef ROOM_H
#define ROOM_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Room {
    int roomNumber;
    char type[20];        
    int capacity;
    float basePrice;
    char status[10];      
} Room;

Room *addRoom(int roomNumber, const char *type, int capacity, float basePrice, const char *status);

void saveRoom(Room *room, FILE *arq);

Room *readRoom(FILE *arq);

void printRoom(Room *room);

int lengthOfRegisterRoom();

int qntOfRegisterRoom(FILE *arq);

void shuffleRoom(int *vet, int max, int swap);

void createDisorderedRoomDatabase(FILE *arq, int length, int qntSwap);

void printDataBaseRoom(FILE *arq);

Room *linearSearchRoom(int roomNumber, FILE *arq);

Room *binarySearchRoom(int numberRoom, FILE *arq, int start, int end);

void changeRoom(FILE *arq);

void bubbleSortRoom(FILE *arq, int length);

#endif