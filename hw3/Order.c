//
// Created by Samuel Panzieri on 5/30/2017.
//

#include "Order.h"
#include "Escaper.h"
#include "Utilities_Functions.c"


struct order{
    Escaper escaper;
    char* time[TIME_STR_LEN];
    int num_ppl;
    char* email;
};

void freeallorder(Order ordertofree)
{
    free(ordertofree->time);
    destroyEscaper(ordertofree->escaper);
    free(ordertofree->email);
}

Order createOrder(char* time[TIME_STR_LEN], int num_ppl, char* email,
                  Escaper escaper){
    Order new_order= malloc(sizeof(Order));
    if (!new_order)
        return NULL;
    new_order->escaper=escaperCopy(escaper);
    if(new_order->escaper==NULL) {
        freeallorder(new_order));
        return NULL;
    }
    new_order->email=stringDuplicate(email);
    if(new_order->email==NULL){
        freeallorder(new_order);
        return NULL:
    }
    new_order->num_ppl=num_ppl;
    new_order->time=stringDuplicate(time;
    if(new_order->time==NULL){
        freeallorder(new_order);
        return NULL;
    }
    return new_order;

}
void destroyOrder(Order order){
    if(!order)
        return;
    free(order->time);
    free(order->email);
    destroyEscaper(order->escaper);
}

Order orderCopy(Order order){
    if(!order)
        return NULL;
    Order new_order = createOrder(order->time,order->num_ppl,order->email,order->escaper);
    if(!new_order) /* even if one of the malloc fails we free everything and return null*/
        return NULL;
}
int orderCompare(Order order_1, Order order_2){
    if(!order_1||!order_2)
        return NULL;
    if(strcmp(order_1->time,order_2->time)==0)
        return strcmp(order_1->email,order_2->email);
    else
        return strcmp(order_1->time,order_2->time);

}
