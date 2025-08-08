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

// funcao para verificar se existe pelo menos um registro n congelado
bool notFrozen(bool frozen[], int length)
{
    for (int i = 0; i < length; i++)
    {
        if (!frozen[i])
        {
            return true; // existe um registro não congelado válido
        }
    }
    return false; // todos estão congelados ou nulos
}

// função seleção por subtituição
int replacementSelection(FILE *arq, int m)
{
    Employee *copyEmployee[m];
    bool isFrozen[m];
    int savedEmployeeId;
    int minKey;
    int partitions = 0;

    // reposiciona o ponteiro para leitura
    rewind(arq);

    // ler m registros do arquivo para a memoria
    for (int i = 0; i < m; i++)
    {
        copyEmployee[i] = readEmployee(arq);
        isFrozen[i] = false;
    }

    while (true)
    {
        // cria nova partição
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
            minKey = -1; // seleciona registro de menor chave

            for (int i = 0; i < m; i++)
            {
                // Seleciona o índice i do registro com menor id entre os registros disponíveis
                if (!isFrozen[i] && copyEmployee[i] != NULL &&
                    (minKey == -1 || copyEmployee[i]->id < copyEmployee[minKey]->id))
                {
                    minKey = i;
                }
            }

            // Comparação e substituição de registro para ordenar em um vetor
            if (minKey != -1)
            {
                // gravar registro na partição de saida
                savedEmployeeId = copyEmployee[minKey]->id;
                saveEmployee(copyEmployee[minKey], partFile);

                // substituição do registro atual pelo prox do arquivo de entrada
                copyEmployee[minKey] = readEmployee(arq);

                // congelar se a chave do registro for menor que a salva
                if (copyEmployee[minKey] == NULL || savedEmployeeId > copyEmployee[minKey]->id)
                {
                    isFrozen[minKey] = true;
                }
            }
        }

        // fechar a partição de saida
        printf("\nPartition %d\n", partitions);
        printDataBaseEmployee(partFile);
        fclose(partFile);

        // Descongelar os registros congelados
        for (int i = 0; i < m; i++)
        {
            if (copyEmployee[i] != NULL)
            {
                isFrozen[i] = false;
            }
        }

        partitions++;

        // verifica se ainda existe registro no ARQ para continuar o processo
        bool remainingRecords = false;
        for (int i = 0; i < m; i++)
        {
            if (copyEmployee[i] != NULL)
            {
                remainingRecords = true;
                break;
            }
        }

        // para quando todos os registro do arq foram lidos
        if (!remainingRecords)
        {
            printf("\nFinished!");
            break;
        }
    }

    return partitions;
}

// função para intercalar as partições
int interleavePartitions(int totalPartitions)
{
    int countInterleavePartitions = 0;                               // contador de partições intercaladas
    int sizeGroup = 4;                                               // definindo o tamanho do grupo (esse cm 4 partitions)
    int totalMerged = (totalPartitions + sizeGroup - 1) / sizeGroup; // calcula a quantidade de grupos

    char nameArqPartitions[50];
    FILE *partitions[totalPartitions];
    Employee *copyEmployee[totalPartitions];

    // abrindo cada partição para leitura
    for (int i = 0; i < totalPartitions; i++)
    {
        sprintf(nameArqPartitions, "partitions/part%d.dat", i);
        partitions[i] = fopen(nameArqPartitions, "r+b");
        if (partitions[i] == NULL)
        {
            printf("Error trying to open partitions %d\n", i);
            exit(EXIT_FAILURE);
        }
        copyEmployee[i] = readEmployee(partitions[i]); // le o funcionario de cada partição
    }

    // intercalação por grupos
    for (int group = 0; group < totalMerged; group++)
    {
        int currentLengthGroup = (group + 1) * sizeGroup > totalPartitions ? totalPartitions - group * sizeGroup : sizeGroup;
        Employee *registerGroup[currentLengthGroup * 100]; // armazena registros de grupo (100 nesse exemplo por partição)
        int idx = 0;

        // processa o grupo de partições (ler o registro e comparar)
        for (int i = group * sizeGroup; i < (group + 1) * sizeGroup && i < totalPartitions; i++)
        {
            while (copyEmployee[i] != NULL)
            {
                registerGroup[idx++] = copyEmployee[i];
                copyEmployee[i] = readEmployee(partitions[i]);
            }
        }

        // ordena os registros dentro do grupo
        for (int i = 0; i < idx - 1; i++)
        {
            for (int j = i + 1; j < idx; j++)
            {
                if (registerGroup[i]->id > registerGroup[j]->id)
                {
                    Employee *temp = registerGroup[i];
                    registerGroup[i] = registerGroup[j];
                    registerGroup[j] = temp;
                }
            }
        }

        // Gerar uma nova partição com os registros intercalados do grupo
        sprintf(nameArqPartitions, "mergePart/partMerged%d.dat", countInterleavePartitions);
        countInterleavePartitions++;
        FILE *output = fopen(nameArqPartitions, "w+b");
        if (output == NULL)
        {
            printf("\nError trying to create an output file.\n");
            exit(EXIT_FAILURE);
        }

        // salva o grupo de registros intercalados na nova partição
        for (int i = 0; i < idx; i++)
        {
            saveEmployee(registerGroup[i], output);
        }

        fclose(output);
    }

    // fechando os arquivos das partições originais
    for (int i = 0; i < totalPartitions; i++)
    {
        fclose(partitions[i]);
    }

    return countInterleavePartitions;
}

