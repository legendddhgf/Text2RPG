#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <tuple>
using namespace std;

void stringTrim(char *, const char *, char *);
void stringToParams(char *, vector<string> *, const char *);

#endif
