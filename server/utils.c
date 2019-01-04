#include "utils.h"
int parse_string(char *line, char ***argv) {

  char *buffer;
  int argc;

  buffer = (char *) malloc(strlen(line) * sizeof(char));
  strcpy(buffer,line);
  (*argv) = (char **) malloc(MAX_TOKS * sizeof(char**));

  argc = 0;  
  (*argv)[argc++] = strtok(buffer, DELIMITERS);
  while ((((*argv)[argc] = strtok(NULL, DELIMITERS)) != NULL) &&
	 (argc < MAX_TOKS)) ++argc;

  return argc;
}