#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "challenge_system.h"

void printChallenge(Challenge *challenge) {
	assert(challenge != NULL);
	printf("%s", challenge->name);
	printf(", id=%d", challenge->id);
	printf(", level = %d", challenge->level-Easy+1);
	printf(", best_time = %d, ", challenge->best_time);
	printf(", num_visits = %d\n", challenge->num_visits);
}

void printChallengeActivity(ChallengeActivity *activity) {
	printf("the challenge is: %d\n", activity->challenge->id);
	if(activity->visitor == NULL) {
		printf("visitor is null\n");
	}
	else {
		printf("visitor is not null\n");
	}
	printf("start time = %d\n", activity->start_time);
}

void printRoom(ChallengeRoom *room) {
	assert(room != NULL);
	printf("%s", room->name);
	printf(" %d", room->num_of_challenges);
	for(int i=0; i<room->num_of_challenges; i++) {
		printf(" %d", (room->challenges+i)->challenge->id);
	}
	printf("\n");
}

void printResult(Result result) {
	if(result == OK) printf("result = OK\n");
	if(result == NULL_PARAMETER) printf("result = NULL_PARAMETER\n");
	if(result == MEMORY_PROBLEM) printf("result = MEMORY_PROBLEM\n");
	if(result == ILLEGAL_PARAMETER) printf("result = ILLEGAL_PARAMETER\n");
	if(result == NOT_IN_ROOM) printf("result = NOT_IN_ROOM\n");
	if(result == ALREADY_IN_ROOM) printf("result = ALREADY_IN_ROOM\n");
	if(result == NO_AVAILABLE_CHALLENGES) printf("result = NO_AVAILABLE_CHALLENGES\n");
	if(result == ILLEGAL_TIME) printf("result = ILLEGAL_TIME\n");
}

int fail_count = 0;
void checkCondition(int num, bool b) {
	if(b) {
		printf("test %d: success!\n", num);
	} else {
		printf("test %d: failed :(\n", num);
		fail_count++;
	}
}

