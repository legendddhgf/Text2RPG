#ifndef _GAMEENGINE_H_
#define _GAMEENGINE_H_

#include "common.h"

typedef struct GameEngine {

    vector <string> list_rooms;
    vector <string> list_room_descriptions;
    // for the following two, the key is the current room
    unordered_map <string, vector <string>> map_room_opts; // value is list of room/group names
    unordered_map <string, vector <string>> map_room_tags; // value is list of descriptions for transitions
    // each pair is the group name with a list of rooms/subgroups and corresponding
    // priorities (to assign possibility of certain rooms being chosen)
    // regex is supported for subgroup and room names in accordance to the
    // specified file format
    // subgroups are defined as those with a matching key in one of the group
    // maps (since they should either be in both or neither)
    unordered_map <string, vector <pair<string, int>>> map_group_contents;
    // NOTE: this applies to the following two
    // each pair is the group name and a vector of strings (opts or tags)
    // note that regex cannot be used for opts in this case because opts : tags
    // must be received in a 1 : 1 ratio (you can't have two opts and the two
    // tags to compensate: must alternate)
    // subgroups are defined as those with a matching key in one of the group
    // maps (since they should either be in all or neither)
    unordered_map <string, vector <string>> map_group_opts;
    unordered_map <string, vector <string>> map_group_tags;

    GameEngine(FILE *);

} GameEngine;


#endif
