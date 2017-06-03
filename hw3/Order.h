//
// Created by Samuel Panzieri on 5/30/2017.
//

#ifndef HW3_2_ORDER_H
#define HW3_2_ORDER_H

#include "Escaper.h"
#define TIME_STR_LEN 5

typedef struct order *Order;

Order createOrder(char* time[TIME_STR_LEN], int num_ppl, char* email,
                  Escaper escaper);
void destroyOrder(Order order);

Order orderCopy(Order order);
int orderCompare(Order order_1, Order order_2);
#endif //HW3_2_ORDER_H
