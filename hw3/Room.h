//
// Created by Samuel Panzieri on 5/30/2017.
//

#ifndef HW3_2_ROOM_H
#define HW3_2_ROOM_H
#define TIME_STR_LEN 5

typedef struct room *Room;

Room createRoom(unsigned int id, unsigned int price, int num_ppl,
                char* work_hr[TIME_STR_LEN], int difficulty);
void roomDestroy (Room room);

Room roomCopy(Room room);
int roomCompare(Room room_1, Room room_2);

#endif //HW3_2_ROOM_H
