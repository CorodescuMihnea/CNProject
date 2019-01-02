#ifndef COMMAND_H
#define COMMAND_H

typedef struct parsed_command {
    int cmdno;
    void * args;
}pcmd;

typedef struct cl_command {
    int cl_id;
    char * command;
}clcmd;

int parse_cmd(char *clcmd, pcmd *parsedcmd);
#endif