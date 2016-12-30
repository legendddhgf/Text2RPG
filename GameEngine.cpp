#include "GameEngine.h"

GameEngine::GameEngine (FILE *fp) {
    list_rooms.clear(); // are these done automatically?
    list_description.clear();
    map_opts.clear();
    map_tags.clear();

    char *buf = (char *) calloc(1024, sizeof(char)); // this is simply a temporary string which gets trimmed to modstr
    char *modstr = (char *) calloc(1024, sizeof(char)); // THIS IS THE LINE WE ARE READING!!!
    int linecount = 0;
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
        switch (modstr[0]) {
            case 'r': // room
                if (list_rooms.size() != list_description.size()) { // one description per room required
                    fprintf(stderr, "%s: Room has no description\n", list_rooms.back().c_str());
                    exit(1);
                }
                if (map_opts.size() != map_tags.size()) {
                    fprintf(stderr, "line %d: There must be one tag for every option\n", linecount);
                    exit(1);
                }
                list_rooms.push_back(string(modstr + 2)); // skip the 'r' and the ' '
                break;
            case 'd': // description
                if (list_rooms.size() == 0) {
                    fprintf(stderr, "Can't have a description without rooms\n");
                    exit(1);
                }
                if (list_rooms.size() == list_description.size()) { // not first description segment
                    string temp = list_description.back() + string("\t\t") + string(modstr + 2) + string("\n");
                    list_description.pop_back();
                    list_description.push_back(temp);
                }
                else if (list_rooms.size() == list_description.size() + 1) {
                    list_description.push_back(string("\t\t") + string(modstr + 2) + string("\n")); // first description segment
                } else {
                    fprintf(stderr, "%s: Room has no description\n", list_rooms.back().c_str());
                    exit(1);
                }
                break;
            case 'o':
                {
                if (list_rooms.size() == 0) {
                    fprintf(stderr, "line %d: Must have at least one room to specify options\n", linecount);
                    exit(1);
                }
                if (map_opts.size() != map_tags.size()) {
                    fprintf(stderr, "line %d: Expected another tag first\n", linecount);
                    exit(1);
                }
                // if there are no transitions from the current room, insert
                // a blank one
                if (map_opts.find(list_rooms.back()) == map_opts.end())
                    map_opts.insert(pair<string, vector<string>>(list_rooms.back(), {}));
                // finally, append to the appropriate vector
                auto list_opts = map_opts.find(list_rooms.back())->second;
                //list_opts.push_back(string(modstr + 2));
                break;
                }
            case 't':
                {
                if (list_rooms.size() == 0) {
                    fprintf(stderr, "line %d: Must have at least one room to specify tags\n", linecount);
                    exit(1);
                }
                if (map_opts.size() == map_tags.size()) {
                    fprintf(stderr, "line %d: Expected another option first\n", linecount);
                    exit(1);
                }
                // if there are no transitions from the current room, insert
                // a blank one
                if (map_tags.find(list_rooms.back()) == map_tags.end())
                    map_tags.insert(pair<string, vector<string>>(list_rooms.back(), {}));
                // finally, append to the appropriate vector
                auto list_tags = map_tags.find(list_rooms.back())->second;
                //list_tags.push_back(string(modstr + 2));
                break;
                }
            case 'g':
                // TODO: PLZ
                break;
            default:
                fprintf(stderr, "%c: No Corresponding field\n", modstr[0]);
                exit(1);
                break;
        }
    }
    if (list_rooms.size() != list_description.size()) { // one description per room required
        fprintf(stderr, "%s: Room has no description\n", list_rooms.back().c_str());
        exit(1);
    }
    // TODO: check that all opts are in list rooms

    // print rooms and descriptions
    for (size_t i = 0; i < list_rooms.size(); i++) {
        string room = list_rooms.at(i);
        string description = list_description.at(i);
        fprintf(stdout, "%zd: %s\n", i + 1, room.c_str());
        fprintf(stdout, "%s", description.c_str());
    }
    free(buf);
    free(modstr);
}
