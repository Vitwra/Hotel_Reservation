#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Entities/client.h"
#include "../Entities/employee.h"
#include "../Entities/order.h"
#include "../Entities/room.h"
#include <time.h>

int main()
{
    FILE *orders;
    FILE *clients;
    FILE *employees;
    FILE *rooms;
    int option;
    srand(time(NULL));

    if ((employees = fopen("employee.dat", "r+b")) == NULL) // abre para leitura e escrita sem apagar o conteúdo, caso contrário, executa w+b
    {
        if ((employees = fopen("employee.dat", "w+b")) == NULL)
        {
            printf("Error opening employee file\n");
            exit(1);
        }
    }
    if ((clients = fopen("client.dat", "r+b")) == NULL)
    {
        if ((clients = fopen("client.dat", "w+b")) == NULL)
        {
            printf("Error opening client file\n");
            exit(1);
        }
    }
    if ((orders = fopen("order.dat", "r+b")) == NULL)
    {
        if ((orders = fopen("order.dat", "w+b")) == NULL)
        {
            printf("Error opening order file\n");
            exit(1);
        }
    }
    if ((rooms = fopen("room.dat", "r+b")) == NULL)
    {
        if ((rooms = fopen("room.dat", "w+b")) == NULL)
        {
            printf("Error opening room file\n");
            exit(1);
        }
    }

    Employee *e;
    Client *c;
    Order *o;
    Room *r;
    int qntOfRegister;
    int qntSwap;
    int length;
    int idRegister;
    char reg[15];
    int idSearch;
    int idOrder;

    do
    {
        printf("\n\n>>>>>>>>>>>>>>>>>>>>>>> HOTEL RESERVATION <<<<<<<<<<<<<<<<<<<");
        printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>> MENU OPTIONS <<<<<<<<<<<<<<<<<<<<\n");

        printf("\n>>>>>>>>>>>>>>>>>>>>>>> Room <<<<<<<<<<<<<<<<<<<\n");
        printf("\n1 - Create database disordered room");
        printf("\n2. Register an room");
        printf("\n3. Search room (Linear)");
        printf("\n4. Search room (Binary)");
        printf("\n5. Modify room informations");
        printf("\n6. Sort room");

        printf("\n>>>>>>>>>>>>>>>>>>>>>>> Employee <<<<<<<<<<<<<<<<<<<\n");
        printf("\n7 - Create database disordered employee");
        printf("\n8 - Register an employee");
        printf("\n9 - Search employee (Linear)");
        printf("\n10 - Search employee (Binary)");
        printf("\n11 - Sort employees");

        printf("\n\n>>>>>>>>>>>>>>>>>>>>>>> Client <<<<<<<<<<<<<<<<<<<\n");
        printf("\n12 - Create disordered database client");
        printf("\n13 - Register an client");
        printf("\n14 - Search client (Linear)");

        printf("\n\n>>>>>>>>>>>>>>>>>>>>>>> Order <<<<<<<<<<<<<<<<<<<\n");
        printf("\n15 - Register an order");
        printf("\n16 - Search an order");
        printf("\n17 - Generate a list of orders made");
        printf("\n18 - Generate a list of orders by employee");

        printf("\n\n0 - Exit\n");

        printf("\nEnter an option: ");

        if (scanf("%d", &option) != 1)
        {
            printf("Invalid input. Try again.\n");
            while (getchar() != '\n')
                ;
            continue;
        };

        switch (option)
        {
        case 0:
            printf("Exiting...\n");
            break;

        case 1:
            printf("\nCreating disordered database room.");
            rooms = fopen("rooms.dat", "w+b"); // para evitar criar base de dados em uma base ja existente
            if (rooms == NULL)
            {
                printf("\nError reopening rooms.dat\n");
                exit(1);
            }
            printf("\nEnter a quantity of register you want to create: ");
            scanf("%d", &qntOfRegister);
            printf("\nEnter a quantity of swaps you want: ");
            scanf("%d", &qntSwap);
            createDisorderedRoomDatabase(rooms, qntOfRegister, qntSwap);
            printDataBaseRoom(rooms);
            printf("\nQuantity of register: %d", qntOfRegisterRoom(rooms));
            break;

        case 2:
            printf("\nRegistering a new room.\n");
            printf("\nEnter an room number: \n");
            scanf("%d", &idRegister);
            r = linearSearchRoom(idRegister, rooms);
            if (r == NULL)
            {
                r = addRoom(idRegister, "double", 3, 110.00, "Occupied");
                fseek(rooms, 0, SEEK_END); // para não sobrescrever dados que existam no arquivo
                saveRoom(r, rooms);
                printDataBaseRoom(rooms);
            }
            else
            {
                printf("\nRoom is occupied.");
            }
            free(r);
            break;

        case 3:
            printf("\nSearching room (Linear).\n");
            printf("\nEnter number room to search: ");
            scanf("%d", &idSearch);
            r = linearSearchRoom(idSearch, rooms);
            if (r != NULL)
            {
                printRoom(r);
                free(r);
            }
            break;

        case 4:
            printf("\nSearching room (Binary).\n");
            printf("\nEnter number room to search: ");
            scanf("%d", &idSearch);
            length = qntOfRegisterRoom(rooms);
            r = binarySearchRoom(idSearch, rooms, 0, length - 1);
            if (r != NULL)
            {
                printRoom(r);
                free(r);
            }
            break;

        case 5:
            printf("\nModifying room informations.\n");
            changeRoom(rooms);
            printDataBaseRoom(rooms);
            break;

        case 6:
            printf("\nSorting rooms.");
            length = qntOfRegisterRoom(rooms);
            bubbleSortRoom(rooms, length);
            printf("\nDatabase sort:\n");
            printDataBaseRoom(rooms);
            break;

        case 7:
            printf("\nCreating database disordered...\n");
            employees = fopen("employee.dat", "w+b"); // para evitar criar base de dados em uma base ja existente
            if (employees == NULL)
            {
                printf("Error reopening employee.dat\n");
                exit(1);
            }
            printf("\nEnter a quantity of register you want to create: ");
            scanf("%d", &qntOfRegister);
            printf("\nEnter a quantity of swaps you want: ");
            scanf("%d", &qntSwap);
            createDisorderedEmployeeDatabase(employees, qntOfRegister, qntSwap);
            printDataBaseEmployee(employees);
            printf("\nQuantity of register: %d", qntOfRegisterEmployee(employees));
            break;

        case 8:
            printf("\nRegistering a new employee.\n");
            printf("\nEnter an ID: \n");
            scanf("%d", &idRegister);
            e = linearSearchEmployee(idRegister, employees);
            if (e == NULL)
            {
                e = addEmployee(idRegister, "anonymous", "Receptionist");
                fseek(employees, 0, SEEK_END); // para não sobrescrever dados que existam no arquivo
                saveEmployee(e, employees);
                printDataBaseEmployee(employees);
            }
            else
            {
                printf("\nEmployee ID already exists.");
            }
            free(e);

            break;

        case 9:
            printf("\nSearching employee (Linear).\n");
            printf("\nEnter an ID to search: ");
            scanf("%d", &idSearch);
            e = linearSearchEmployee(idSearch, employees);
            if (e != NULL)
            {
                printEmployee(e);
                free(e);
            }
            break;

        case 10:
            printf("\nSearching employee (Binary).\n");
            printf("\nEnter an ID to search: ");
            scanf("%d", &idSearch);
            length = qntOfRegisterEmployee(employees);
            e = binarySearchEmployee(employees, idSearch, 0, length - 1);
            if (e != NULL)
            {
                printEmployee(e);
                free(e);
            }
            break;

        case 11:
            printf("\nSorting employees.");
            length = qntOfRegisterEmployee(employees);
            bubbleSortEmployee(employees, length);
            printf("\nDatabase sort:\n");
            printDataBaseEmployee(employees);
            break;

        case 12:
            printf("\nCreating disordered database client.");
            clients = fopen("clients.dat", "w+b"); // para evitar criar base de dados em uma base ja existente
            if (clients == NULL)
            {
                printf("\nError reopening clients.dat\n");
                exit(1);
            }
            printf("\nEnter a quantity of register you want to create: ");
            scanf("%d", &qntOfRegister);
            printf("\nEnter a quantity of swaps you want: ");
            scanf("%d", &qntSwap);
            madeDisorderedBaseClient(clients, qntOfRegister, qntSwap);
            printDataBaseClient(clients);
            printf("\nQuantity of register: %d", qntOfRegisterClient(clients));
            break;

        case 13:
            printf("\nRegistering a new client.");
            printf("\nEnter an CPF: \n");
            scanf("%14s", reg);
            c = linearSearchClient(reg, clients);
            if (c == NULL)
            {
                c = addClient(reg, "anonymous", "(99)99999-9999", "1234567ABC");
                saveClient(c, clients);
                printDataBaseClient(clients);
            }
            else
            {
                printf("\nClient CPF already exists.");
            }
            free(c);

            break;

        case 14:
            printf("\nSearching client (Linear Search).\n");
            char cpfSearch[15];
            printf("\nEnter CPF to search: ");
            scanf("%14s", cpfSearch);
            c = linearSearchClient(cpfSearch, clients);
            if (c != NULL)
            {
                printClient(c);
                free(c);
            }
            else
            {
                printf("\nClient not found.\n");
            }
            break;

        case 15:
            printf("\nRegistering new reservation (order)...\n");

            int idEmployee, numRoom, qntOfGuests;
            float price;
            char cpf[15], periodOfStay[12];

            printf("Enter Employee ID: ");
            scanf("%d", &idEmployee);

            printf("Enter Client CPF (format 000.000.000-00): ");
            scanf("%14s", cpf);

            printf("Enter number of guests: ");
            scanf("%d", &qntOfGuests);

            printf("Enter Room number: ");
            scanf("%d", &numRoom);

            printf("Enter period of stay (e.g., 3days): ");
            scanf("%11s", periodOfStay);

            printf("Enter price: ");
            scanf("%f", &price);

            // Buscar os dados nos arquivos
            e = linearSearchEmployee(idEmployee, employees);
            c = linearSearchClient(cpf, clients);
            r = linearSearchRoom(numRoom, rooms);

            if (e != NULL && c != NULL && r != NULL && strcmp(r->status, "Free") == 0)
            {
                // Gerar novo ID da ordem
                int idOrder = getLastIDOrder(orders) + 1;

                // Criar nova ordem
                o = addOrder(idOrder, qntOfGuests, numRoom, periodOfStay, price, idEmployee, cpf);

                // Salvar ordem no arquivo
                saveOrder(o, orders);

                // Atualizar status do quarto
                strcpy(r->status, "Occupied");

                // Atualizar quarto no arquivo
                int totalRooms = qntOfRegisterRoom(rooms);
                int sizeRoom = lengthOfRegisterRoom();

                for (int i = 0; i < totalRooms; i++)
                {
                    fseek(rooms, i * sizeRoom, SEEK_SET);
                    Room *tmp = readRoom(rooms);

                    if (tmp && tmp->roomNumber == numRoom)
                    {
                        fseek(rooms, i * sizeRoom, SEEK_SET);
                        saveRoom(r, rooms);
                        free(tmp);
                        break;
                    }
                    free(tmp);
                }

                printf("\nOrder registered successfully!\n");
                printingOrder(o);
                printClient(c);
                printRoom(r);

                free(o);
            }
            else
            {
                printf("\nFailed to register order. Check room availability, employee ID or client CPF.\n");
            }

            free(e);
            free(c);
            free(r);

            break;

        case 16:

            printf("\nSearching order.\n");
            printf("Enter an ID order: ");
            scanf("%d", &idOrder);
            o = linearSearchOrder(idOrder, orders);
            if (o != NULL)
            {
                printingOrder(o);
                free(o);
            }
            else
            {
                printf("ID order not found! Please check your informatiions.");
            }
            break;

        case 17:
            printf("\nGenerating a list of orders made...\n");
            printingAllOrders(orders);
            break;

        case 18:
            printf("\nGenerating report of orders by employee.\n");
            printf("Enter employee ID: ");
            scanf("%d", &idSearch);

            rewind(orders);
            Order *ord;
            int found = 0;

            e = linearSearchEmployee(idSearch, employees);
            if (e == NULL)
            {
                printf("\nEmployee not found.\n");
                break;
            }
            while ((ord = readOrder(orders)) != NULL)
            {
                if (ord->idEmployee == idSearch)
                {
                    printingOrder(ord);

                    // Busca e imprime os dados do funcionário responsável
                    Employee *emp = linearSearchEmployee(ord->idEmployee, employees);
                    if (emp != NULL)
                    {
                        printf("\n--- Employee Responsible ---\n");
                        printEmployee(emp);
                        free(emp);
                    }

                    found = 1;
                }

                free(ord);
            }

            if (!found)
            {
                printf("\nNo orders found for this employee.\n");
            }
            break;

        default:
            printf("\nInvalid option!\n");
            break;
        }
    } while (option != 0);

    fclose(employees);
    fclose(orders);
    fclose(clients);
    fclose(rooms);

    return 0;
}