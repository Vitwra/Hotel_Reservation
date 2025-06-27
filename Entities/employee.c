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

        // Copia valores de string para os campos do disco
        strncpy(employee->name, name, sizeof(employee->name) - 1);
        strncpy(employee->position, position, sizeof(employee->position) - 1);
        employee->next = -1; //incicializa o prox employee como -1, indicando NULL
    }
    return employee;
}

// Salva colaborador no arquivo arq, na posição do cursor
void saveEmployee(Employee *employee, FILE *arq)
{
    fwrite(&employee->id, sizeof(int), 1, arq);
    fwrite(employee->name, sizeof(char), sizeof(employee->name), arq);
    fwrite(employee->position, sizeof(char), sizeof(employee->position), arq);

    //salva nextID como long
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

    //imprime valor do prox ponteiro
    printf("Next employee adress: %ld\n", employee->next);
    printf("**********************************************\n");
}

// Retorna o tamanho do disco em bytes
int lengthOfRegisterEmployee()
{
    return sizeof(int)          // id
           + sizeof(char) * 50  // nome
           + sizeof(char) * 40 // cargo
           + sizeof(long);    //proximo employee
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

// Algoritmo Bubble Sort para ordenar a base de dados
void bubbleSortEmployee(FILE *arq, int length)
{
    Employee *currentEmployee = NULL;
    Employee *nextEmployee = NULL;
    int swapped;

    for (int i = 0; i < length - 1; i++)
    {
        swapped = 0;
        for (int j = 0; j < length - i - 1; j++)
        {
            // Posiciona o cursor no início do colaborador atual
            fseek(arq, j * lengthOfRegisterEmployee(), SEEK_SET);
            currentEmployee = readEmployee(arq);

            // Posiciona o cursor no início do próximo colaborador
            fseek(arq, (j + 1) * lengthOfRegisterEmployee(), SEEK_SET);
            nextEmployee = readEmployee(arq);

            // Compara os IDs dos colaboradores
            if (currentEmployee->id > nextEmployee->id)
            {
                // Troca os funcionarios de lugar no arquivo
                fseek(arq, j * lengthOfRegisterEmployee(), SEEK_SET);
                saveEmployee(nextEmployee, arq);

                fseek(arq, (j + 1) * lengthOfRegisterEmployee(), SEEK_SET);
                saveEmployee(currentEmployee, arq);

                swapped = 1;
            }

            // Libera a memória alocada para os funcionarios
            free(currentEmployee);
            free(nextEmployee);
        }

        // Se nenhuma troca foi feita, a lista já está ordenada
        if (!swapped)
        {
            break;
        }
    }
}