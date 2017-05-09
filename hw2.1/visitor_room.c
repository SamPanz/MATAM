#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

#include "visitor_room.h"

/* IMPLEMENT HERE ALL WHAT IS NEEDED */
#define WORD_LEN 51
static char* string_duplicate(const char* str) {
	char* copy = malloc(strlen(str) + 1);
	return copy ? strcpy(copy, str) : NULL;
}

Result init_challenge_activity(ChallengeActivity *activity,
		Challenge *challenge) {
	if (activity == NULL || challenge == NULL) {
		return NULL_PARAMETER;
	}

	activity->challenge = challenge;
	activity->start_time = 0;
	activity->visitor = NULL;

	return OK;
}

Result reset_challenge_activity(ChallengeActivity *activity) {
	if (activity == NULL) {
		return NULL_PARAMETER;
	}
	activity->challenge = NULL;
	activity->start_time = 0;
	activity->visitor = NULL;
	return OK;
}

Result init_visitor(Visitor *visitor, char *name, int id) {
	if (name == NULL || visitor == NULL) {
		return NULL_PARAMETER;
	}

	char *copy_str = string_duplicate(name);
	if (!copy_str) {
		return MEMORY_PROBLEM;
	}

	visitor->current_challenge = NULL;
	visitor->room_name = NULL;
	visitor->visitor_id = id;
	visitor->visitor_name = copy_str;

	return OK;
}

Result reset_visitor(Visitor *visitor) {
	if (visitor == NULL) {
		return NULL_PARAMETER;
	}

	visitor->current_challenge = NULL;
	free(visitor->room_name);
	visitor->room_name = NULL;
	visitor->visitor_id = 0;
	visitor->visitor_name = NULL;

	return OK;
}

Result init_room(ChallengeRoom *room, char *name, int num_challenges) {

	if (room == NULL || name == NULL) {
		return NULL_PARAMETER;
	}
	if (num_challenges < 1) {
		return ILLEGAL_PARAMETER;
	}
	char *copy_name = string_duplicate(name);
	if (!copy_name) {
		return MEMORY_PROBLEM;
	}

	room->challenges = malloc(sizeof(ChallengeActivity) * num_challenges);
	if (!room->challenges) {
		return NULL_PARAMETER;
	}
	room->name = copy_name;
	room->num_of_challenges = num_challenges;

	return OK;
}

Result reset_room(ChallengeRoom *room) {
	if (room == NULL) {
		return NULL_PARAMETER;
	}

	free(room->challenges);
	room->challenges = NULL;
	free(room->name);
	room->name = NULL;
	room->num_of_challenges = 0;

	return OK;
}

Result num_of_free_places_for_level(ChallengeRoom *room, Level level,
		int *places) {
	if (room == NULL) {
		return NULL_PARAMETER;
	}

	if (room->num_of_challenges == 0) {
		*places = 0;
		return OK;
	}

	int summer_of_challenges = 0;
	for (int i = 0; i < room->num_of_challenges; i++) {
		if (room->challenges[i].visitor == NULL)
		{
			if (room->challenges[i].challenge->level == level) {
				summer_of_challenges += 1;
			} else if (level == All_Levels) {
				summer_of_challenges += 1;
			}
		}
	}

	*places = summer_of_challenges;
	return OK;
}

Result change_room_name(ChallengeRoom *room, char *new_name) {
	if (room == NULL || new_name == NULL) {
		return NULL_PARAMETER;
	}

	char *copy_str = string_duplicate(new_name);
	if (!copy_str) {
		return MEMORY_PROBLEM;
	}

	free(room->name);
	room->name = copy_str;

	return OK;
}

Result room_of_visitor(Visitor *visitor, char **room_name) {
	if (visitor == NULL || room_name == NULL) {
		return NULL_PARAMETER;
	}

	if (visitor->room_name == NULL){
		return NOT_IN_ROOM;
	}
	char *copy_str = string_duplicate(*(visitor->room_name));
	if (!copy_str) {
		return MEMORY_PROBLEM;
	}

	*room_name = copy_str;
	return OK;
}

Result visitor_enter_room(ChallengeRoom *room, Visitor *visitor, Level level,
		int start_time) {
	/* the challenge to be chosen is the lexicographically named smaller one that has
	 the required level. assume all names are different. */
	if (room == NULL || visitor == NULL) {
		return NULL_PARAMETER;
	}

	if (visitor->room_name != NULL) {
		return ALREADY_IN_ROOM;
	}

	int *places = malloc(sizeof(int));
	if (!places) {
		return MEMORY_PROBLEM;
	}

	num_of_free_places_for_level(room, level, places);
	if (*places == 0) {
		free(places);
		return NO_AVAILABLE_CHALLENGES;
	}
	free(places);
	int flag_iterator = -1;
	//There are available challenges, because I execute num_of_free_places_for_level before
	for (int i = 0; i < room->num_of_challenges; i++) {
		if (room->challenges[i].visitor == NULL) {
			if (level == All_Levels
					|| room->challenges[i].challenge->level == level) {
				flag_iterator = i;
				break;
			}
		}
	}

	if (flag_iterator > -1) {
		char* m_challenge_name = room->challenges[flag_iterator].challenge->name;

		for (int i = flag_iterator + 1; i < room->num_of_challenges; i++) {
			if (room->challenges[i].visitor == NULL) {
				if (level == All_Levels
						|| room->challenges[i].challenge->level == level) {
					if (strcmp(room->challenges[i].challenge->name,
							m_challenge_name) < 0) {
						m_challenge_name = room->challenges[i].challenge->name;
						flag_iterator = i;
					}
				}
			}
		}

		room->challenges[flag_iterator].visitor = visitor;
		room->challenges[flag_iterator].start_time = start_time;
		visitor->current_challenge = &(room->challenges[flag_iterator]);

		visitor->room_name = malloc(sizeof(char)*WORD_LEN);
		if (!(visitor->room_name)){
			return MEMORY_PROBLEM;
		}

		*(visitor->room_name) = string_duplicate(room->name);
		if (*(visitor->room_name) == NULL){
			return MEMORY_PROBLEM;
		}
		inc_num_visits(visitor->current_challenge->challenge);
		return OK;
	}

	return NO_AVAILABLE_CHALLENGES;

}

Result visitor_quit_room(Visitor *visitor, int quit_time) {
	if (visitor == NULL) {
		return NULL_PARAMETER;
	}

	if (visitor->room_name == NULL) {
		return NOT_IN_ROOM;
	}

	int time_in_challenge = quit_time - visitor->current_challenge->start_time;
	set_best_time_of_challenge(visitor->current_challenge->challenge,
			time_in_challenge);

	Challenge *ch = visitor->current_challenge->challenge;
	reset_challenge_activity(visitor->current_challenge);
	init_challenge_activity(visitor->current_challenge, ch);
	visitor->current_challenge = NULL;
	free(visitor->room_name);
	visitor->room_name = NULL;
	return OK;

}
