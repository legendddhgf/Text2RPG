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

   delete parser;
   fclose(fp);
   return 0;
}
