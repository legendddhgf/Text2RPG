#ifndef _GAMEENGINE_H_
#define _GAMEENGINE_H_

#include "common.h"

typedef struct GameEngine {

  // key is room name, value is description
  unordered_map <string, string> map_room_descriptions;
  // each pair is the room name with a list of rooms and corresponding
  // priorities (to assign possibility of certain rooms being chosen)
  // regex is supported for room names
  unordered_map <string, vector <tuple<string, string>>> map_room_contents;
  // NOTE: this applies to the following two
  // each pair is the room name and a vector of strings (opts or tags)
  // note that regex cannot be used for opts in this case because opts : tags
  // must be received in a 1 : 1 ratio (you can't have two opts and the two
  // tags to compensate: must alternate)
  unordered_map <string, vector <string>> map_room_opts;
  unordered_map <string, vector <string>> map_room_tags;

  GameEngine(FILE *);

} GameEngine;


#endif
