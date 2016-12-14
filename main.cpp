#include "common.h"
#include "ArgParser.h"

int main (int argc, char **argv) {

   ArgParser *parser = new ArgParser();
   parser->SetExpected({string("infile"), string("config")});
   parser->ArgParse(argc, argv);
   // open files
   string infile = parser->GetTarget(string("infile"));
   FILE *fp = fopen(infile.c_str(), "r");
   if (fp == nullptr) {
      fprintf(stderr, "%s: File not found\n", infile.c_str());
      exit(1);
   }

   vector <string> list_rooms = {};
   char buf[1024];
   while (fgets(buf, 1023, fp)) {
      buf[strlen(buf) - 1] = '\0';
      if (buf[0] == 'r') list_rooms.push_back(string(buf + 2));
   }
   int count = 1;
   for (string n : list_rooms) fprintf(stdout, "%d: %s\n", count++, n.c_str());
   fclose(fp);
   
   return 0;
}
