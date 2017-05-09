#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

#include "challenge_system.h"

/* IMPLEMENT HERE ALL WHAT IS NEEDED */
#define WORD_LEN 51

/**
 * STATIC FUNCTIONS
 */
static char* string_duplicate(const char* str) {
	char* copy = malloc(strlen(str) + 1);
	return copy ? strcpy(copy, str) : NULL;
}

static Result destroy_all_challenges(ChallengeRoomSystem *sys) {
	if (!sys) {
		return NULL_PARAMETER;
	}
	for (int i = 0; i < sys->number_of_challenges; i++) {
		reset_challenge(&(sys->all_challenges[i]));
//		free(sys->all_challenges[i]);
	}
	free(sys->all_challenges);
	return OK;
}
static Result destroy_all_rooms(ChallengeRoomSystem *sys) {
	if (!sys) {
		return NULL_PARAMETER;
	}
	for (int i = 0; i < sys->number_of_rooms; i++) {
		reset_room(&(sys->all_rooms[i]));
	}
	free(sys->all_rooms);
	return OK;
}

static struct visitor_node* create_visitor_node(Visitor* visitor) {
	struct visitor_node *ptr = malloc(sizeof(struct visitor_node));
	if (!ptr) {
		return NULL;
	}
	ptr->visitor_n = visitor;
	ptr->next = NULL;
	return ptr;
}

static void add_visitor_to_list(ChallengeRoomSystem *sys, Visitor* visitor) {
	struct visitor_node *new_node = create_visitor_node(visitor);
	if (!sys->visitor_list) {
		sys->visitor_list = new_node;
	} else {
		struct visitor_node *ptr = sys->visitor_list;
		while (ptr->next) {
			ptr = ptr->next;
		}
		ptr->next = new_node;
	}
}

static void remove_visitor_from_list(ChallengeRoomSystem *sys, Visitor* visitor) {

	struct visitor_node *ptr_first = sys->visitor_list;
	if (!ptr_first) {
		return;
	}
	struct visitor_node *ptr_second = ptr_first->next;

	if (ptr_first->visitor_n->visitor_id == visitor->visitor_id) {
		sys->visitor_list = ptr_second;
		reset_visitor(ptr_first->visitor_n);
		free(ptr_first->visitor_n);
		free(ptr_first);
		return;
	} else {
		while (ptr_second != NULL
				&& (ptr_second->visitor_n->visitor_id != visitor->visitor_id)) {
			ptr_first = ptr_first->next;
			ptr_second = ptr_second->next;
		}

		if (ptr_second) {
			ptr_first->next = ptr_second->next;
			reset_visitor(ptr_second->visitor_n);
			free(ptr_second->visitor_n);
		}
		free(ptr_second);
	}
}
static Result destroy_visitor_list_node(struct visitor_node *ptr) {
	while (ptr) {
		struct visitor_node *toDelete = ptr;
		ptr = ptr->next;
		free(toDelete);
	}
	return OK;
}

static struct visitor_node* find_visitor_node(
		struct visitor_node *firstVisitorNode, int visitor_id) {
	if (!firstVisitorNode) {
		return NULL;
	}
	struct visitor_node *ptr = firstVisitorNode;
	while (ptr != NULL && (ptr->visitor_n->visitor_id != visitor_id)) {
		ptr = ptr->next;
	}

	return ptr;
}

static struct visitor_node* find_visitor_node_by_name(
		struct visitor_node *firstVisitorNode, char* visitor_name) {
	if (!firstVisitorNode) {
		return NULL;
	}
	struct visitor_node *ptr = firstVisitorNode;
	while (ptr != NULL
			&& (strcmp(ptr->visitor_n->visitor_name, visitor_name) != 0)) {
		ptr = ptr->next;
	}

	return ptr;
}

static Result init_sys_variables(ChallengeRoomSystem** sys){
	(*sys)->last_time_on_system = 0;
		(*sys)->visitor_list = NULL;
		(*sys)->system_name = malloc(sizeof(char) * WORD_LEN);
		if ((*sys)->system_name == NULL) {
			return MEMORY_PROBLEM;
		}
		return OK;
}
/**
 *
 * MY FUNCTIONS - IMPLEMENT OF HEADER FILE
 */

/**
 * create_system - create the enitre system, initial the CHhallengeRoomSystem
 * 					struct. Read the input file, and initial the system
 * 					Challenge and Rooms arrays.
 */
