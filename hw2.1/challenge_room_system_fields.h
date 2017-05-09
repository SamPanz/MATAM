/* TO BE COMPLETED HERE THE FIELDS OF THE DATA TYPE ChallengeRoomSystem */
#include "challenge.h"

char *system_name;
Challenge *all_challenges;
ChallengeRoom *all_rooms;
int number_of_rooms;
int number_of_challenges;
int last_time_on_system;
struct visitor_node
{
	Visitor *visitor_n;
	struct visitor_node *next;
} *visitor_list;
