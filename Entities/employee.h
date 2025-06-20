#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Employee 
{
    int id;
    char name[50];
    char position[40];
} Employee;

//Principais
Employee *addEmployee(int id, const char *name, const char *position);

void saveEmployee(Employee *employee, FILE *arq);

Employee *readEmployee(FILE *in);

void printEmployee(Employee *employee);

//Utilitarios
int lengthOfRegisterEmployee();

int qntOfRegisterEmployee(FILE *arq);

void shuffleEmployee(int *vet, int max, int switchs);

void madeDisorderedBaseEmployee (FILE *arq, int length, int qntSwitchs);

void generateRandomName(char *name, int lenght);

void printDataBaseEmployee(FILE *arq);

//Busca e ordenação
Employee *linearSearchEmployee(int key, FILE *arq);

Employee *binarySearchEmployee(FILE *arq, int id, int start, int end);

void bubbleSortEmployee(FILE *arq, int length);

#endif