//
// Created by Samuel Panzieri on 5/30/2017.
//

#ifndef HW3_2_ESCAPER_H
#define HW3_2_ESCAPER_H
#include <stdlib.h>
#include <stdbool.h>
#include "mtm_ex3.h"

typedef struct escaper *Escaper;

Escaper createEscaper(char* email, TechnionFaculty faculty_id, int skill_level);
void destroyEscaper(Escaper escaper);

Escaper escaperCopy(Escaper escaper);
int escaperCompare(Escaper escaper_1, Escaper escaper_2);

#endif //HW3_2_ESCAPER_H