// função para unir as partições ordenadas
void unionPartitions(int numPartitions)
{

    // cria arq com saida final e verifica se n esta vazio
    FILE *outputFinal = fopen("mergePart/output_final_sorted.dat", "w+b");
    if (outputFinal == NULL)
    {
        printf("\nError trying to create an final output sorted.\n");
        exit(1);
    }

    Employee *copyEmployee[numPartitions];
    FILE *partitions[numPartitions];

    // Abrir todas as partições
    for (int i = 0; i < numPartitions; i++)
    {
        char nameArqPartition[50];
        sprintf(nameArqPartition, "mergePart/partMerged%d.dat", i); // começa em 0 para nome do arquivo
        partitions[i] = fopen(nameArqPartition, "rb");
        if (partitions[i] == NULL)
        {
            printf("Error trying to open file partition %s\n", nameArqPartition);
            exit(1);
        }

        copyEmployee[i] = readEmployee(partitions[i]); // le o primeiro registro de cada partição
    }

    // Processo de merge para unir as partições intercaladas
    while (1)
    {
        int minId = INT_MAX;
        int idxMin = -1;

        // procura o menor ID funcionário entre as partições
        for (int i = 0; i < numPartitions; i++)
        {
            if (copyEmployee[i] != NULL && copyEmployee[i]->id < minId)
            {
                minId = copyEmployee[i]->id;
                idxMin = i;
            }
        }

        if (idxMin == -1)
        {
            // Todos os registros foram unidos
            break;
        }

        // Salvar o menor funcionário no arquivo final
        saveEmployee(copyEmployee[idxMin], outputFinal);

        // Ler o próximo funcionário da partição correspondente
        copyEmployee[idxMin] = readEmployee(partitions[idxMin]);
    }

    // Fechar e remover as partições
    for (int i = 0; i < numPartitions; i++)
    {
        fclose(partitions[i]);
        char nameArqPartition[50];
        sprintf(nameArqPartition, "mergePart/partMerged%d.dat", i);
        remove(nameArqPartition); // remove as partições que foram criadas temporariamente
    }

    fclose(outputFinal);

    // Abrir o arquivo final ordenado para impressão
    FILE *outputFinalSorted = fopen("mergePart/output_final_sorted.dat", "r+b");
    if (outputFinalSorted == NULL)
    {
        printf("\nError trying to open output file sorted for print.\n");
        exit(1);
    }

    // Imprimir o conteúdo do arquivo final ordenado
    printf("\nOUTPUT SORTED\n");
    printDataBaseEmployee(outputFinalSorted);

    fclose(outputFinalSorted);
}

