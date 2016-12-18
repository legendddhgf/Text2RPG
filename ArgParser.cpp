#include "ArgParser.h"

ArgParser::ArgParser () {
    opts = {};
    fields = {};
    expected = {};
}

void ArgParser::usage1(string unexpected) {
    fprintf(stderr, "%s: unexpected field\n", unexpected.c_str());
    fprintf(stderr, "Expected Fields:\n");
    for (string s : expected) {
        fprintf(stderr, "\t%s\n", s.c_str());
    }
    exit(1);
}

void ArgParser::usage2(string expectedstr) {
    fprintf(stderr, "%s: expected field but not found\n", expectedstr.c_str());
    fprintf(stderr, "Expected Fields:\n");
    for (string s : expected) {
        fprintf(stderr, "\t%s\n", s.c_str());
    }
    exit(1);
}

// call to parse arguments and set internal opts and fields
void ArgParser::ArgParse(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') opts.push_back(argv[i] + 1); // add to opts
        else { // add to fields (IE: field:target, field=target, field)
            string field, target = string("");
            char *location = strchr(argv[i], '=');
            if (location == nullptr) location = strchr(argv[i], ':');
            if (location != nullptr) {
                target = string(location + 1);
            }
            if (location != nullptr) *location = '\0';
            field = string(argv[i]);
            if (find(expected.begin(), expected.end(), field) == expected.end()) {
                usage1(field);
            }
            fields.insert(pair<string, string>(field, target));
        }
    }
    for (string check : expected) {
        if (fields.find(check) == fields.end()) {
            usage2(check);
        }
    }
}

string ArgParser::GetTarget(string field) {
    if (fields.find(field) == fields.end()) {
        fprintf(stderr, "%s: Not an existing field\n", field.c_str());
        exit(1);
    }
    return fields.at(field);
}

void ArgParser::AddExpected(string istring) {
    expected.push_back(istring);
}

void ArgParser::AddExpected(vector<string> ifields) {
    for (string istring : ifields) expected.push_back(istring);
}

void ArgParser::SetExpected(vector<string> ifields) {
    expected.clear();
    for (string istring : ifields) expected.push_back(istring);
}
