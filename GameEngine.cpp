#include "GameEngine.h"
GameEngine::GameEngine (FILE *fp) {
    list_rooms.clear(); // are these done automatically?
    list_description.clear();
    map_opts.clear();
    map_tags.clear();

    char *buf = (char *) calloc(1024, sizeof(char));
    char *modstr = (char *) calloc(1024, sizeof(char)); // modstr can be at most str size of buf + 1
    while (fgets(buf, 1023, fp)) {
        stringTrim(buf, "\n\r ", modstr); // trim newline, carriage return, and space from front and back
        if (strlen(modstr) == 0) { // skip empty lines
            continue;
        }
        switch (modstr[0]) {
            case 'r': // room
                if (list_rooms.size() != list_description.size()) { // one description per room required
                    fprintf(stderr, "%s: Room has no description\n", list_rooms.back().c_str());
                    exit(1);
                }
                // TODO: check that all options have atleast one tag
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
                // string for indexing into map is back of room list
                //if (!inserted into opts) {
                    //insert empty vector
                //}
                // append to the vector
                break;
            case 't':
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
