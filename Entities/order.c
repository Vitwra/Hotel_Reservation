#include "order.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cria uma compra
Order *addOrder(int idOrder, int qntOfGuests, int numRoom, const char *periodOfStay, float price, int idEmployee, const char *cpfClient)
{
    Order *order = (Order *)malloc(sizeof(Order));

    if (order){
    memset(order, 0, sizeof(Order));
    }
    order->idOrder = idOrder;
    order->numRoom = numRoom;
    order->price = price;
    order->idEmployee = idEmployee;
    order->qntOfGuests = qntOfGuests;

    strncpy(order->cpfClient, cpfClient, sizeof(order->cpfClient) - 1);
    strncpy(order->periodOfStay, periodOfStay, sizeof(order->periodOfStay) - 1);

    return order;
}

// Salva a compra
void saveOrder(Order *order, FILE *arq)
{
    fwrite(&order->idOrder, sizeof(int), 1, arq);
    fwrite(&order->numRoom, sizeof(int), 1, arq);
    fwrite(&order->price, sizeof(float), 1, arq);
    fwrite(&order->idEmployee, sizeof(int), 1, arq);
    fwrite(&order->qntOfGuests, sizeof(int), 1, arq);
    fwrite(order->cpfClient, sizeof(char), sizeof(order->cpfClient), arq);
    fwrite(order->periodOfStay, sizeof(char), sizeof(order->periodOfStay), arq);
}

// Imprime uma compra
void printingOrder(Order *order)
{
    printf("**********************************************\n");
    printf("Order ID: %d\n", order->idOrder);
    printf("Employee ID: %d\n", order->idEmployee);
    printf("Client CPF: %s\n", order->cpfClient);
    printf("Quantity of guests: %d\n", order->qntOfGuests);
    printf("Period of Stay: %s\n", order->periodOfStay);
    printf("Room number: %d\n", order->numRoom);
    printf("Total value: %.2f\n", order->price);
    printf("**********************************************\n");
}

// Imprime todas as compras armazenadas no arquivo
void printingAllOrders(FILE *arq)
{
    printf("Printing database...\n");

    rewind(arq);
    Order *order;

    while ((order = readOrder(arq)) != NULL)
    {
        printingOrder(order);
        free(order);
    }
}

// leitura de uma compra
Order *readOrder(FILE *in)
{
    Order *order = (Order *)malloc(sizeof(Order));

    if (fread(&order->idOrder, sizeof(int), 1, in) != 1)
    {
        free(order);
        return NULL;
    }
    fread(&order->numRoom, sizeof(int), 1, in);
    fread(&order->price, sizeof(float), 1, in);
    fread(&order->idEmployee, sizeof(int), 1, in);
    fread(&order->qntOfGuests, sizeof(int), 1, in);
    fread(order->cpfClient, sizeof(char), sizeof(order->cpfClient), in);
    fread(order->periodOfStay, sizeof(char), sizeof(order->periodOfStay), in);

return order;
}

// Busca sequencial por um ID de uma compra
Order *linearSearchOrder(int key, FILE *arq)
{
    Order *o;

    int find = 0;

    rewind(arq);

    while ((o = readOrder(arq)) != NULL)
    {
        if (o->idOrder == key)
        {
            return o;
        }
        free(o);
    }
        printf("Order not found\n");
        free(o);
        return NULL;
}