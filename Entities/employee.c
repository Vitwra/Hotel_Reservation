#include <stdio.h>
#include <stdlib.h>
#include "employee.h"
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>

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

        current->next = (i + 1) * lengthOfRegisterEmployee(); // offset/deslocamento do proximo funcionario

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

bool notFrozen(bool frozen[], int length){
    for (int i = 0; i < length; i++)
    {
        if (!frozen[i])
        {
            return true;  //existe um registro não congelado válido
        }
    }
    return false;  //todos estão congelados ou nulos
}

//função seleção por subtituição
int replacementSelection(FILE *arq, int m) {
    Employee *copyEmployee[m];
    bool isFrozen[m];
    int savedEmployeeId;
    int minKey;
    int partitions = 0;

    rewind (arq);

    //ler m registros do arquivo para a memoria
    for (int i = 0; i < m; i++)
    {
        copyEmployee[i] = readEmployee(arq);
        isFrozen[i] = false;
    }
    
    while (true)
    {
        //cria nova partição
        char namePartition[50];
        sprintf(namePartition, "partitions/part%d.dat", partitions);

        FILE *partFile = fopen(namePartition, "w+b");
        if (partFile == NULL)
        {
            printf("Error trying to open achive.");
            return -1;
        }
        rewind(partFile);

        while (notFrozen(isFrozen, m))
        {
            minKey = -1;  //seleciona registro de menor chave
            for (int i = 0; i < m; i++)
            {
                if (!isFrozen[i] && copyEmployee[i] != NULL && 
                    (minKey == -1 || copyEmployee[i]->id < copyEmployee[minKey]->id))
                {
                    minKey = i;
                }
            }
            
            if (minKey != -1)
            {
                //gravar registro na partição de saida
                savedEmployeeId = copyEmployee[minKey]->id;
                saveEmployee(copyEmployee[minKey], partFile);

                //substituição do registro atual pelo prox do arquivo de entrada
                copyEmployee[minKey] = readEmployee(arq);

                //congelar se a chave do registro for menor que a salva
                if (copyEmployee[minKey] == NULL || savedEmployeeId > copyEmployee[minKey]->id)
                {
                    isFrozen[minKey] = true;
                }
            }
        }

        //fechar a partição de saida
        printf("\nPartition %d\n", partitions);
        printDataBaseEmployee(partFile);
        fclose(partFile);

        //Descongelar os registros congelados
        for (int i = 0; i < m; i++)
        {
            if (copyEmployee[i] != NULL)
            {
                isFrozen[i] = false;
            }
        }
        
        partitions++;

        //verifica se ainda existe registro no vetor para continuar o processo
        bool remainingRecords = false;
        for (int i = 0; i < m; i++)
        {
            if (copyEmployee[i] != NULL)
            {
                remainingRecords = true;
                break;
            }
        }
        
        if (!remainingRecords)
        {
            printf("\nFinished!");
            break;
        }
    }
    
    return partitions;
}

//função para intercalar as partições
int interleavePartitions(int totalPartitions)
{
    int countInterleavePartitions = 0;
    int sizeGroup = 4;  //definindo o tamanho do grupo (esse cm 4 partitions)
    int totalMerged = (totalPartitions + sizeGroup - 1) / sizeGroup; //calcula a quantidade de grupos
    
    char nameArqPartitions[50];
    FILE *partitions[totalPartitions];
    Employee *copyEmployee[totalPartitions];

    //abrindo cada partição para leitura
    for (int i = 0; i < totalPartitions; i++)
    {
        sprintf(nameArqPartitions, "partitions/part%d.dat", i);
        partitions[i] = fopen(nameArqPartitions, "r+b");
        if (partitions[i] == NULL)
        {
            printf("Error trying to open partitions %d\n", i);
            exit(EXIT_FAILURE);
        }
        copyEmployee[i] = readEmployee(partitions[i]); //le o funcionario de cada partição
    }
    
    //intercalação por grupos
    for (int group = 0; group < totalMerged; group++) {
        int currentLengthGroup = (group + 1) * sizeGroup > totalPartitions ? totalPartitions - group * sizeGroup : sizeGroup;
        Employee *registerGroup[currentLengthGroup * 100];  //armazena registros de grupo (100 nesse exemplo por partição)
        int idx = 0;

        //processa o grupo de partições
        for ( int i = group * sizeGroup; i < (group + 1) * sizeGroup && i < totalPartitions; i++)
        {
            while (copyEmployee[i] != NULL)
            {
                registerGroup[idx++] = copyEmployee[i];
                copyEmployee[i] = readEmployee(partitions[i]);
            }
        }
        
        //ordena os registros dentro do grupo
        for (int i = 0; i < idx - 1; i++)
        {
            for (int j = i + 1; j < idx; j++)
            {
                if (registerGroup[i]->id > registerGroup[j]->id)
                {
                    Employee * temp = registerGroup[i];
                    registerGroup[i] = registerGroup[j];
                    registerGroup[j] = temp;
                }
            }
        }
        
        //Gerar uma nova partição com os registros intercalados do grupo
        sprintf(nameArqPartitions, "mergePart/partMerged%d.dat", countInterleavePartitions);
        countInterleavePartitions++;
        FILE *output = fopen(nameArqPartitions, "w+b");
        if (output == NULL)
        {
            printf("\nError trying to create an output file.\n");
            exit(EXIT_FAILURE);
        }

        //salva o grupo de registros intercalados na nova partição
        for (int i = 0; i < idx; i++)
        {
            saveEmployee(registerGroup[i], output);
        }

        fclose(output);
    }

    //fechando os arquivos das partições originais
    for (int i = 0; i < totalPartitions; i++)
    {
        fclose(partitions[i]);
    }

    return countInterleavePartitions;
}