void writeLog(const char *filename, const char *operation, int nRecords, double time, int partitions)
{
    FILE *log = fopen(filename, "a"); // "a" para adicionar sem apagar o conteúdo anterior
    if (log == NULL)
    {
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
    if (!fp)
        return 0;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);

    return size / sizeof(Employee);
}

// Função para ler um funcionario a partir de uma posição especifica do arquivo
Employee *readEmployeeInPosition(FILE *arq, long position)
{

    // Aloca memoria do tamanho de Employee
    Employee *e = (Employee *)malloc(sizeof(Employee));

    // Veirfica se foi a locado memoria
    if (e == NULL)
    {
        return NULL;
    }

    // Move o cursor para a posição deseada
    fseek(arq, position, SEEK_SET);

    // le os dados do funcionario
    if (fread(&e->id, sizeof(int), 1, arq) < 1)
    {
        free(e);
        return NULL; // se n puder ler libera a memoria e retorna NULL
    }

    fread(e->name, sizeof(char), sizeof(e->name), arq);
    fread(e->position, sizeof(char), sizeof(e->position), arq);
    fread(&e->next, sizeof(long), 1, arq);

    return e;
}

// função para salvar o funcionario na posição especifica
void saveEmployeeInPosition(FILE *arq, Employee *e, long position)
{

    // verifica se os arquivos de employee.dat são validos
    if (arq == NULL || e == NULL)
    {
        printf("\nError trying to open file.\n");
        return;
    }

    // move o cursor para a posição selecionada
    fseek(arq, position, SEEK_SET);

    // grava os dados
    fwrite(&e->id, sizeof(int), 1, arq);
    fwrite(e->name, sizeof(char), sizeof(e->name), arq);
    fwrite(e->position, sizeof(char), sizeof(e->position), arq);
    fwrite(&e->next, sizeof(long), 1, arq);

    // força gravar o arq
    fflush(arq);

    // mensagem para mostrar ao usuario que a operação foi bem sucessida
    printf("Successfuly! register has been change in position %ld! \n", position);
}

// função para criar tabela hash
int hash(int id, int length)
{

    return id % length;
}

// função para incializar função hash
void hashInitialize(int m)
{

    // abre o arquivo para leitura
    FILE *table = fopen("hashTable.dat", "w+b");
    if (table == NULL)
    {
        printf("\nError trying to open file table.\n");
        exit(1);
    }

    // incia variavel de valor inicial
    long empty = -1;

    // percorre todo o arq Hash
    for (int i = 0; i < m; i++)
    {
        fseek(table, i * sizeof(long), SEEK_SET);
        fwrite(&empty, sizeof(long), 1, table);
    }

    fclose(table);
}

// função para buscar um fncionario na tabela hash
Employee *searchHash(int id, int length, FILE *arqEmployee)
{

    long positionEmployeeArq;
    long positionHash;
    Employee *e = NULL;
    Employee *employeeAux = NULL;
    long aux;

    // cria arquivo
    FILE *hashTable = fopen("hashTable.dat", "r+b");

    // verifica se o arq esta vazio
    if (hashTable == NULL)
    {
        printf("Error trying to open file Hash Table.");
        return NULL;
    }

    // calcula a posição inicial na tabela hash
    positionHash = hash(id, length);

    // mover para a posição calculada e leitura da posição do funcionario no arq
    fseek(hashTable, positionHash * sizeof(long), SEEK_SET);
    fread(&positionEmployeeArq, sizeof(long), 1, hashTable);

    // verifica se existe um funcionario na position informada
    if (positionEmployeeArq == -1)
    {
        printf("\nId employee %d not found in hash table.\n", id);
        fclose(hashTable);
        return NULL;
    }

    // retorna para o usuario a posição inical do arquivo
    printf("\nInitial position in the file: %ld\n", positionEmployeeArq);

    // leitura do registro na posição indicada
    e = readEmployeeInPosition(arqEmployee, positionEmployeeArq);

    // verifica se o registro esta vazio e se existe um correspondente
    if (e != NULL && e->id == id)
    {
        printf("Employee found in initial position: \n\n");
        printEmployee(e);
        fclose(hashTable);
        return e;
    }

    // verifica se o funcionario existe e faz aux receber o campo next de employee
    if (e != NULL && e->id == id)
    {
        printEmployee(e);
        aux = e->next;

        if (aux == -1)
        {
            return e;
        }

        // Se o ID não bater, continuar a busca nos próximos elementos

        // free(f);  // Liberar a memória alocada para o funcionário inicial

        // while para realizar a busca ate encontrar o ID desejado ou até que o fim da lista seja alcaçado
        while (aux != -1)
        {
            // leitura do arq EmployeeAux
            employeeAux = readEmployeeInPosition(arqEmployee, aux);

            // verifica se o arq esta vazio
            if (hashTable == NULL)
            {
                printf("\nError trying to read employee in position %ld.\n\n Exiting search...\n", aux);
                fclose(hashTable);
                return NULL;
            }

            // imprimindo registro do funcionario
            printf("Searching employee in position: %ld\n", aux);
            printEmployee(employeeAux);

            // encontrou o funcionario com o ID desejado
            if (employeeAux->id == id)
            {
                printf("Employee id %d found!\n", id);
                fclose(hashTable);
                return employeeAux;
            }

            // atualiza aux para a posição do proximo funcionario na lista encadeada
            aux = employeeAux->next;
            free(employeeAux);
        }
        return e;
    }

    // se não encontrar o funcionario
    printf("Id employee %d not found.\n", id);
    fclose(hashTable);
    return NULL;
}

