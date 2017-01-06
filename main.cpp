#include "common.h"
#include "ArgParser.h"
#include "GameEngine.h"

// note that the std libs hold memory on exit so one block of still reachable
// memory is expected
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
  GameEngine *game = new GameEngine(fp);

  // end
  delete game;
  delete parser;
  fclose(fp);
  return 0;
}
