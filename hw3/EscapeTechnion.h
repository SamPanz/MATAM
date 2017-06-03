//
// Created by Samuel Panzieri on 5/30/2017.
//

#ifndef HW3_2_ESCAPETECHNION_H
#define HW3_2_ESCAPETECHNION_H
#include "mtm_ex3.h"

#define TIME_STR_LEN 5

typedef struct escape_technion *System;
typedef struct faculty *Faculty;
typedef struct escaper *Escaper;
typedef struct order *Order;

/**
 * Escape Technion Functions
 */
System createSystem(int argc, char** argv);

void destroySystem(System system);

MtmErrorCode companyAdd(System system, char* email, TechnionFaculty faculty);

MtmErrorCode companyRemove(System system, char* email);

MtmErrorCode roomAdd(System system, char* email, int id, int price, int num_ppl,
                     char* working_hrs, int difficulty);

MtmErrorCode roomRemove(System system, TechnionFaculty faculty, int id);

MtmErrorCode escaperAdd(System system, TechnionFaculty facult, int skill_level);

MtmErrorCode escaperRemoce(System system, char* email);

MtmErrorCode escaperOrder(System system, char* email, TechnionFaculty faculty,
                          int id, char time[TIME_STR_LEN], int num_ppl);

MtmErrorCode escaperRecommend(System system, char* email, int num_ppl);

void reportDay(System system);

void reportBest(System system);

/**
 * Faculty Data Type functions
 */

/**
 * Escaper Data Type functions
 */

/**
 * Order Data Type functions
 */

#endif //HW3_2_ESCAPETECHNION_H
