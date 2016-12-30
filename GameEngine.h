#ifndef _GAMEENGINE_H_
#define _GAMEENGINE_H_

#include "common.h"

typedef struct GameEngine {

    vector <string> list_rooms;
    vector <string> list_description;
    // for the following three, the key is the current room
    unordered_map <string, vector <string>> map_opts; // list of room/group names
    unordered_map <string, vector <string>> map_tags; // list of descriptions for transitions
    // each pair is the group name with a list of rooms and corresponding
    // priorities (to assign possibility of certain rooms being chosen)
    unordered_map <string, vector <pair<string, int>>> map_groups;

    GameEngine(FILE *);

} GameEngine;


#endif