// função para inserir na tabela hash
void insertHash(int m, FILE *arq)
{
    Employee *e = NULL;
    Employee *eSearch = NULL;
    Employee *employeeAux = NULL;
    long positionEmployeeArq;
    int positionHash;
    long aux;

    // cria arq tabela
    FILE *table = fopen("hashTable.dat", "r+b");

    // verifica se o arq é vazio
    if (table == NULL)
    {
        printf("\nError trying to open hashTable.\n");
        exit(1);
    }

    // volta para o inicio de funcionarios
    rewind(arq);

    // verifica se o registro atual não é vazio e então imprime, calcula e busca na tabela
    while ((e = readEmployee(arq)) != NULL)
    {
        positionEmployeeArq = ftell(arq) - lengthOfRegisterEmployee();
        int employeeId = e->id;
        printf("\nID employee: %d, position in the file: %ld\n", employeeId, positionEmployeeArq);

        // calcula a posição na hash
        positionHash = hash(employeeId, m);
        printf("\nPosition hash for ID %d: %d\n", employeeId, positionHash);

        // busca na tabela hash se o ID esta presente
        eSearch = searchHash(employeeId, m, arq);

        // verifica se existe e grava na tabela
        if (eSearch == NULL)
        {
            fseek(table, positionHash * sizeof(long), SEEK_SET);
            fwrite(&positionEmployeeArq, sizeof(long), 1, table);
            fflush(table);
            printf("\nSave in position %d complete.\n\n", positionHash);
        }
        
        //operação que verifica se já existe um funcionário com esse ID na tabela hash
        else
        {
            printf("\nId employee %d already exist in hash table.\n", employeeId);
            printf("Inserindo na lista encadeada...\n");

            if (eSearch->id != employeeId)
            {
                // inserir na lista encadeada se ja existir um funcionario com ID diferente
                aux = eSearch->next;

                // caso seja -1, significa que é o final da lista
                if (aux == -1)
                {
                    eSearch->next = positionEmployeeArq;
                    saveEmployeeInPosition(arq, eSearch, ftell(arq) - lengthOfRegisterEmployee());
                    printf("\nInserindo na lista encadeada na posicao: %ld\n", eSearch->next);
                }
                else
                {
                    // percorre a lista ate encontrar o final
                    while (aux != -1)
                    {
                        employeeAux = readEmployeeInPosition(arq, aux);

                        if (employeeAux == NULL)
                        {
                            printf("\nError trying to read employee in position %ld. \nExiting...", aux);
                            break;
                        }

                        // atualiza aux para o proximo employee na lista
                        aux = employeeAux->next;

                        // libera a memoria anterior para evitar vazamento
                        if (aux != -1)
                        {
                            free(employeeAux);
                        }
                    }

                    //caso o employeeAux é o ultimo
                    if (employeeAux != NULL)
                    {
                        // inserir ao final da lista encadeada
                        employeeAux->next = positionEmployeeArq;

                        // salva o ultimo funcionario atualizado na sua posição original
                        saveEmployeeInPosition(arq, employeeAux, ftell(arq) - lengthOfRegisterEmployee());
                        printf("Inserido na lista encadeada na posição: %ld\n", employeeAux->next);

                        // libera memoria para employeeAux
                        free(employeeAux);
                    }
                }
            }
        }
    }

    // libera arq
    free(e);
    free(eSearch);

    fclose(table);
}

