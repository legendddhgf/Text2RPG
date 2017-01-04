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
