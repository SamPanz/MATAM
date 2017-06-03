//
// Created by Samuel Panzieri on 5/30/2017.
//

#include "Escaper.h"
#include "utilities_functions.c"
struct escaper{
    char* email;
    TechnionFaculty faculty_id;
    int skill_level;
};

Escaper createEscaper(char* email, TechnionFaculty faculty_id, int skill_level){
    if (!email || !faculty_id || !skill_level){
        return NULL;
    }
    assert(ifValidEmail(email));
    Escaper escaper = malloc(sizeof(*escaper));
    if (escaper == NULL){
        return NULL;
    }
    escaper->email = stringDuplicate(email);
    if (!escaper->email){
        destroyEscaper(escaper);
        return NULL;
    }
    escaper->faculty_id = faculty_id;
    if (!checkValidSkillLevel(skill_level)){
        destroyEscaper(escaper);
        return NULL;
    }
    escaper->skill_level = skill_level;
    return escaper;
}
void destroyEscaper(Escaper escaper){
    if (!escaper)
        return NULL;
    free(escaper->email);
    free(escaper);
}

Escaper escaperCopy(Escaper escaper){
    Escaper copy = createEscaper(escaper->email,escaper->faculty_id,escaper->skill_level);
    if (!copy) /* The creation of a new escaper didnt succeed*/
        return NULL;
    return copy;
}
/*
 * For EscaperCompare comparing betweeen the emails of the 2 escapers is enough since we know that
 * there arent 2 different escapers with the same email;
 * so the function returns 0 if the emails are equal(same escaper)
 * otherwise it returns the lexicographical difference between the emails.
 */
int escaperCompare(Escaper escaper_1, Escaper escaper_2){
    if (!escaper_1||!escaper_2)/*null parameter handling*/
        return NULL;
    return (strcmp(escaper_1->email,escaper_2->email);
}