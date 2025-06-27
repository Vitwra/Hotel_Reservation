#include <stdio.h>
#include <stdlib.h>
#include "employee.h"
#include <string.h>
#include <time.h>
#include <math.h>

// Adiciona novo colaborador
Employee *addEmployee(int id, const char *name, const char *position)
{
    Employee *employee = (Employee *)malloc(sizeof(Employee));

    if (employee)
    {
        memset(employee, 0, sizeof(Employee)); // Zera a memoria

        employee->id = id;

        // Copia valores de string para os campos de employee
        strncpy(employee->name, name, sizeof(employee->name) - 1);
        strncpy(employee->position, position, sizeof(employee->position) - 1);
        employee->next = -1; // incicializa o prox employee como -1, indicando NULL
    }
    return employee;
}

// Salva colaborador no arquivo arq, na posição do cursor
void saveEmployee(Employee *employee, FILE *arq)
{
    fwrite(&employee->id, sizeof(int), 1, arq);
    fwrite(employee->name, sizeof(char), sizeof(employee->name), arq);
    fwrite(employee->position, sizeof(char), sizeof(employee->position), arq);

    // salva nextID como long
    long nextID = employee->next;
    fwrite(&nextID, sizeof(long), 1, arq);
}

// Le um colaborador do arquivo na posição atual
Employee *readEmployee(FILE *in)
{
    Employee *employee = (Employee *)malloc(sizeof(Employee));

    if (fread(&employee->id, sizeof(int), 1, in) != 1)
    {
        free(employee);
        return NULL;
    }
    fread(employee->name, sizeof(char), sizeof(employee->name), in);
    fread(employee->position, sizeof(char), sizeof(employee->position), in);
    fread(&employee->next, sizeof(long), 1, in);

    return employee;
}

// imprime colaborador
void printEmployee(Employee *employee)
{
    printf("**********************************************\n");
    printf("Employee ID: %d\n", employee->id);
    printf("Name: %s\n", employee->name);
    printf("Position in the company: %s\n", employee->position);

    // imprime valor do prox ponteiro
    printf("Next employee address (offset bytes): %ld\n", employee->next);
    printf("**********************************************\n");
}

// Retorna o tamanho de employee em bytes
int lengthOfRegisterEmployee()
{
    return sizeof(int)         // id
           + sizeof(char) * 50 // nome
           + sizeof(char) * 40 // cargo
           + sizeof(long);     // proximo employee
}

// Retorna a quantidade de registros no arquivo
int qntOfRegisterEmployee(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    int length = ftell(arq) / lengthOfRegisterEmployee();
    return length;
}

// Embaralha base de dados
void shuffleEmployee(int *vet, int max, int swap)
{
    for (int i = 0; i < swap; i++)
    {
        int j = rand() % max;
        int k = rand() % max;

        int tmp = vet[j];
        vet[j] = vet[k];
        vet[k] = tmp;
    }
}

// Cria base de dados desordenada
void createDisorderedEmployeeDatabase(FILE *arq, int length, int qntSwap)
{
    int vet[length];
    Employee *e;

    const char *position[] = {
        "Receptionist",
        "Concierge",
    };
    int numOfPosition = sizeof(position) / sizeof(position[0]);

    // Inicializa aleatoriedade
    srand(time(NULL));

    for (int i = 0; i < length; i++)
        vet[i] = i + 1;

    shuffleEmployee(vet, length, qntSwap);

    printf("\nGenerating database...\n");

    for (int i = 0; i < length; i++)
    {
        // Sorteia um cargo do vetor
        const char *randomposition = position[rand() % numOfPosition];

        // Cria e salva o funcionário
        e = addEmployee(vet[i], "anonymous", randomposition);
        saveEmployee(e, arq);
        free(e);
    }
}

// Imprime a base de dados
void printDataBaseEmployee(FILE *arq)
{
    printf("\nPrinting database...\n");

    rewind(arq);
    Employee *e;

    while ((e = readEmployee(arq)) != NULL)
    {
        printEmployee(e);
        free(e);
    }
}

