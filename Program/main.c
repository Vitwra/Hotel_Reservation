#include <stdio.h>
#include <stdlib.h>
#include "../Entities/client.h"
#include "../Entities/employee.h"
#include "../Entities/order.h"

int main(){

    Employee *e;
    Client *c;
    Order *o;
    FILE *orders;
    FILE *clients;
    FILE *employees;
    int qntOfRegister = 10;
    int qntSwap = 0;
    int length;
    int idRegister;
    int option;
    char reg[15];

    if ((orders = fopen("order.dat", "w+b")) == NULL)
    {
        printf("Error to open order archives\n");
        exit(1);
    }
    if ((clients = fopen("client.dat", "w+b")) == NULL)
    {
        printf("Error to open order archives\n");
        exit(1);
    }
    if ((employees = fopen("employee.dat", "w+b")) == NULL)
    {
        printf("Error to open order archives\n");
        exit(1);
    }
 
    do
    {
        printf("\n\n>>>>>>>>>>>>>>>>>>>>>>> SYSTEM OF HOTEL RESERVATION <<<<<<<<<<<<<<<<<<<");
        printf("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> MENU OPTIONS <<<<<<<<<<<<<<<<<<<<<<<<<\n");

        printf("\n>>>>>>>>>>>>>>>>>>>>>>> Employee <<<<<<<<<<<<<<<<<<<\n");
        printf("\n1 - Create database employee disordered");
        printf("\n2 - Register an employee");
        printf("\n3 - Search employee (Linear)");
        printf("\n4 - Search employee (Binary)");
        printf("\n5 - Sort employees");

        printf("\n\n>>>>>>>>>>>>>>>>>>>>>>> Client <<<<<<<<<<<<<<<<<<<\n");
        printf("\n6 - Create disordered database client");
        printf("\n7 - Register an client");
        printf("\n8 - Search client (Linear)");
        printf("\n9 - Search client (Binary)");
        printf("\n10 - Sort clients");

        printf("\n\n>>>>>>>>>>>>>>>>>>>>>>> Order <<<<<<<<<<<<<<<<<<<\n");
        printf("\n11 - Register an order");
        printf("\n12 - Search an order");
        printf("\n13 - Gerar relatorio de compras");

        printf("\n\n0 - Exit");
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
            printf("\nCreating database disordered...\n");
            createDisorderedEmployeeDatabase(employees, qntOfRegister, qntSwap);
            printDataBaseEmployee(employees);
            break;

        case 2:
            printf("\nRegistering a new employee.\n");
            printf("\nEnter an ID: \n");
            scanf("%d", &idRegister);
            e = linearSearchEmployee(idRegister, employees);
            if (e == NULL)
            {
                e = addEmployee(idRegister, "anonimous", "Receptionist");
                saveEmployee(e, employees);
                printDataBaseEmployee(employees);
            }
            else
            {
                printf("Employee ID already exists.");
            }
            free(e);

            break;

        case 3:
            printf("\nSearching employee (Linear).\n");
            e = linearSearchEmployee(1001, employees);
            if (e != NULL)
            {
                printEmployee(e);
                free(e);
            }
            break;

        case 4:
            printf("\nSearching employee (Binary).\n");
            length = qntOfRegisterEmployee(employees);
            e = binarySearchEmployee(employees, 1000, 0, length - 1);
            if (e != NULL)
            {
                printEmployee(e);
                free(e);
            }
            break;

        case 5:
            printf("\nSorting employees.\n");
            length = qntOfRegisterEmployee(employees);
            bubbleSortEmployee(employees, length);
            printf("Database sort:\n");
            printDataBaseEmployee(employees);
            break;

        case 6:
            printf("\nCreating disordered database client.\n");
            madeDisorderedBaseClient(clients, qntOfRegister, qntSwap);
            printDataBaseClient(clients);
            printf("Quantity of register: %d", qntOfRegisterClient(clients));
            break;

        case 7:
            printf("\nRegistering a new client.\n");
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
                printf("Client CPF already exists.");
            }
            free(c);

            break;

        case 8:
            printf("\nSearching client (Linear Search).\n");
            char cpfSearch[15];
            printf("Enter CPF to search: ");
            scanf("%14s", cpfSearch);
            c = linearSearchClient(cpfSearch, clients);
            if (c != NULL)
            {
                printClient(c);
                free(c);
            }
            else
            {
                printf("Client not found.\n");
            }
            break;

        case 9:
            printf("\nSearching client (Binary Search).\n");
            printf("Enter CPF to search: ");
            scanf("%14s", cpfSearch);
            c = binarySearchClient(clients, cpfSearch, 0, qntOfRegisterClient(clients) - 1);
            if (c != NULL)
            {
                printClient(c);
                free(c);
            }
            else
            {
                printf("Client not found.\n");
            }
            break;

        case 10:
            printf("\nSorting clients.\n");
            length = qntOfRegisterClient(clients);
            bubbleSortClient(clients, length);
            printf("Database sort:\n");
            printDataBaseClient(clients);
            break;

        case 11:
            printf("\nRegister a new order.\n");

            Order *order;
            int idEmployee;
            static int idOrder = 0;
            char cpf[15];
            int qntOfGuests;
            int numRoom;
            char periodOfStay[12];
            float price;

            printf("Employee ID: ");
            scanf("%d", &idEmployee);
            printf("Client CPF: ");
            scanf("%14s", cpf);
            printf("Quantity of Guests: ");
            scanf("%d", &qntOfGuests);
            printf("Room number: ");
            scanf("%d", &numRoom);
            printf("Period of stay: ");
            scanf("%12s", periodOfStay);
            printf("Price: ");
            scanf("%f", &price);

            c = linearSearchClient(cpf, clients);
            e = linearSearchEmployee(idEmployee, employees);

            if (c != NULL && e != NULL)
            {
                order = addOrder(++idOrder, qntOfGuests, numRoom, periodOfStay, price, idEmployee, cpf);

                saveOrder(order, orders);

                printf("\nOrder successfully registared!\n");
                printingOrder(order);
                printClient(c);
                free(order);
            }
            else
            {
                printf("\nOrder failed. Verify the informations.\n");
            }

            free(c);
            free(e);
            break;

        case 12:
            printf("\nSearching order.\n");
            o = linearSearchOrder(3, orders);
            if (o != NULL)
                printingOrder(o);
            free(o);

            break;

        case 13:
            printf("\nGenerating a list of orders made...\n");
            printingAllOrders(orders);
            break;

        case 14:
            printf("\nGenerating order receipt...\n");
            int idToSearch;
            printf("Enter order ID to print receipt: ");
            scanf("%d", &idToSearch);

            o = linearSearchOrder(idToSearch, orders);

            if (o != NULL)
            {
                printingOrder(o);
                free(o);
            }
            else
            {
                printf("Order not found.\n");
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

    return 0;
}
