//
// Created by Samuel Panzieri on 5/30/2017.
//

#include "Company.h"
#include "set.h"
#include "stdlib.h"
#include "Utilities_Functions.c"
#include <Room.h>
#include "mtm_ex3.h"
#include "Faculty.h"

struct company{
    Set rooms;
    TechnionFaculty faculty_id;
    char* email;
};


Company createCompany(char* email, TechnionFaculty faculty){
    Company company= malloc(sizeof(company));
    if(!company)
        return NULL;
    company->email=stringDuplicate(email);
    if(!company->email) {
        companyDestroy(company);
        return NULL;
    }
    company->rooms=setCreate(roomCopy,roomDestroy,roomCompare);
    assert(company->rooms);
    company->faculty_id=faculty;
    return company;
    }


void companyDestroy(Company company){
    assert(company);
    free(company->email);
    setDestroy(company->rooms);
    free(company);
}

MtmErrorCode addRoomToCompany(Company company, Room room){
    SetResult result=setAdd(company->rooms,room);
    if(result==SET_ITEM_ALREADY_EXISTS)
        return
}

MtmErrorCode removeRoomFromCompany(Company company, TechnionFaculty faculty,
                                   int id);

Room recommendRoom(Company, char* email, int num_ppl);