// Algoritmo de Busca Sequencial/linear
Employee *linearSearchEmployee(int key, FILE *arq)
{
    Employee *e;
    clock_t start_time, end_time;
    double cpu_time_used;

    start_time = clock(); // tempo inicial

    rewind(arq);

    while ((e = readEmployee(arq)) != NULL)
    {
        if (e->id == key)
        {
            end_time = clock();
            cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
            printf("\nEmployee found in %f seconds.\n", cpu_time_used);
            return e;
        }
        free(e); // LIBERAR MEMÓRIA
    }
    end_time = clock();                                                 // tempo final
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC; // Calcula o tempo de execução
    printf("\nEmployee not found. Search time: %f seconds.\n", cpu_time_used);
    return NULL;
}

// Algoritmo de Busca Binária
Employee *binarySearchEmployee(FILE *arq, int id, int start, int end)
{
    clock_t start_time, end_time;
    double cpu_time_used;
    start_time = clock(); // Captura o tempo inicial

    while (start <= end)
    {
        int middle = start + (end - start) / 2;
        fseek(arq, middle * lengthOfRegisterEmployee(), SEEK_SET);

        Employee *e = readEmployee(arq);

        if (e->id == id)
        {
            end_time = clock();                                                 // Captura o tempo final
            cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC; // Calcula o tempo de execução
            printf("\nEmployee found in %f seconds.\n", cpu_time_used);
            return e;
        }
        if (e->id > id)
        {
            end = middle - 1;
        }
        else
        {
            start = middle + 1;
        }
        free(e);
    }

    end_time = clock();                                                 // Captura o tempo final
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC; // Calcula o tempo de execução
    printf("\nEmployee not found. Search time %f seconds.\n", cpu_time_used);
    return NULL;
}

// algortimo quick para ordenar os dados no dat
void quickSortEmployee(FILE *arq, int low, int high)
{
    if (low < high)
    {
        // Escolher o pivô como último elemento
        fseek(arq, high * lengthOfRegisterEmployee(), SEEK_SET);
        Employee *pivot = readEmployee(arq);

        int i = low - 1;

        for (int j = low; j <= high - 1; j++)
        {
            fseek(arq, j * lengthOfRegisterEmployee(), SEEK_SET);
            Employee *current = readEmployee(arq);

            if (current->id <= pivot->id)
            {
                i++;

                // Trocar employees nas posições i e j
                fseek(arq, i * lengthOfRegisterEmployee(), SEEK_SET);
                Employee *empI = readEmployee(arq);

                fseek(arq, j * lengthOfRegisterEmployee(), SEEK_SET);
                Employee *empJ = readEmployee(arq);

                // Swap
                fseek(arq, i * lengthOfRegisterEmployee(), SEEK_SET);
                saveEmployee(empJ, arq);

                fseek(arq, j * lengthOfRegisterEmployee(), SEEK_SET);
                saveEmployee(empI, arq);

                free(empI);
                free(empJ);
            }
            free(current);
        }

        // Colocar pivô na posição correta (i+1)
        fseek(arq, (i + 1) * lengthOfRegisterEmployee(), SEEK_SET);
        Employee *empPivotPos = readEmployee(arq);

        fseek(arq, high * lengthOfRegisterEmployee(), SEEK_SET);
        Employee *empHigh = readEmployee(arq);

        fseek(arq, (i + 1) * lengthOfRegisterEmployee(), SEEK_SET);
        saveEmployee(empHigh, arq);

        fseek(arq, high * lengthOfRegisterEmployee(), SEEK_SET);
        saveEmployee(empPivotPos, arq);

        free(empPivotPos);
        free(empHigh);
        free(pivot);

        int pi = i + 1;

        // Recursão para as duas partições
        quickSortEmployee(arq, low, pi - 1);
        quickSortEmployee(arq, pi + 1, high);
    }
}

void linkEmployeeID(FILE *arq)
{
    int total = qntOfRegisterEmployee(arq);
    Employee *current;

    for (int i = 0; i < total - 1; i++)
    {
        fseek(arq, i * lengthOfRegisterEmployee(), SEEK_SET);
        current = readEmployee(arq);

        current->next = (i + 1) * lengthOfRegisterEmployee(); // offset do próximo funcionário

        fseek(arq, i * lengthOfRegisterEmployee(), SEEK_SET);
        saveEmployee(current, arq);

        free(current);
    }

    // último aponta para -1
    fseek(arq, (total - 1) * lengthOfRegisterEmployee(), SEEK_SET);
    current = readEmployee(arq);
    current->next = -1;
    fseek(arq, (total - 1) * lengthOfRegisterEmployee(), SEEK_SET);
    saveEmployee(current, arq);
    free(current);
}