Result create_system(char *init_file, ChallengeRoomSystem **sys) {
	// checking errors
	if (!init_file || !sys) {
		return NULL_PARAMETER;
	}
	//open file
	FILE* fd = fopen(init_file, "r");
	if (!fd) {
		return NULL_PARAMETER;
	}
	*sys = malloc(sizeof(**sys));
	if (*sys == NULL) {
		return MEMORY_PROBLEM;
	}
	Result r = init_sys_variables(sys);
	if (r != OK){
		return r;
	}
	fscanf(fd, "%s", (*sys)->system_name); //scan system name
	// scan from file the amount of challenges for input
	int challenges_number_input = 0;
	fscanf(fd, "%d", &challenges_number_input);
	(*sys)->number_of_challenges = challenges_number_input;
	// allocate memory for system challenges array
	(*sys)->all_challenges = malloc(
			sizeof(Challenge) * challenges_number_input);
	if (!(*sys)->all_challenges) {
		fclose(fd);
		return MEMORY_PROBLEM;
	}
	// iterate all the challenges from file, create the Challenge and insert it
	// to the system Challenges array
	for (int i = 0; i < (*sys)->number_of_challenges; i++) {
		char *challenge_name = malloc(sizeof(char) * WORD_LEN);
		if (!challenge_name) {
			free((*sys)->all_challenges);
			fclose(fd);
			return MEMORY_PROBLEM;
		}
		int challenge_id, challenge_level;
		fscanf(fd, "%s %d %d", challenge_name, &challenge_id, &challenge_level);
		Level ch_lvl = challenge_level - 1;
		Result res = init_challenge(&((*sys)->all_challenges[i]), challenge_id,
				challenge_name, ch_lvl);
		if (res != OK) {
			free((*sys)->all_challenges);
			free(challenge_name);
			fclose(fd);
			return res;
		}
	}
	// scan the amount of room from file to system variable
	int room_number = 0;
	fscanf(fd, "%d", &room_number);
	(*sys)->number_of_rooms = room_number;
	// allocate memory for every all rooms in the system rooms array
	(*sys)->all_rooms = malloc(sizeof(ChallengeRoom) * room_number);
	if (!(*sys)->all_rooms) {
		free((*sys)->all_challenges);
		fclose(fd);
		return MEMORY_PROBLEM;
	}
	// create all the rooms from input file and add it the right challenges,
	// according to the input FILE
	for (int i = 0; i < room_number; i++) {
		// allocate memory for room name
		char *room_name = malloc(sizeof(char) * WORD_LEN);
		if (!room_name) {
			free((*sys)->all_challenges);
			free((*sys)->all_rooms);
			fclose(fd);
			return MEMORY_PROBLEM;
		}
		// scan amount of challenges in each room
		int num_of_challenges_in_room = 0;
		fscanf(fd, "%s %d", room_name, &num_of_challenges_in_room);
		// make and array of all the challenges id of that paticular room.
		// challenge_number_in_room must be freed
		int *challenge_number_in_room = malloc(
				sizeof(int) * num_of_challenges_in_room);
		if (!challenge_number_in_room) {
			free((*sys)->all_challenges);
			free((*sys)->all_rooms);
			fclose(fd);
			return MEMORY_PROBLEM;
		}
		// iterate all challenges in the room and add it to the array above
		for (int j = 0; j < num_of_challenges_in_room; j++) {
			fscanf(fd, "%d", &challenge_number_in_room[j]);
		}
		//create this room in the system array of rooms
		Result res1 = init_room(&((*sys)->all_rooms[i]), room_name,
				num_of_challenges_in_room);
		if (res1 != OK) {
			free((*sys)->all_challenges);
			free((*sys)->all_rooms);
			free(challenge_number_in_room);
			fclose(fd);
			return res1;
		}
		// loop to link all the challenges id's from the input FILE from
		// the system's challenges array to this room
		for (int x = 0; x < num_of_challenges_in_room; x++) {
			for (int y = 0; y < (*sys)->number_of_challenges; y++) {
				if (challenge_number_in_room[x]
						== (*sys)->all_challenges[y].id) {
					Result res2 = init_challenge_activity(
							&((*sys)->all_rooms[i].challenges[x]),
							&((*sys)->all_challenges[y]));
					if (res2 != OK) {
						free((*sys)->all_challenges);
						free((*sys)->all_rooms);
						free(challenge_number_in_room);
						fclose(fd);
						return res2;
					}
				}
			}
		}
		free(challenge_number_in_room);
	}
	fclose(fd);
	return OK;
}

