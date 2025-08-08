#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#define MAX_PARTITIONS 200
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Employee 
{
    int id;
    char name[50];
    char position[40];
    long next;  //ponteiro para o proximo registro de employee
} Employee;

//Principais
Employee *addEmployee(int id, const char *name, const char *position);

void saveEmployee(Employee *employee, FILE *arq);

Employee *readEmployee(FILE *in);

void printEmployee(Employee *employee);

//Utilitarios
int lengthOfRegisterEmployee();

int qntOfRegisterEmployee(FILE *arq);

void shuffleEmployee(int *vet, int max, int swap);

void createDisorderedEmployeeDatabase (FILE *arq, int length, int qntSwap);

void printDataBaseEmployee(FILE *arq);

//Busca e ordenação
Employee *linearSearchEmployee(int key, FILE *arq);

Employee *binarySearchEmployee(FILE *arq, int id, int start, int end);

void quickSortEmployee(FILE *arq, int low, int high);

void linkEmployeeID(FILE *arq);

void unionPartitions(int numPartitions);

int interleavePartitions(int numPartitions);

int replacementSelection(FILE *arq, int m);

bool notFrozen(bool frozen[], int length);

void writeLog(const char *filename, const char *operation, int nRecords, double time, int partitions);

int countEmployees(const char *filename);

void printHashTable(int m);

void deleteHash(int id, int length, FILE *arqEmployee);

void insertHash(int m, FILE *arq);

Employee *searchHash(int id, int length, FILE *arqEmployee);

void hashInitialize(int m);

void saveEmployeeInPosition(FILE *arq, Employee *e, long position);

Employee *readEmployeeInPosition(FILE *arq, long position);

#endif