// função para excluir hash
void deleteHash(int id, int length, FILE *arqEmployee)
{

    long positionEmployeeArq;
    long positionHash = hash(id, length);
    Employee *e = NULL;
    Employee *anterior = NULL;

    // cria arq e verifica de esta vazio
    FILE *hashTable = fopen("hashTable.dat", "r+b");

    if (hashTable == NULL)
    {
        printf("Error trying to open hashTable.dat.\n");
        exit(1);
    }

    // ler a posição do funcionario na tabela hash
    fseek(hashTable, positionHash * sizeof(long), SEEK_SET);
    fread(&positionEmployeeArq, sizeof(long), 1, hashTable);

    if (positionEmployeeArq == -1)
    {
        printf("Id employee %d not found in the table.\n", id);
        fclose(hashTable);
        return;
    }

    // ler o primeiro funcionario na posição incial
    e = readEmployeeInPosition(arqEmployee, positionEmployeeArq);

    // caso o funcionario seja o primeiro da posição hash
    if (e != NULL && e->id == id)
    {
        if (e->next == -1)
        {
            // Caso não haja mais ninguem na lista, marca a posição como vazia
            long voidValue = -1;
            fseek(hashTable, positionHash * sizeof(long), SEEK_SET);
            fwrite(&voidValue, sizeof(long), 1, hashTable);
            printf("\nId employee %d remove from first position.\n", id);
        }
        else
        {
            // atualiza a posição inicial para o prox funcionario na lista
            fseek(hashTable, positionHash * sizeof(long), SEEK_SET);
            fwrite(&e->next, sizeof(long), 1, hashTable);
            printf("\nId employee %d remove from first position.\n New first position and ID is %ld\n", id, e->next);
        }
    }
    else
    {
        // se não for o primeiro, buscar o elemento na lista encadeada
        long anteriorPosition = -1;
        while (e != NULL && e->id == id)
        {
            anteriorPosition = ftell(arqEmployee); // armazena a posição atual do anterior
            anterior = e;
            e = readEmployeeInPosition(arqEmployee, e->next);
        }

        if (e == NULL)
        {
            printf("\nId employee %d not found.\n", id);
            fclose(hashTable);
            return;
        }

        // encontrou o funcionario, ajusta a lista
        anterior->next = e->next;

        // salva as alterações anteriores no arquivo
        saveEmployeeInPosition(arqEmployee, anterior, anteriorPosition);
        printf("\nId employee %d remove from the list.\n", id);
    }

    free(e);
    fclose(hashTable);
}

// função para imprimir a tabela hash
void printHashTable(int m)
{

    long position;

    // abre arq da tabela hash
    FILE *table = fopen("hashTable.dat", "r+b");
    if (table == NULL)
    {
        printf("Error trying to open hashTable.dat para leitura.\n");
        exit(1);
    }

    printf("\nHash table:\n");
    for (int i = 0; i < m; i++)
    {
        // Move o ponteiro do arquivo para a posição correspondente no arq
        fseek(table, i * sizeof(long), SEEK_SET);

        // Le o valor da posição (offset) do funcionario
        fread(&position, sizeof(long), 1, table);

        // Exibir a posição e o valor
        printf("indice %d: ", i + 1);
        if (position == -1)
        {
            printf("No register\n");
        }
        else
        {
            printf("Employee position: %ld\n", position);
        }
    }

    fclose(table);
}