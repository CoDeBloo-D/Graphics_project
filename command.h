#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <fstream>
#define MAX_COMMAND_SEG 20
#define MAX_COMMAND_SEG_LEN 20
using namespace std;

class Command
{
public:
    int argc;
    char *argv[MAX_COMMAND_SEG];
    Command();
    ~Command();
    friend ifstream & operator >> (ifstream &input, Command &command);
};
ifstream & operator >> (ifstream &input, Command &command);

#endif // COMMAND_H
