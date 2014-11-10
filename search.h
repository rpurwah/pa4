#ifndef SEARCH_H
#define SEARCH_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct filesnode {
  char* filesName;
  struct filesnode* next;
};

struct newnode {
  char* word;
  struct newnode* next;
  struct filesnode* files;
};

char** buildFiles(const char*);
struct newnode* buildWordList(const char*);
void andSearch(struct newnode*, char*);
void orSearch(struct newnode*, char*);


#endif