/**
 * destroy_system - function to release every necessary memory
 * output - best time of all challenges, most popular room
 */
Result destroy_system(ChallengeRoomSystem *sys, int destroy_time,
		char **most_popular_challenge_p, char **challenge_best_time) {
	if (!sys || !most_popular_challenge_p || !destroy_time) {
		return NULL_PARAMETER;
	}

	if (destroy_time < sys->last_time_on_system) {
		return ILLEGAL_TIME;
	}
	Result r = all_visitors_quit(sys, destroy_time);
	if (r != OK) {
		return r;
	}
	r = most_popular_challenge(sys, most_popular_challenge_p);
	if (r != OK) {
		return r;
	}

	int best_time_holder = 0;
	int time_holder = 0;
	for (int i = 0; i < sys->number_of_challenges; i++) {
		best_time_of_system_challenge(sys, sys->all_challenges[i].name,
				&time_holder);
		if (time_holder == 0) {
			continue;
		}
		if (best_time_holder == 0 || time_holder < best_time_holder) {
			best_time_holder = time_holder;
			//TODO leak of memory here, need to free challenge_best_time before
			//free(*challenge_best_time);
			*challenge_best_time = string_duplicate(
					sys->all_challenges[i].name);
			if (!(*challenge_best_time)) {
				return MEMORY_PROBLEM;
			}
		} else if (time_holder == best_time_holder && time_holder != 0) {
			if (strcmp(*challenge_best_time, sys->all_challenges[i].name) < 0) {
				//TODO leak of memory here, need to free challenge_best_time before
				*challenge_best_time = string_duplicate(
						sys->all_challenges[i].name);
				if (!(*challenge_best_time)) {
					return MEMORY_PROBLEM;
				}
			}
		}
	}
	destroy_all_rooms(sys);
	destroy_all_challenges(sys);
	destroy_visitor_list_node(sys->visitor_list);
	free(sys);
	return OK;
}

/**
 * visitor_arrice - create visitor from input parameters, and insert the visitor
 * 					to the room according to the input parameters.
 */
Result visitor_arrive(ChallengeRoomSystem *sys, char *room_name,
		char *visitor_name, int visitor_id, Level level, int start_time) {
	if (!sys) {
		return NULL_PARAMETER;
	}
	if (start_time < sys->last_time_on_system) {
		return ILLEGAL_TIME;
	}
	if (!room_name || !visitor_name) {
		return ILLEGAL_PARAMETER;
	}
	struct visitor_node *v = find_visitor_node(sys->visitor_list, visitor_id);
	if (v != NULL) {
		return ALREADY_IN_ROOM;
	}
	for (int i = 0; i < sys->number_of_rooms; i++) {
		if (strcmp(sys->all_rooms[i].name, room_name) == 0) {
			Visitor *visitor = malloc(sizeof(*visitor));
			if (!visitor) {
				return MEMORY_PROBLEM;
			}
			Result r = init_visitor(visitor, visitor_name, visitor_id);
			if (r != OK) {
				free(visitor);
				return r;
			}
			r = visitor_enter_room(&(sys->all_rooms[i]), visitor, level,
					start_time);
			if (r != OK) {
				free(visitor);
			} else {
				//r == OK
				add_visitor_to_list(sys, visitor);
				sys->last_time_on_system = start_time; // update system time
			}
			return r;
		}
	}
	return NO_AVAILABLE_CHALLENGES;
}

/**
 * visitor_quit - finish the visitor activity in the system. take him out
 * 					from his current room.
 */
Result visitor_quit(ChallengeRoomSystem *sys, int visitor_id, int quit_time) {
	if (!sys) {
		return NULL_PARAMETER;
	}
	if (quit_time < sys->last_time_on_system) {
		return ILLEGAL_PARAMETER;
	}
	struct visitor_node *visitor_node = malloc(sizeof(*visitor_node));
	if (!visitor_node) {
		return MEMORY_PROBLEM;
	}
	visitor_node = find_visitor_node(sys->visitor_list, visitor_id);
	if (visitor_node == NULL) {
		free(visitor_node);
		return NOT_IN_ROOM;
	}
	visitor_quit_room(visitor_node->visitor_n, quit_time);
	sys->last_time_on_system = quit_time;
	remove_visitor_from_list(sys, visitor_node->visitor_n);
	return OK;
}

