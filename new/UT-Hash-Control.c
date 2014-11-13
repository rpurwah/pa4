#include <stdio.h>   /* gets */
#include <stdlib.h>  /* atoi, malloc */
#include <string.h>  /* strcpy */
#include "uthash.h"

struct my_struct {
    int id;                    // key will be word
    char name[10];
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

int main(int argc, char *argv[]) {
    char in[10];
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
                add_word(id++, gets(in));
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

    delete_all();  /* free any structures */
    return 0;
}