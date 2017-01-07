#include "common.h"

// trims from beginning and end of s, all characters in trim and copies it
// to ret.
// This was designed with the intention of avoiding reassignment of pointers
// to manually allocated blocks.
void stringTrim (char *s, const char *trim, char *ret) {
  size_t i = 0;
  // first pass skips front
  while (i < strlen(s)) { // sufficient condition because srlen decreases
    if (strchr(trim, s[i]) == nullptr) {
      break; // if beginning doesn't need to be trimmed anymore
    }
    i++;
  }
  // second pass puts the rest of s in ret
  strncpy(ret, s + i, strlen(s + i) + 1); //copy the null character too

  // third pass trims back of ret
  for (int i = strlen(ret) - 1; i >= 0; i--) {
    if (strchr(trim, ret[i]) == nullptr) {
      ret[i + 1] = '\0'; // trim everything after this character
      break;
    }
  }
}

// expects a char * that is parameters separated by a combination of atlest one
// of the characters in delimiters.
// Will append parameters to the params vector (may append nothing)
// Note that empty strings will never be appended
void stringToParams(char *str, vector<string> *params, const char *delimiters) {
  string temp = string("");
  for (size_t i = 0; i < strlen(str); i++) {
    if (strchr(delimiters, str[i]) != nullptr && temp.size() > 0) {
      params->push_back(temp);
      temp = string("");
      continue;
    } else if (strchr(delimiters, str[i]) != nullptr) continue;
    temp.append(1, str[i]); // unfortunately unintuitive way of appending chars
  }
  if (temp.size() > 0) {
    params->push_back(temp);
  }
}
