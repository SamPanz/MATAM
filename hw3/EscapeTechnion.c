//
// Created by Samuel Panzieri on 5/30/2017.
//

#include "EscapeTechnion.h"
#include "list.h"
#include "set.h"

struct escape_technion{
    Set companies;
    Set escapers;
    List orders;
    Set faculties;
    char time[TIME_STR_LEN];
};
