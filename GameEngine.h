#ifndef _GAMEENGINE_H_
#define _GAMEENGINE_H_

#include "common.h"

typedef struct GameEngine {

    vector <string> list_rooms;
    vector <string> list_description;

    GameEngine(FILE *);

} GameEngine;


#endif
