#include "command.h"
#include "assert.h"
#include <QDebug>

Command::Command() {
    argc = 0;
}

Command::~Command(){
    for (int i = 0; i < argc; i++)
        delete[] argv[i];
}

ifstream & operator >> (ifstream &input, Command &command){
    if (command.argc > 0){
        for (int i = 0; i < command.argc; i++){
            delete[] command.argv[i];
        }
        command.argc = 0;
    }

    assert(command.argc == 0);

    char tmp[MAX_COMMAND_SEG * MAX_COMMAND_SEG_LEN], *p;
    const char *dlim = ",. ";
    input.getline(tmp, MAX_COMMAND_SEG * MAX_COMMAND_SEG_LEN);
    p = strtok(tmp, dlim);
    int i = 0;
    while (p != nullptr){
        command.argv[i] = new char[MAX_COMMAND_SEG_LEN];
        strcpy(command.argv[i++], p);
        p = strtok(nullptr, dlim);
    }
    if(!strcmp(command.argv[0], "drawPolygon") || !strcmp(command.argv[0], "drawCurve")) {
        char tmp2[MAX_COMMAND_SEG * MAX_COMMAND_SEG_LEN], *q;
        input.getline(tmp2, MAX_COMMAND_SEG * MAX_COMMAND_SEG_LEN);
        q = strtok(tmp2, dlim);
        while (q != nullptr){
            command.argv[i] = new char[MAX_COMMAND_SEG_LEN];
            strcpy(command.argv[i++], q);
            q = strtok(nullptr, dlim);
        }
    }
    command.argc = i;
    return input;
}
