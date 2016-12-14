#ifndef _ARGPARSER_H_
#define _ARGPARSER_H_

#include "common.h"

typedef struct ArgParser {
   private:
      vector<string> opts; // anything with a '-' prefix
      unordered_map<string, string> fields; // anything not an opt
      vector<string> expected; // currently both expected and required
   public:
      ArgParser(void);
      void usage1(string);
      void usage2(string);
      void ArgParse(int, char **);
      string GetTarget(string);
      void AddExpected(string);
      void AddExpected(vector <string>);
      void SetExpected(vector <string>);
} ArgParser;

#endif
