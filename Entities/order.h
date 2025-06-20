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

}Order;


#endif