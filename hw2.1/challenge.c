#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

#include "challenge.h"

/* IMPLEMENT HERE ALL WHAT IS NEEDED */
static char* string_duplicate(const char* str) {
	char* copy = malloc(strlen(str) + 1);
	return copy ? strcpy(copy, str) : NULL;
}

/** Initialize the challenge struct.
 */
Result init_challenge(Challenge *challenge, int id, char *name, Level level) {

	if (challenge == NULL || name == NULL) {
		return NULL_PARAMETER;
	}

	char *copy_str = string_duplicate(name);
	if (!copy_str) {
		return MEMORY_PROBLEM;
	}

	challenge->level = level;
	challenge->id = id;
	challenge->name = copy_str;
	challenge->best_time = 0;
	challenge->num_visits = 0;

	return OK;
}

/**
 * reset all parameters of challenge to 0
 */
Result reset_challenge(Challenge *challenge) {
	if (challenge == NULL) {
		return NULL_PARAMETER;
	}
	free(challenge->name);
	challenge->name = NULL;
	challenge->best_time = 0;
	challenge->level = All_Levels; // TODO is it ok?
	challenge->num_visits = 0;
	challenge->id = 0;

	return OK;
}

/**
 * Change the name of challenge.
 */
Result change_name(Challenge *challenge, char *name) {
	if (challenge == NULL || name == NULL) {
		return NULL_PARAMETER;
	}

	char *copy_str = string_duplicate(name);
	if (!copy_str) {
		return MEMORY_PROBLEM;
	}
	free(challenge->name);
	challenge->name = copy_str;

	return OK;
}

/**
 * Setting the best time of challenge.
 */
Result set_best_time_of_challenge(Challenge *challenge, int time) {
	if (challenge == NULL) {
		return NULL_PARAMETER;
	}

	if (time < 0 || (time > challenge->best_time && challenge->best_time > 0)) {
		return ILLEGAL_PARAMETER;
	}

	challenge->best_time = time;
	return OK;
}

/**
 * Pass the best time of challenge into the input pointer parameter
 */
Result best_time_of_challenge(Challenge *challenge, int *time) {
	if (challenge == NULL || time == NULL) {
		return NULL_PARAMETER;
	}

	*time = challenge->best_time;
	//TODO - check if its only value and not pointer

	return OK;
}

/**
 * Increment number of visits of Challenge in 1
 */
Result inc_num_visits(Challenge *challenge) {
	if (challenge == NULL) {
		return NULL_PARAMETER;
	}

	challenge->num_visits += 1;
	return OK;
}

/**
 * Passing the number of visits to the Input pointer parameter
 */
Result num_visits(Challenge *challenge, int *visits) {
	if (challenge == NULL || visits == NULL) {
		return NULL_PARAMETER;
	}

	*visits = challenge->num_visits;
	//TODO - check if its only value and not pointer

	return OK;
}
