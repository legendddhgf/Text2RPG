#include "GameEngine.h"

#define PARSEMODENONE 0
#define PARSEMODEROOM 1
#define PARSEMODEROOMOPTION 2 // unfortunate development right here
#define PARSEMODEITEM 3

#define PRIORITYDEFAULT 100



GameEngine::GameEngine (FILE *fp) {
  map_room_descriptions.clear(); // are these done automatically?
  map_room_contents.clear();
  map_room_opts.clear();
  map_room_tags.clear();

  int parseMode = PARSEMODENONE;
  string lastKey = string(""); // the key for the current parseMode
  // (IE: the current room if PARSEMODEROOM)
  char *buf = (char *) calloc(1024, sizeof(char)); // this is simply a
  // temporary string which
  // gets trimmed to modstr
  char *modstr = (char *) calloc(1024, sizeof(char)); // THIS IS THE STRING
  // WE ARE USING!!!
  int linecount = 0; // helps the user debug their text file
  while (fgets(buf, 1023, fp)) {
    linecount++;
    if (strlen(modstr) >= 1000) {
      fprintf(stderr, "line %d: Too long\n", linecount);
      exit(1);
    }
    stringTrim(buf, "\n\r ", modstr); // trim newline, carriage return,
    // and space from front and back
    if (strlen(modstr) == 0) { // skip empty lines
      continue;
    }
    if (strlen(modstr) == 1) {
      fprintf(stderr, "line %d: No parameters specified\n", linecount);
      exit(1);
    }
    char tag = modstr[0]; // in this case, by tag we mean the character of
    // the line (IE: 'r', 'd', 'g', etc.)
    strncpy(buf, modstr, strlen(modstr) + 1); // this line and the
    // following one trim
    // everything from
    stringTrim(buf + 1, "\n\r ", modstr);     // the tag to the beginning
    // of the parameter
    vector<string> params = {};
    stringToParams(modstr, &params, " ");
    // (room for 'r', description for 'd', etc)
    switch (tag) { // REMEMBER: at this point modstr only contains the
      // parameter, tag contains the current tag
      case 'r': // room
        {
          if (params.size() != 1) {
            fprintf(stderr, "line %d: 'r' takes one parameter\n", linecount);
            exit(1);
          }
          // TODO: create a function that checks if it is ok to
          // change parsemodes, in other words, checks that the
          // previous thing being modified is ok up to this point
          parseMode = PARSEMODEROOM;

          /* VERY BAD BECAUSE WE DON'T KNOW IF LASTKEY IS A ROOM!,
           * just make a complete checking function already
           if (map_room_descriptions.size() > 0 &&
          // if desc is blank, none specified
          map_room_descriptions.at(lastKey).compare(string("")) == 0) {
          fprintf(stderr, "%s: Room has no description\n",
          list_rooms.back().c_str());
          exit(1);
          }
          */

          if (map_room_opts.size() != map_room_tags.size()) {
            fprintf(stderr, "line %d: There must be one tag for every option\n",
                linecount);
            exit(1);
          }
          lastKey = params.at(0); // key is now the current room
          if (map_room_descriptions.find(lastKey) !=
              map_room_descriptions.end()) {
            fprintf(stderr, "line %d: Can't have duplicate room names\n",
                linecount);
            exit(1);
          }
          if (map_room_descriptions.size() == 0) { // don't check tags if there
            // are no rooms yet
            map_room_descriptions.insert(pair<string, // new entry
                string>(lastKey, string("")));
            break;
          }
          auto opts_iter = map_room_opts.find(lastKey); // the next ~10 lines
          // are for checking
          // opts : tags
          auto tags_iter = map_room_tags.find(lastKey);
          if (opts_iter == map_room_opts.end() &&
              tags_iter == map_room_tags.end()) { // no tags or opts, thus equal
            map_room_descriptions.insert(pair<string,
                string>(lastKey, string(""))); // if unchanged, if
            // statement above is trigerred
            break;
          }
          if (opts_iter == map_room_opts.end() || // only opts has nothing
              tags_iter == map_room_tags.end() || // or only tags has nothing
              opts_iter->second.size() !=
              tags_iter->second.size()) { // or for key: numopts != numtags
            fprintf(stderr, "line %d: There must be one tag for every option\n",
                linecount);
            exit(1);
          }
          map_room_descriptions.insert(pair<string,
              string>(lastKey, string("")));
          break;
        }
      case 'd': // description
        // NOTE: that there is only ever one description per room. The
        // functionality of having multiple descriptions is implemented
        // by appending to the previous description string for a given
        // room if it already exists
        if (parseMode == PARSEMODENONE) {
          fprintf(stderr, "line %d: Cannot provide descriptions %s",
              linecount, "if not parsing for anything\n");
          exit(1);
        }
        if (parseMode == PARSEMODEROOM) {
          if (map_room_descriptions.size() == 0) {
            fprintf(stderr, "line %d: Can't have a description without rooms\n",
                linecount);
            exit(1);
          }
          // in all cases, append: we know the entry must exist at
          // this point
          map_room_descriptions.at(lastKey).append(string("\t")
              + params.at(0) + string("\n"));
        } else if (parseMode == PARSEMODEITEM) {
          // TODO: PLZ
        }
        break;
      case 'o':
        {
          if (parseMode == PARSEMODENONE) {
            fprintf(stderr, "line %d: Cannot provide options %s",
                linecount, "if not parsing for anything\n");
            exit(1);
          }
          if (parseMode == PARSEMODEROOM) {
            if (map_room_opts.find(lastKey) != map_room_opts.end() &&
                map_room_tags.find(lastKey) != map_room_tags.end() &&
                map_room_opts.find(lastKey)->second.size() !=
                map_room_tags.find(lastKey)->second.size()) { // numopts should
              // equal numtags
              // at this point
              fprintf(stderr, "line %d: Expected another tag first\n",
                  linecount);
              exit(1);
            }
            // if there are no transitions from the current room, insert
            // a blank one
            if (map_room_opts.find(lastKey) == map_room_opts.end()) {
              map_room_opts.insert(pair<string, vector<string>>(lastKey, {}));
            }
            // finally, append to the appropriate vector
            map_room_opts.find(lastKey)->second.push_back(params.at(0));
          } else if (parseMode == PARSEMODEITEM) {
            // TODO: dat functionality
          }
          break;
        }
      case 't':
        {
          if (parseMode == PARSEMODENONE) {
            fprintf(stderr, "line %d: Cannot provide tags %s",
                linecount, "if not parsing for anything\n");
            exit(1);
          }
          if (parseMode == PARSEMODEROOM) {
            // can't have tags before opts
            if (map_room_opts.find(lastKey) == map_room_opts.end() ||
                (map_room_opts.find(lastKey) != map_room_opts.end() &&
                 map_room_tags.find(lastKey) != map_room_tags.end() &&
                 map_room_opts.find(lastKey)->second.size() ==
                 map_room_tags.find(lastKey)->second.size())) {
              // numopts should equal numtags - 1 at this point
              fprintf(stderr, "line %d: Expected another option first\n",
                  linecount);
              exit(1);
            }
            // if there are no transitions from the current room, insert
            // a blank one
            if (map_room_tags.find(lastKey) == map_room_tags.end())
              map_room_tags.insert(pair<string, vector<string>>(lastKey, {}));
            // finally, append to the appropriate vector
            map_room_tags.find(lastKey)->second.push_back(params.at(0));
          } else if (parseMode == PARSEMODEITEM) {
            // TODO: dat functionality
          }
          break;
        }
      case 'm':
        if (parseMode == PARSEMODENONE) {
          fprintf(stderr, "line %d: 'm' is an invalid tag %s",
              linecount, "when not parsing for anything\n");
          exit(1);
        }
        if (parseMode == PARSEMODEROOM) {
          if (params.size() > 2) {
            fprintf(stderr, "line %d: in room parsing, 'm' %s",
                linecount, "takes one or two parameters\n");
            exit(1);
          }
          // Cannot have a room be a member of itself
          if (params.at(0).compare(lastKey) == 0) {
            fprintf(stderr, "line %d: %s cannot be a member of itself\n",
                linecount, modstr);
            exit(1);
          }
          if (map_room_contents.find(lastKey) == map_room_contents.end()) {
            // sometimes life isn't simple but it could be
            map_room_contents.insert(pair<string,
                vector<tuple<string, string>>>(lastKey, {}));
          }
          if (params.size() == 1) {
            map_room_contents.find(lastKey)->second.emplace_back(
                params.at(0), to_string(PRIORITYDEFAULT));
          } else {
            if (params.at(1).find_first_not_of("1234567890") != string::npos) {
              fprintf(stderr, "line %d: Priority must be an integer\n",
                  linecount);
              exit(1);
            }
            map_room_contents.find(lastKey)->second.emplace_back(
                params.at(0), params.at(1));
          }
        } else if (parseMode == PARSEMODEITEM) {
          // TODO PLZ
        }
        break;
      case 'e':
        // TODO
        break;
      case 'a':
        // TODO
        break;
      case 'i':
        // TODO: items PLZ
        // TODO: create a function that checks if it is ok to
        // change parsemodes, in other words, checks that the
        // previous thing being modified is ok up to this point
        parseMode = PARSEMODEITEM;
        break;
      case '#': // support those comments
        // Yes, this works.
        break;
      default:
        fprintf(stderr, "%c: No Corresponding field\n", tag);
        exit(1);
        break;
    }
  }
  /* General checking function needed
     if (list_rooms.size() != list_room_descriptions.size()) {
// one description per room required
fprintf(stderr, "%s: Room has no description\n",
list_rooms.back().c_str());
exit(1); }
*/
  // TODO resolve regex
  // TODO:
  // check that for all keys num opts = num tags and all opts are valid rooms
  // (will have to add checking for non-visitable rooms later)

  // print all info
  auto iter_room = map_room_descriptions.begin();
  int count_rooms = 1;
  fprintf(stdout, "ROOMS:\n=====\n");
  while (iter_room != map_room_descriptions.end()) {
    string room = iter_room->first;
    string description = iter_room->second;
    iter_room++;
    fprintf(stdout, "%d: %s\n", count_rooms++, room.c_str());
    // assumes description was modified in a certain way first
    fprintf(stdout, "%s", description.c_str());
    if (map_room_contents.find(room) != map_room_contents.end()) {
      fprintf(stdout, "\tHas Members:\n");
      auto list_contents = map_room_contents.find(room)->second;
      for (size_t k = 0; k < list_contents.size(); k++) {
        fprintf(stdout, "\t\t%s: Priority=%s\n",
            get<0>(list_contents.at(k)).c_str(),
            get<1>(list_contents.at(k)).c_str());
      }
    }
    // if no opts/tags
    if (map_room_opts.find(room) == map_room_opts.end()) continue;
    fprintf(stdout, "\tHas Options and Tags:\n");
    auto list_opts = map_room_opts.at(room);
    auto list_tags = map_room_tags.at(room);
    for (size_t k = 0; k < list_opts.size(); k++) { // since numopts = numtags
      fprintf(stdout, "\t\tOption: %s\n", list_opts.at(k).c_str());
      fprintf(stdout, "\t\t\tTag: %s\n", list_tags.at(k).c_str());
    }
  }
  fprintf(stdout, "=====\nITEMS:\n=====\n");
  while(false) { // this is where you would print out items

  }
  fprintf(stdout, "=====\n");
  free(buf);
  free(modstr);
}