void myTests() {

	ChallengeRoomSystem *sys=NULL;

	Result r = create_system("test_1.txt", NULL);
	checkCondition(1, r == NULL_PARAMETER);
	r = create_system(NULL, &sys);
	checkCondition(2, r == NULL_PARAMETER);
	r = create_system("test_1.txt", &sys);
	checkCondition(3, r == OK);

	char *challenge_name=NULL;
	r = most_popular_challenge(sys, &challenge_name);
	checkCondition(4, r==OK);
	checkCondition(5, challenge_name == NULL);

	int time;
	r = best_time_of_system_challenge(sys, "challenge_6", &time);
	checkCondition(6, r == OK);
	checkCondition(7, time == 0);

	r = visitor_arrive(NULL, NULL, NULL, 201, Medium, 5);
	checkCondition(8, r == NULL_PARAMETER);
	r = visitor_arrive(sys, NULL, "visitor_1", 201, Medium, 5);
	checkCondition(9, r == ILLEGAL_PARAMETER);
	r = visitor_arrive(sys, "room_1", NULL, 201, Medium, -2);
	checkCondition(10, r == ILLEGAL_TIME);
	r = visitor_arrive(sys, "room_1", "visitor_1", 201, Medium, 3);
	checkCondition(11, r == NO_AVAILABLE_CHALLENGES);
	r = visitor_arrive(sys, "room_2", "visitor_1", 201, Medium, 3);
	checkCondition(12, r == OK);
	r = visitor_arrive(sys, "room_1", "visitor_1", 201, Medium, 1);
	checkCondition(13, r == ILLEGAL_TIME);
	r = visitor_arrive(sys, "room_1", "visitor_1", 201, Medium, 5);
	checkCondition(14, r == ALREADY_IN_ROOM);
	r = visitor_arrive(sys, "room_2", "visitor_1", 201, Medium, 5);
	checkCondition(15, r == ALREADY_IN_ROOM);

	r = visitor_quit(NULL, 13, -4);
	checkCondition(16, r == NULL_PARAMETER);
	r = visitor_quit(sys, 13, -4);
	checkCondition(17, r == ILLEGAL_TIME);
	r = visitor_quit(sys, 13, 4);
	checkCondition(18, r == NOT_IN_ROOM);

	char *room_name=NULL;
	r = system_room_of_visitor(NULL, NULL, NULL);
	checkCondition(19, r == NULL_PARAMETER);
	r = system_room_of_visitor(sys, "visitor_1", NULL);
	checkCondition(20, r == ILLEGAL_PARAMETER);
	r = system_room_of_visitor(sys, "visitor_42", &room_name);
	checkCondition(21, r == NOT_IN_ROOM);
	r = system_room_of_visitor(sys, "visitor_1", &room_name);
	checkCondition(22, r == OK);
	checkCondition(23, strcmp(room_name, "room_2")==0);
	free(room_name);

	r = change_challenge_name(sys, 42, NULL);
	checkCondition(24, r == NULL_PARAMETER);
	r = change_challenge_name(sys, 42, "challenge_222");
	checkCondition(25, r == ILLEGAL_PARAMETER);
	r = change_challenge_name(sys, 22, "challenge_222");
	checkCondition(26, r == OK);

	r = change_system_room_name(sys, NULL, "kawabanga");
	checkCondition(27, r == NULL_PARAMETER);
	r = change_system_room_name(sys, "kawabanga", "bazinga");
	checkCondition(28, r == ILLEGAL_PARAMETER);
	r = change_system_room_name(sys, "room_1", "room_1s");
	checkCondition(29, r == OK);

	r = best_time_of_system_challenge(NULL, "challenge_2", &time);
	checkCondition(30, r == NULL_PARAMETER);
	r = best_time_of_system_challenge(sys, "challenge_2", NULL);
	checkCondition(31, r == NULL_PARAMETER);
	r = best_time_of_system_challenge(sys, "challenge_2", &time);
	checkCondition(32, r == ILLEGAL_PARAMETER);
	r = best_time_of_system_challenge(sys, "challenge_222", &time);
	checkCondition(33, r == OK);
	checkCondition(34, time == 0);

	r = most_popular_challenge(sys, NULL);
	checkCondition(35, r == NULL_PARAMETER);
	r = most_popular_challenge(sys, &challenge_name);
	checkCondition(36, r == OK);
	checkCondition(37, strcmp(challenge_name, "challenge_222") == 0);
	free(challenge_name);

	r = visitor_quit(NULL, 42, 2);
	checkCondition(38, r == NULL_PARAMETER);
	r = visitor_quit(sys, 42, 2);
	checkCondition(39, r == ILLEGAL_TIME);
	r = visitor_quit(sys, 42, 7);
	checkCondition(40, r == NOT_IN_ROOM);
	r = visitor_quit(sys, 201, 5);
	checkCondition(41, r == OK);

	r = all_visitors_quit(NULL, 8);
	checkCondition(42, r == NULL_PARAMETER);
	r = all_visitors_quit(sys, 4);
	checkCondition(43, r == ILLEGAL_TIME);
	r = all_visitors_quit(sys, 6);
	checkCondition(44, r == OK);

	char *most_popular_name=NULL, *best_time_name=NULL;
	r = destroy_system(sys, 2, &most_popular_name, NULL);
	checkCondition(45, r == NULL_PARAMETER);
	r = destroy_system(sys, 10, NULL, &best_time_name);
	checkCondition(46, r == NULL_PARAMETER);
	r = destroy_system(sys, 8, &most_popular_name, &best_time_name);
	checkCondition(47, r == OK);
	checkCondition(48, strcmp(most_popular_name, "challenge_222") == 0);
	checkCondition(49, strcmp(best_time_name, "challenge_222") == 0);
	free(most_popular_name);
	free(best_time_name);
	if(fail_count == 0) {
		printf("all tests passed :)\n");
	}
	else {
		printf("number of fails: %d\n", fail_count);
	}
}

int omo() {
	setvbuf(stdout, NULL, _IONBF, 0);
	myTests();
	return 0;
}



