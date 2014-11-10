#include "search.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("INPUT INVALID! Please enter a VALID index.\n");
    return 1; //Error
  }
  char* searchMe = malloc(1024);
  struct newnode* list = buildWordList(argv[1]);
  while (1) { //run loop
    printf("Search> ");
    fgets(searchMe, 1024, stdin);
    searchMe[strlen(searchMe) - 1] = '\0';
    if (searchMe[0] == 'q'){
      puts("We got q"); //quit program
      break; //exit run loop
    }else if (searchMe[0] == 's' && searchMe[1] == 'a'){ //search and
        andSearch(list, searchMe); //search 'list and' for search input
    }else if(searchMe[0] == 's' && searchMe[1] == 'o'){ //search or
        orSearch(list, searchMe);  //search 'list or' for search input
    }
  }

//Free All Variables 
  struct newnode* release = list;

  while (release != NULL) {
    struct newnode* tmpW = release;
    struct filesnode* shackled = release->files;

    while (shackled != NULL) {
      struct filesnode* tmpF = shackled;
      shackled = shackled->next;
      free(tmpF->filesName);
      free(tmpF);
    }

    release = release->next;
    free(tmpW->word);
    free(tmpW);
  }
//End of Variable Free
  return 0;
}




struct newnode* buildWordList(const char* indexFile) {
  struct newnode* nextWord = NULL;
  struct newnode* curr = NULL;
  char* line;
  char** fileList = buildFiles(indexFile);
  char* word = NULL;
  FILE *file = fopen (indexFile, "r");
  int inList = 0;
  if (file != NULL){
    line = malloc(2048); 
  }
  while (fgets(line, 2048, file ) != NULL){
    if (strstr(line, "<list>")){
      inList = 1;
      word = malloc(strlen(line+6));
      strncpy(word, line+7, strlen(line+7) - 1);
      curr = malloc(sizeof(struct newnode) + 1);
      curr->word = word;
      curr->files = NULL;
      curr->next = nextWord;
      nextWord = curr;
    }else if (strstr(line, "</list>")){
      inList = 0;
      continue;
    }else if (inList){
      struct filesnode* currFile = malloc(sizeof(struct filesnode) + 1);
      int colonSpot = 0;
      while (colonSpot < strlen(line) && line[colonSpot] != ':'){ 
        colonSpot++; 
      }
      line[colonSpot] = '\0';
      strcpy(line, line+1);
      currFile->filesName = malloc(colonSpot + 1);
      strcpy(currFile->filesName, fileList[atoi(line)]);
      currFile->next = curr->files;
      curr->files = currFile;
    }
  }
  fclose(file);
  return nextWord;
}









char** buildFiles(const char* indexFile){
  int fileCount = countFiles(indexFile);
  char** fileList = malloc(sizeof(char*) * fileCount + 1);
  char* line;
  FILE *file = fopen (indexFile, "r");
  int inFile = 0;
  if (file != NULL) {
    line = malloc(2048); //Buffer where line is stored
  }
  while (fgets(line, 2048, file ) != NULL) {
    if (strstr(line, "<files>") != NULL){
      inFile = 1;
      continue;
    }
    else if (strstr(line, "</files>") != NULL){
      inFile = 0;
      break;
    }
    else if (inFile == 0) {
      continue;
    }
    else {
        fileCount--;
        fileList[fileCount] = malloc(strlen(line));
        strncpy(fileList[fileCount], line+2, strlen(line) - 2);
        //fileList[fileCount][strlen()]
    }
  }
  fclose(file);
  return fileList;
}











int countFiles(const char* indexFile){
  char* line;
  FILE *file = fopen (indexFile, "r");
  int count = 0;
  int inFile = 0;
  if (file != NULL) {
    line = malloc(1024);
  }
  while (fgets(line, 1024, file ) != NULL) {
    if (strstr(line, "<files>")){
      inFile = 1;
    }
    else if (strstr(line, "</files>")){
        break;
    }
    else if (inFile) {
      count += 1;
    }
  }
  return count;
}




