//
// Created by Samuel Panzieri on 5/30/2017.
//

#include "Faculty.h"
#include <stdlib.h>
struct faculty{
    TechnionFaculty faculty_id;
    int profit;
};

Faculty createFaculty(TechnionFaculty faculty_id, int i) {
    Faculty new_faculty = malloc(sizeof(Faculty));
    if (!new_faculty)
        return NULL;
    new_faculty->faculty_id=faculty_id;
    new_faculty->profit=0;
    return new_faculty;
}
void destroyFaculty(Faculty faculty){
    if(!faculty)
        return /*Nothing to do here its pointless to free(null)*/
    free(faculty);
}

Faculty facultyCopy(Faculty faculty){
    Faculty copy = createFaculty(faculty->faculty_id, faculty->profit);
    if(!copy)
        return NULL;
    return copy;
}
int facultyCompare(Faculty faculty_1, Faculty faculty_2){//TODO:check how to compare between faculties;

}