/**
 * all_visitor_quit - finish all visitors actions in the system
 */
Result all_visitors_quit(ChallengeRoomSystem *sys, int quit_time) {
	if (!sys) {
		return NULL_PARAMETER;
	}
	if (quit_time < sys->last_time_on_system) {
		return ILLEGAL_TIME;
	}
	sys->last_time_on_system = quit_time;
	while (sys->visitor_list != NULL) {
		visitor_quit(sys, sys->visitor_list->visitor_n->visitor_id, quit_time);
	}
	return OK;
}

/**
 * system_room_of_visitor - find the room where the visitor is inside.
 * 							input param is the visitor_name
 * OUTPUT - room name where the visitor is found
 */
Result system_room_of_visitor(ChallengeRoomSystem *sys, char *visitor_name,
		char **room_name) {
	if (!sys) {
		return NULL_PARAMETER;
	}
	if (!room_name || !visitor_name) {
		return ILLEGAL_PARAMETER;
	}
	struct visitor_node *my_node = find_visitor_node_by_name(sys->visitor_list,
			visitor_name);
	if (!my_node) {
		return NOT_IN_ROOM;
	}
	Result r = room_of_visitor(my_node->visitor_n, room_name);
	return r;
}

/**
 * change_challenge_name - change the name of a paticular challenge in the
 * 							system.
 */
Result change_challenge_name(ChallengeRoomSystem *sys, int challenge_id,
		char *new_name) {
	if (!sys || !new_name) {
		return NULL_PARAMETER;
	}
	for (int i = 0; i < sys->number_of_challenges; i++) {
		if (sys->all_challenges[i].id == challenge_id) {
			Result r = change_name(&(sys->all_challenges[i]), new_name);
			return r;
		}
	}
	return ILLEGAL_PARAMETER;
}

/**
 * change_system_room_name - change the name of a room in the system.
 */
Result change_system_room_name(ChallengeRoomSystem *sys, char *current_name,
		char *new_name) {
	if (!sys || !current_name || !new_name) {
		return NULL_PARAMETER;
	}
	for (int i = 0; i < sys->number_of_rooms; i++) {
		if (strcmp(sys->all_rooms[i].name, current_name) == 0) {
			Result r = change_room_name(&(sys->all_rooms[i]), new_name);
			return r;
		}
	}
	return ILLEGAL_PARAMETER;
}

/**
 * INPUT - system, and challenge name
 * OUTPUT - return the best time of the input challenge
 */
Result best_time_of_system_challenge(ChallengeRoomSystem *sys,
		char *challenge_name, int *time) {
	if (!sys || !challenge_name || !time) {
		return NULL_PARAMETER;
	}
	for (int i = 0; i < sys->number_of_challenges; i++) {
		if (strcmp(sys->all_challenges[i].name, challenge_name) == 0) {
			Result r = best_time_of_challenge(&(sys->all_challenges[i]), time);
			return r;
		}
	}
	return ILLEGAL_PARAMETER;
}

/**
 * most_popular_challenge - return the name of the popular challenge in the
 * entire system.
 */
Result most_popular_challenge(ChallengeRoomSystem *sys, char **challenge_name) {
	if (!sys || !challenge_name) {
		return NULL_PARAMETER;
	}
	int popularity_holder = 0;
	int most_popular_holder = 0;
	for (int i = 0; i < sys->number_of_challenges; i++) {
		Result r = num_visits(&(sys->all_challenges[i]), &popularity_holder);
		if (r != OK) {
			return r;
		}
		if (popularity_holder > most_popular_holder) {
			*challenge_name = string_duplicate((sys->all_challenges[i].name));
			if (*challenge_name == NULL) {
				return MEMORY_PROBLEM;
			}
			most_popular_holder = popularity_holder;
		} else if (popularity_holder == most_popular_holder) {
			if (*challenge_name == NULL) {
				*challenge_name = string_duplicate(sys->all_challenges[i].name);
				if (*challenge_name == NULL) {
					return MEMORY_PROBLEM;
				}
			} else {
				if (strcmp(sys->all_challenges[i].name, *challenge_name) < 0) {
					*challenge_name = string_duplicate(
							sys->all_challenges[i].name);
					if (*challenge_name == NULL) {
						return MEMORY_PROBLEM;
					}
				}
			}
		}
	}
	return OK;
}
