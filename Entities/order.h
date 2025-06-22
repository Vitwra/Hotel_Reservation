#ifndef ORDER_H
#define ORDER_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Order
{
    int qntOfGuests;
    int numRoom;
    char periodOfStay[12];
    float price;
    int idEmployee;
    char cpfClient[15];
    int idOrder;
}Order;


#endif