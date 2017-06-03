//
// Created by Samuel Panzieri on 5/30/2017.
//

#include "Room.h"
#include "stdlib.h"
#include "Utilities_Functions.c"
struct room{
    unsigned int id;
    unsigned int price;
    int num_ppl;
    char work_hrs[TIME_STR_LEN];
    int difficulty; // 1-10
};

Room createRoom(unsigned int id, unsigned int price, int num_ppl,
                char* work_hr[TIME_STR_LEN], int difficulty)
{
  Room new_room = malloc(sizeof(Room));
    if (!new_room)
        return NULL;
    new_room->work_hrs=stringDuplicate(work_hr);
    if (!new_room->work_hrs){
        free(new_room);
        return NULL;
    }
    new_room->num_ppl=num_ppl;
    new_room->difficulty=difficulty;
    new_room->id=id;
    new_room->price=price;
    return new_room;
}

void roomDestroy (Room room){
    assert(room);
    free(room->work_hrs);
    free(room);
}

Room roomCopy(Room room){
    assert(room);
   Room new_room=malloc(sizeof(Room));
    if (!new_room)
        return NULL;
    new_room->work_hrs=stringDuplicate(room->work_hrs);
    if(!new_room->work_hrs) {
        free(room);
        return NULL;
    }
    new_room->price=room->price;
    new_room->id=room->id;
    new_room->difficulty=room->difficulty;
    new_room->num_ppl=room->num_ppl;
    return new_room;
}
int roomCompare(Room room_1, Room room_2){
    assert(room_1 && room_2);
    return (room_2->id - room_1->id);/*returns 0 if they're the same room*/
}
