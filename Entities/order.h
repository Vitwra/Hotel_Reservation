#ifndef ORDER_H
#define ORDER_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Order
{
    int idOrder;
    int numRoom;
    float price;
    int idEmployee;
    int qntOfGuests;
    char cpfClient[15];
    char periodOfStay[12];
}Order;

Order *addOrder(int idOrder, int qntOfGuests, int numRoom, const char *periodOfStay, float price, int idEmployee, const char *cpfClient);

void saveOrder(Order *order, FILE *arq);

void printingOrder(Order *order);

void printingAllOrders(FILE *arq);

Order *readOrder(FILE *in);

Order *linearSearchOrder(int key, FILE *arq);
#endif