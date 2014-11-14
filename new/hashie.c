#include <stdio.h>   /* gets */
#include <stdlib.h>  /* atoi, malloc */
#include <string.h>  /* strcpy */
#include <limits.h>
#include <math.h>
#include "uthash.h"

struct my_struct {
    int id;                    // key will be word
    char name[100000000000000];
    UT_hash_handle hh;         /* makes this structure hashable */
};

struct my_struct *words = NULL;

void add_word(int word_id, char *name) {
    struct my_struct *s;

    HASH_FIND_INT(words, &word_id, s);  /* id already in the hash? */
    if (s==NULL) {
      s = (struct my_struct*)malloc(sizeof(struct my_struct));
      s->id = word_id;
      HASH_ADD_INT( words, id, s );  /* id: name of key field */
    }
    strcpy(s->name, name);
}

struct my_struct *find_word(int word_id) {
    struct my_struct *s;

    HASH_FIND_INT( words, &word_id, s );  /* s: output pointer */
    return s;
}

void delete_word(struct my_struct *word) {
    HASH_DEL( words, word);  /* word: pointer to deletee */
    free(word);
}

void delete_all() {
  struct my_struct *current_word, *tmp;

  HASH_ITER(hh, words, current_word, tmp) {
    HASH_DEL(words,current_word);  /* delete it (words advances to next) */
    free(current_word);            /* free it */
  }
}

void print_words() {
    struct my_struct *s;

    for(s=words; s != NULL; s=(struct my_struct*)(s->hh.next)) {
        printf("word id %d: name %s\n", s->id, s->name);
    }
}

int name_sort(struct my_struct *a, struct my_struct *b) {
    return strcmp(a->name,b->name);
}

int id_sort(struct my_struct *a, struct my_struct *b) {
    return (a->id - b->id);
}

void sort_by_name() {
    HASH_SORT(words, name_sort);
}

void sort_by_id() {
    HASH_SORT(words, id_sort);
}




char *scat(char *,char *);
char *scat(char *s,char *t)
{
    char *p=malloc(strlen(s)+strlen(t)+1);    /* 3: you will have to reserve memory to hold the copy. */
    int ptr =0, temp = 0;                   /* 4 initialise some helpers */

    while(s[temp]!='\0'){                  /* 5. use the temp to "walk" over string 1 */
        p[ptr++] = s[temp++];
    }
    temp=0;
    while(t[temp]!='\0'){                   /* and string two */
        p[ptr++]=t[temp++];
    }
    return p;
}
char* chartobin ( unsigned char c )
{
    static char bin[CHAR_BIT + 1] = {0};
    int i;
 
    for ( i = CHAR_BIT - 1; i >= 0; i-- ){
        bin[i] = (c % 2) + '0';
        c /= 2;
    }
 
    return bin;
}


char* bintochar(char* c){
    char string[strlen(c)];
    int x;
    int result=0;
    char *splitbinaries[strlen(c)/8];
    char b;
    char* output="";
    int asciivalue=0;
    char *asciiletter="";
    char *addin;
    char holder;

    for(int i=0;i<strlen(c)/8;i++){
        asciivalue=0;
        splitbinaries[i]="";
        x=(i*8);
        if(i==0){ 
            while(x<8){
                b=c[x];
                x++;
                splitbinaries[i]=scat(splitbinaries[i],&b);
            }
        }
        else if(i!=0){
            while(x<(((i+1)*8))){
                b=c[x];
                x++;
                splitbinaries[i]=scat(splitbinaries[i],&b);
            }
        }
        //printf("string is %s\n",splitbinaries[i]);
        char *results=splitbinaries[i];
        //printf("%s\n",results);
        int result=atoi(results);
        //printf("%d\n",result);
        int counter=0;
        while (result > 0){
            asciivalue=asciivalue+(pow(2.00,counter)*(result%10));
            counter++;
            result=result/10;
        }
        //printf("%d\n",asciivalue);
        holder=(char) asciivalue;
        asciiletter=&holder;
        output=scat(output,asciiletter);
        //printf("%s\n",output);
    }
    return output;

}
/*
char* chartobinary(char c){
    char* output="";
    int binary;
    char* nummy="";
    binary=c; 
    printf("letter %c:\n",c);
    while(c){
        if(binary&1) 
             nummy="1";
            //printf("1"); 
        else 
             nummy="0";
            //printf("0"); 
        output=scat(output,nummy);
        binary>>=1; 
    }
    return output;

}*/
/*
char* concat(char *s1, char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
void customstrcat(char* dest, const char* source) {
  while(*dest != '\0') // finding the end of the string 
  {
    dest++;
  }

  *dest = *source;
  while(*dest != '\0')
  {
    dest++;
    source++;
    *dest = *source;
  }
}
*/



int main(int argc, char *argv[]) {
    char in[10]; //maxlength
    int id=1, running=1;
    long int binarykey;
    struct my_struct *s;
    unsigned num_words;
    char newword[]="test";

    char* holder="";
    for(int i=0;i<strlen(newword);i++){
        holder=scat(holder,chartobin(newword[i]));
        //printf("string is %s\n",holder);
    }
    char * pEnd;
    printf("%s\n",holder);
    //binarykey=atol(holder);
    binarykey = strtol (holder,&pEnd,10);
    printf("%ld\n",binarykey);

    bintochar(holder);


    char *filename="file";


    add_word(id++, filename);
    sort_by_id();
    print_words();
    running=0;
    delete_all();  /* free any structures */
    return 0;
}
/*
int main(int argc, char *argv[]) {
    char in[10]; //maxlength
    int id=1, running=1;
    struct my_struct *s;
    unsigned num_words;

    while (running) {
        printf(" 1. add word\n");
        printf(" 2. add/rename word by id\n");
        printf(" 3. find word\n");
        printf(" 4. delete word\n");
        printf(" 5. delete all words\n");
        printf(" 6. sort items by name\n");
        printf(" 7. sort items by id\n");
        printf(" 8. print words\n");
        printf(" 9. count words\n");
        printf("10. quit\n");
        gets(in);
        switch(atoi(in)) {
            case 1:
                printf("name?\n");
                dd_word(id++, gets(in));a
                break;
            case 2:
                printf("id?\n");
                gets(in); id = atoi(in);
                printf("name?\n");
                add_word(id, gets(in));
                break;
            case 3:
                printf("id?\n");
                s = find_word(atoi(gets(in)));
                printf("word: %s\n", s ? s->name : "unknown");
                break;
            case 4:
                printf("id?\n");
                s = find_word(atoi(gets(in)));
                if (s) delete_word(s);
                else printf("id unknown\n");
                break;
            case 5:
                delete_all();
                break;
            case 6:
                sort_by_name();
                break;
            case 7:
                sort_by_id();
                break;
            case 8:
                print_words();
                break;
            case 9:
                num_words=HASH_COUNT(words);
                printf("there are %u words\n", num_words);
                break;
            case 10:
                running=0;
                break;
        }
    }

    delete_all();  // free any structures
    return 0;
}*/