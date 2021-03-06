#include "utils.h"
#include "command_queue.h"

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

void parse_command(char *client_req, cmd *c) {
	char **split_client_req;
	int nof_strings;
	nof_strings = parse_string(client_req, &split_client_req);

	if(strncmp(split_client_req[0], "delay", strlen("delay")) == 0) {
		c->cmd_no = 0;
	}
	else if(strncmp(split_client_req[0], "arrivals", strlen("arrivals")) == 0) {
		c->cmd_no = 1;
	}
	else if(strncmp(split_client_req[0], "departures", strlen("departures")) == 0) {
		c->cmd_no = 2;
	}
	else {
		c->cmd_no = -1;
	}

	int it;
	c->args = malloc(nof_strings -1 );

	for(it=1; it < nof_strings; it++) {
		c->args[it] = malloc(sizeof(split_client_req[it]));
		strcpy(c->args[it], split_client_req[it]);
	}

}