void andSearch (struct newnode *head, char* line) {
   //puts(line);
   strncpy(line, line + 2, strlen(line));
   //puts(line);
   char* token;
   token = strtok(line, " ");
   int foundAll = 0;
   int foundWord;
   struct filesnode* fileHead = NULL;
   while (token != NULL) {
      struct newnode* ptr = head;
      foundWord = 0;
      while (ptr != NULL) {
         //printf("word: %s, token: %s",ptr->word,token);
         if (strcmp(ptr->word, token) == 0) {
            foundWord = 1;
            if (fileHead == NULL) {
               fileHead = malloc(sizeof(struct filesnode) + 1);
               struct filesnode* tmpPtr2;
               struct filesnode* tmpPtr;
               tmpPtr = ptr->files;
               tmpPtr2 = fileHead;
               tmpPtr2->next = NULL;
               tmpPtr2->filesName = tmpPtr->filesName;
               tmpPtr = tmpPtr->next;

               //tmpPtr2 = tmpPtr2->next;
               //tmpPtr = tmpPtr->next;
               //tmpPtr2 = tmpPtr2->next;
               while (tmpPtr != NULL) {
                  tmpPtr2->next = malloc(sizeof(struct filesnode) + 1);
                  tmpPtr2->next->filesName = tmpPtr->filesName;
                  tmpPtr2->next->next = NULL;
                  tmpPtr2 = tmpPtr2->next;
                  tmpPtr = tmpPtr->next;
               }
               //fileHead->next = NULL;
            }
            else {
               struct filesnode* tmpPtr = fileHead;
               struct filesnode* prev = tmpPtr;
               struct filesnode* tmpPtr2 = ptr->files;
               int found = 0;
               while (tmpPtr != NULL) {
                  while (tmpPtr2 != NULL) {
                     if (strcmp(tmpPtr->filesName,tmpPtr2->filesName) == 0){
                        found = 1;
                        break;
                     }
                     tmpPtr2 = tmpPtr2->next;
                  }
                  if (found == 0){
                     if (strcmp(prev->filesName,tmpPtr->filesName) == 0){
                        fileHead = fileHead->next;
                        free(prev);
                        tmpPtr = fileHead;
                        prev = tmpPtr;
                     }
                     else {
                        prev->next = tmpPtr->next;
                        free(tmpPtr);
                        tmpPtr = prev->next;
                     }
                  }
                  else {
                     prev = tmpPtr;
                     tmpPtr = tmpPtr->next;
                  }
                  found = 0;
               }
            }
         }
         ptr = ptr->next;
      }
      if (foundWord == 0) {
         foundAll = 1;
      }
      token = strtok(NULL, " ");
   }
   struct filesnode* tmp = fileHead;
   struct filesnode* freedom = tmp;
   if (foundAll != 1){
      while (tmp != NULL){
         printf("%s\n", tmp->filesName);
         freedom = tmp;
         tmp = tmp->next;
         free(freedom);
      }
   }
   else {
      while (tmp != NULL) {
         freedom = tmp;
         tmp = tmp->next;
         free (freedom);
      }
   }
}




void orSearch(struct newnode *head, char* line){
  strncpy(line, line + 2, strlen(line));
  char* token;
  token = strtok(line, " ");
  struct filesnode* fileHead = NULL;
  struct filesnode* tail = NULL;
  while (token != NULL) {
      struct newnode* ptr = head;
      while (ptr != NULL) {
         if (strcmp(ptr->word, token) == 0) {
            if (fileHead == NULL) {
               fileHead = malloc(sizeof(struct filesnode) + 1);
               struct filesnode* tmpPtr2;
               struct filesnode* tmpPtr;
               tmpPtr = ptr->files;
               tmpPtr2 = fileHead;
               tmpPtr2->next = NULL;
               tmpPtr2->filesName = tmpPtr->filesName;
               tail = tmpPtr2;
               tmpPtr = tmpPtr->next;

               //tmpPtr2 = tmpPtr2->next;
               //tmpPtr = tmpPtr->next;
               //tmpPtr2 = tmpPtr2->next;
               while (tmpPtr != NULL) {
                  tmpPtr2->next = malloc(sizeof(struct filesnode) + 1);
                  tmpPtr2->next->filesName = tmpPtr->filesName;
                  tmpPtr2->next->next = NULL;
                  tmpPtr2 = tmpPtr2->next;
                  tail = tmpPtr2;
                  tmpPtr = tmpPtr->next;
               }
            }
            else {
               struct filesnode* tmpPtr = fileHead;
               struct filesnode* prev = tmpPtr;
               struct filesnode* tmpPtr2 = ptr->files;
               int found = 0;
               while (tmpPtr2 != NULL) {
                  while (tmpPtr != NULL) {
                     if (strcmp(tmpPtr->filesName, tmpPtr2->filesName) == 0){
                        found = 1;
                        break;
                     }
                     tmpPtr = tmpPtr->next;
                  }
                  if (found == 0) {
                     tail->next = malloc(sizeof(struct filesnode) + 1);
                     tail->next->filesName = tmpPtr2->filesName;
                     tail->next->next = NULL;
                     tail = tail->next;
                  }
                  tmpPtr2 = tmpPtr2->next;
               }
            }
         }
         ptr = ptr->next;
      }
      token = strtok(NULL, " ");
  }
  struct filesnode* tmp = fileHead;
  struct filesnode* freedom = tmp;
  while (tmp != NULL) {
     printf("%s\n",tmp->filesName);
     freedom = tmp;
     tmp = tmp->next;
     free(freedom);
  }
}


