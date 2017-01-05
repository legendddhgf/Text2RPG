#include "GameEngine.h"

#define NOPARSEMODE 0
#define ROOMPARSEMODE 1
#define GROUPPARSEMODE 2

GameEngine::GameEngine (FILE *fp) {
    list_rooms.clear(); // are these done automatically?
    list_room_descriptions.clear();
    map_room_opts.clear();
    map_room_tags.clear();

    int parseMode = NOPARSEMODE;
    string lastGroup = string(""); // when parseMode == GROUPPARSEMODE, this is the key into the group maps
    char *buf = (char *) calloc(1024, sizeof(char)); // this is simply a temporary string which gets trimmed to modstr
    char *modstr = (char *) calloc(1024, sizeof(char)); // THIS IS THE LINE WE ARE READING!!!
    int linecount = 0; // helps the user debug their text file
    while (fgets(buf, 1023, fp)) {
        linecount++;
        stringTrim(buf, "\n\r ", modstr); // trim newline, carriage return, and space from front and back
        if (strlen(modstr) == 0) { // skip empty lines
            continue;
        }
        if (strlen(modstr) == 1) {
            fprintf(stderr, "line %d: No parameters specified\n", linecount);
            exit(1);
        }
        char tag = modstr[0]; // in this case, by tag we mean the character of the line (IE: 'r', 'd', 'g', etc.)
        strncpy(buf, modstr, strlen(modstr) + 1); // this line and the following one trim everything from
        stringTrim(buf + 1, "\n\r ", modstr);     // the tag to the beginning of the parameter
        // (room for 'r', description for 'd', etc)
        switch (tag) { // REMEMBER: at this point modstr only contains the parameter, tag contains the current tag
            case 'r': // room
                {
                    parseMode = ROOMPARSEMODE;
                    if (list_rooms.size() != list_room_descriptions.size()) { // one description per room required
                        fprintf(stderr, "%s: Room has no description\n", list_rooms.back().c_str());
                        exit(1);
                    }
                    if (map_room_opts.size() != map_room_tags.size()) {
                        fprintf(stderr, "line %d: There must be one tag for every option\n", linecount);
                        exit(1);
                    }
                    if (list_rooms.size() == 0) { // don't check tags if there are no rooms yet
                        list_rooms.push_back(string(modstr));
                        break;
                    }
                    auto opts_iter = map_room_opts.find(list_rooms.back()); // the next ~10 lines are for checking opts : tags
                    auto tags_iter = map_room_tags.find(list_rooms.back());
                    if (opts_iter == map_room_opts.end() && tags_iter == map_room_tags.end()) { // no tags or opts, thus equal
                        list_rooms.push_back(string(modstr));
                        break;
                    }
                    if (opts_iter == map_room_opts.end() || // only opts has nothing
                            tags_iter == map_room_tags.end() || // or only tags has nothing
                            opts_iter->second.size() != tags_iter->second.size()) { // or for key: numopts != numtags
                        fprintf(stderr, "line %d: There must be one tag for every option\n", linecount);
                        exit(1);
                    }
                    list_rooms.push_back(string(modstr));
                    break;
                }
            case 'd': // description
                // NOTE: that there is only ever one description per room. The
                // functionality of having multiple descriptions is implemented
                // by appending to the previous description string for a given
                // room if it already exists
                if (parseMode != ROOMPARSEMODE) {
                    fprintf(stderr, "line %d: Cannot provide descriptions unless parsing for rooms\n", linecount);
                    exit(1);
                }
                if (list_rooms.size() == 0) {
                    fprintf(stderr, "line %d: Can't have a description without rooms\n", linecount);
                    exit(1);
                }
                if (list_rooms.size() == list_room_descriptions.size()) { // not first description segment
                    string temp = list_room_descriptions.back() + string("\t") + string(modstr) + string("\n");
                    list_room_descriptions.pop_back();
                    list_room_descriptions.push_back(temp);
                }
                else if (list_rooms.size() == list_room_descriptions.size() + 1) {
                    list_room_descriptions.push_back(string("\t") + string(modstr) + string("\n")); // first description segment
                } else {
                    fprintf(stderr, "%s: Room has no description\n", list_rooms.back().c_str());
                    exit(1);
                }
                break;
            case 'o':
                {
                    if (parseMode == NOPARSEMODE) {
                        fprintf(stderr, "line %d: Can't specify options without rooms or groups\n", linecount);
                        exit(1);
                    }
                    if (parseMode == ROOMPARSEMODE) {
                        if (list_rooms.size() == 0) {
                            fprintf(stderr, "line %d: Must have at least one room to specify options\n", linecount);
                            exit(1);
                        }
                        if (map_room_opts.find(list_rooms.back()) != map_room_opts.end() &&
                                map_room_tags.find(list_rooms.back()) != map_room_tags.end() &&
                                map_room_opts.find(list_rooms.back())->second.size() !=
                                map_room_tags.find(list_rooms.back())->second.size()) { // numopts should equal numtags at this point
                            fprintf(stderr, "line %d: Expected another tag first\n", linecount);
                            exit(1);
                        }
                        // if there are no transitions from the current room, insert
                        // a blank one
                        if (map_room_opts.find(list_rooms.back()) == map_room_opts.end()) {
                            map_room_opts.insert(pair<string, vector<string>>(list_rooms.back(), {}));
                        }
                        // finally, append to the appropriate vector
                        map_room_opts.find(list_rooms.back())->second.push_back(string(modstr));
                        break;
                    } else if (parseMode == GROUPPARSEMODE) {
                        // TODO: dat functionality
                    }
                }
            case 't':
                {
                    if (parseMode == NOPARSEMODE) {
                        fprintf(stderr, "line %d: Can't specify tags without rooms or groups\n", linecount);
                        exit(1);
                    }
                    if (parseMode == ROOMPARSEMODE) {
                        if (list_rooms.size() == 0) {
                            fprintf(stderr, "line %d: Must have at least one room to specify tags\n", linecount);
                            exit(1);
                        }
                        if (map_room_opts.find(list_rooms.back()) == map_room_opts.end() || // can't have tags before opts
                                (map_room_opts.find(list_rooms.back()) != map_room_opts.end() &&
                                 map_room_tags.find(list_rooms.back()) != map_room_tags.end() &&
                                 map_room_opts.find(list_rooms.back())->second.size() ==
                                 map_room_tags.find(list_rooms.back())->second.size())) { // numopts should equal numtags - 1 at this point
                            fprintf(stderr, "line %d: Expected another option first\n", linecount);
                            exit(1);
                        }
                        // if there are no transitions from the current room, insert
                        // a blank one
                        if (map_room_tags.find(list_rooms.back()) == map_room_tags.end())
                            map_room_tags.insert(pair<string, vector<string>>(list_rooms.back(), {}));
                        // finally, append to the appropriate vector
                        map_room_tags.find(list_rooms.back())->second.push_back(string(modstr));
                        break;
                    } else if (parseMode ==  GROUPPARSEMODE) {
                        // TODO: dat functionality
                    }
                }
            case 'g':
                // TODO: PLZ
                break;
            case '#': // support those comments
                break;
            default:
                fprintf(stderr, "%c: No Corresponding field\n", modstr[0]);
                exit(1);
                break;
        }
    }
    if (list_rooms.size() != list_room_descriptions.size()) { // one description per room required
        fprintf(stderr, "%s: Room has no description\n", list_rooms.back().c_str());
        exit(1);
    }
    // check that for all keys num opts = num tags and all opts are valid rooms
    // (will have to add checking for groups later)
    // TODO

    // print all info
    for (size_t i = 0; i < list_rooms.size(); i++) {
        string room = list_rooms.at(i);
        string description = list_room_descriptions.at(i);
        fprintf(stdout, "%zd: %s\n", i + 1, room.c_str());
        fprintf(stdout, "%s", description.c_str()); // assumes description was modified in a certain way first
        if (map_room_opts.find(room) == map_room_opts.end()) continue; // if no opts/tags
        auto list_opts = map_room_opts.at(room);
        auto list_tags = map_room_tags.at(room);
        for (size_t k = 0; k < list_opts.size(); k++) { // since numopts = numtags
            printf("\t\tOption: %s\n", list_opts.at(k).c_str());
            printf("\t\t\tTag: %s\n", list_tags.at(k).c_str());
        }
    }
    free(buf);
    free(modstr);
}
