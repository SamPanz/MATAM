//
// Created by Samuel Panzieri on 5/30/2017.
//

#ifndef HW3_2_FACULTY_H
#define HW3_2_FACULTY_H
#include "mtm_ex3.h"

typedef struct faculty *Faculty;

Faculty createFaculty(TechnionFaculty faculty_id, int i);
void destroyFaculty(Faculty faculty);

Faculty facultyCopy(Faculty faculty);
int facultyCompare(Faculty faculty_1, Faculty faculty_2);

#endif //HW3_2_FACULTY_H
