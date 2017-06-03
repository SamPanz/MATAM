//
// Created by Samuel Panzieri on 5/30/2017.
//
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

static bool ifValidEmail(char* email){
    assert(email != NULL);
    int counter = 0;
    for (int i = 0; i < strlen(email) ; i++){
        if (email[i] == '@'){
            counter++;
        }
    }
    return (counter == 1);
}

static char* stringDuplicate(const char* str) {
    char* copy = malloc(strlen(str) + 1);
    return copy ? strcpy(copy, str) : NULL;
}

static bool checkValidSkillLevel(int skill_level){
    return(skill_level > 0 && skill_level <= 10);
}

