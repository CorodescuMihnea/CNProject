#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_STRING_LENGTH 256
#define MAX_TOKS 100
#define DELIMITERS " \t"

/* Splits string into an array of strings, return value is number of strings*/
/* Example usage:
int main() {

  char S[MAX_STRING_LENGTH];

  char **A;
  int  n,i;

  gets(S);

  n = parse_string(S, &A);  

  printf("strings read:\t%d\n",n);
  
  for (i = 0; i < n; ++i)
    printf("A[%d] = %s\n",i,A[i]);

}*/
int parse_string(char* line, char*** argv);

#endif