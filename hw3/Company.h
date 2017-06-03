//
// Created by Samuel Panzieri on 5/30/2017.
//

#ifndef HW3_2_COMPANY_H
#define HW3_2_COMPANY_H
#include "mtm_ex3.h"
#include "Room.h"

typedef struct company *Company;

/**
 * Company Data Type
 */
Company createCompany(char* email, TechnionFaculty faculty);

void companyDestroy(Company company);

MtmErrorCode addRoomToCompany(Company company, Room room);

MtmErrorCode removeRoomFromCompany(Company company, TechnionFaculty faculty,
                                   int id);

Room recommendRoom(Company, char* email, int num_ppl);

#endif //HW3_2_COMPANY_H