//função para unir as partições ordenadas
void unionPartitions(int numPartitions) {
    FILE *outputFinal = fopen("mergePart/output_final_sorted.dat", "w+b");
    if (outputFinal == NULL) {
        printf("\nError trying to create an final output sorted.\n");
        exit(1);
    }

    Employee *copyEmployee[numPartitions];
    FILE *partitions[numPartitions];

    // Abrir todas as partições
    for (int i = 0; i < numPartitions; i++) {
        char nameArqPartition[50];
        sprintf(nameArqPartition, "mergePart/partMerged%d.dat", i);  // começa em 0 para nome do arquivo
        partitions[i] = fopen(nameArqPartition, "rb");
        if (partitions[i] == NULL) {
            printf("Error trying to open file partition %s\n", nameArqPartition);
            exit(1);
        }

        copyEmployee[i] = readEmployee(partitions[i]); //le o primeiro registro de cada partição
    }

    // Processo de merge para unir as partições intercaladas
    while (1) {
        int minId = INT_MAX;
        int idxMin = -1;

        // Encontrar o menor funcionário entre as partições
        for (int i = 0; i < numPartitions; i++) {
            if (copyEmployee[i] != NULL && copyEmployee[i]->id < minId) {
                minId = copyEmployee[i]->id;
                idxMin = i;
            }
        }

        if (idxMin == -1) {
            // Todos os registros foram unidos
            break;
        }

        // Salvar o menor funcionário no arquivo final
        saveEmployee(copyEmployee[idxMin], outputFinal);

        // Ler o próximo funcionário da partição correspondente
        copyEmployee[idxMin] = readEmployee(partitions[idxMin]);
    }

    // Fechar e remover as partições
    for (int i = 0; i < numPartitions; i++) {
        fclose(partitions[i]);
        char nameArqPartition[50];
        sprintf(nameArqPartition, "mergePart/partMerged%d.dat", i);  
        remove(nameArqPartition); //remove as partições que foram criadas temporariamente  
    }

    fclose(outputFinal);

    // Abrir o arquivo final ordenado para impressão
    FILE *outputFinalSorted = fopen("mergePart/output_final_sorted.dat", "r+b");
    if (outputFinalSorted == NULL) {
        printf("\nError trying to open output file sorted for print.\n");
        exit(1);
    }

    // Imprimir o conteúdo do arquivo final ordenado
    printf("\nOUTPUT SORTED\n");
    printDataBaseEmployee(outputFinalSorted);

    fclose(outputFinalSorted);
}

void writeLog(const char *filename, const char *operation, int nRecords, double time, int partitions) {
    FILE *log = fopen(filename, "a"); // "a" para adicionar sem apagar o conteúdo anterior
    if (log == NULL) {
        printf("Erro ao abrir log.\n");
        return;
    }

    fprintf(log, "Operação: %s\n", operation);
    fprintf(log, "Registros: %d\n", nRecords);
    fprintf(log, "Tempo: %.4lf segundos\n", time);
    fprintf(log, "Partições: %d\n", partitions);
    fprintf(log, "--------------------------\n");

    fclose(log);
}

int countEmployees(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) return 0;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);

    return size / sizeof(Employee);
}
