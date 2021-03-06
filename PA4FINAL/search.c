#include <stdio.h>   /* gets */
#include <stdlib.h>  /* atoi, malloc */
#include <string.h>  /* strcpy */
#include <limits.h>
#include <sys/stat.h>
#include <math.h>
#include "sorted-list.h"
#include "tokenizer.h"
#include "uthash.h"
#include "util.h"

#define KGRN  "\x1B[32m"
#define KNRM  "\x1B[0m"

enum cases{sa,so,q};



struct my_struct {
	int id;
	char *word;
	SortedList *filenames;
	UT_hash_handle hh;
};

struct my_struct *words = NULL;

void destroyAllStrings(void *p);
int hashfix(unsigned char *str);
//////////////////////////////////////////////////////////////////////////////
//use add_word(word, filename); to add to hashie./////////////////////////////
void add_word(char *word, char *filename);////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//use struct my_struct *info = find_word(token); if info is null free it and tkdestroy it, you are done
//SortedListIterator *iter = SLCreateIterator(info->filenames);//////////////////////////////////////////
SortedListPtr find_word(char *word);////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void delete_all_words(void);
void print_all_words(void);
void print_list(SortedList *list);
void fill_hash(char * f_name);





SortedListPtr merge_lists(SortedListPtr list_a, SortedListPtr list_b){  //this merges 2 sorted lists into 1 sorted list

	SortedListPtr final=SLCreate(list_a->compare_func,list_a->destroy_func);
	
	
	if(list_a!=NULL){
		NodePtr a_head=list_a->front;
		
		while (a_head!=NULL) {
			char *alpha=a_head->data;
			
			int len=strlen(alpha);	
			char *for_hash=malloc(sizeof(char)*len);
			strcpy(for_hash,alpha);
			SLInsert(final,for_hash);
				
			a_head= a_head->next;
		}
	}
	
	if(list_a!=NULL){
		NodePtr b_head=list_b->front;
		
		while (b_head!=NULL) {
			char *alpha=b_head->data;
			
			int len=strlen(alpha);	
			char *for_hash=malloc(sizeof(char)*len);
			strcpy(for_hash,alpha);
			SLInsert(final,for_hash);
				
			b_head= b_head->next;
		}
	}
		
	return final;


}


SortedListPtr OR_funct(SortedListPtr given){  //this eliminates repeats from a giant sortedlist which contains filenames for everything
	
	SortedListPtr final=SLCreate(given->compare_func,given->destroy_func);
	
	NodePtr g_head=given->front;
	
	while (g_head!=NULL) {
		char *alpha=g_head->data;
		
		int len=strlen(alpha);	
		char *for_hash=malloc(sizeof(char)*len);
		strcpy(for_hash,alpha);
		SLInsert(final,for_hash);
			
		g_head= g_head->next;
    }
	
		NodePtr now= final->front;
		NodePtr nex=now->next;
		
		while(nex!=NULL){
			void* nowData = now->data;
			void* nexData = nex->data;
			int compare = final->compare_func(nowData, nexData);

			if(compare==0){
				SLRemove(final,nowData);
			}
			
			now=nex;
			nex=nex->next;
		
		}
		

	return final;

}



SortedListPtr AND_funct(SortedListPtr given,int count){
	
	SortedListPtr final=SLCreate(given->compare_func,given->destroy_func);
	
	
	NodePtr g_head=given->front;
	
	while (g_head!=NULL) {
		char *alpha=g_head->data;
		
		int len=strlen(alpha);	
		char *for_hash=malloc(sizeof(char)*len);
		strcpy(for_hash,alpha);
		SLInsert(final,for_hash);
			
		g_head= g_head->next;
    }
	
		NodePtr now= final->front;
		
		int check=0;
		
		while(now->next!=NULL){
			void* nowData = now->data;
			void* nexData = now->next->data;
			
			int compare = final->compare_func(nowData, nexData);
			//printf("checking %s vs %s and count is %d \n",(char*)nowData,(char*)nexData,check);  //ERROR CHECKING

			if(compare!=0){
				check=0;
				
			}if(compare==0){
				check++;
				if(check==count-1){
						printf("%s \n",(char*)nowData);
				}
			}
			
			now=now->next;
		}
		

	return final;
	
	
	
	
}




void destroyAllStrings(void *p) {
	char *string = (char *)p;
	free(string);
}

int hashfix(unsigned char *str){
	int c; 
	int hash = 5381;
	while ((c = *str++)){
		hash = ((hash << 5) + hash) + c;
 	}
	return (hash < 0) ? -hash : hash;
}

void add_word(char *word, char *filename){
	struct my_struct *r;
	int my_struct_id = hashfix((unsigned char *) word);
	HASH_FIND_INT(words, &my_struct_id, r);
	//stop collisions
	while (r != NULL) {
	//found word
		if (strcmp((*r).word, word) == 0) {
			break;
    		}
		my_struct_id++;
		HASH_FIND_INT(words, &my_struct_id, r);
  	}
  	if (r == NULL) {
		//if empty spot is found then add to it
		r = (struct my_struct *)malloc(sizeof(struct my_struct));
		(*r).id = my_struct_id;
		(*r).word = word;
		(*r).filenames = SLCreate(compareStrings, destroyAllStrings);
		SLInsert((*r).filenames, filename);
		HASH_ADD_INT(words, id, r);
  	} else {
    		SLInsert((*r).filenames, filename);
  	}
	printf("print filenames\n");
	print_list(r->filenames);
	printf("End of print filenames\n");

}
SortedListPtr find_word(char *word){
	
	
	struct my_struct *r;
	printf("word is %s\n",word);
	int my_struct_id = hashfix((unsigned char *) word);
	printf("%d \n",my_struct_id);
	HASH_FIND_INT(words, &my_struct_id, r);
	//stop collisions
	puts("declaring");
	
	if(r==NULL){
		puts("r is null");
	}
	
	while (r != NULL) {
		
		puts("inside while loop");
		//word has been found
    		if (strcmp((*r).word, word) == 0) {
      			break;
    		}
    		my_struct_id++;
    		HASH_FIND_INT(words, &my_struct_id, r);
  	}
  	return r->filenames;
}

void delete_all_words(void){//delete and free all the words
	struct my_struct *current_word, *tmp;
	HASH_ITER(hh, words, current_word, tmp) {
		HASH_DEL(words, current_word);
		free((*current_word).word);
		SLDestroy((*current_word).filenames);
		free(current_word);
	}
}




void print_all_words(void){
	struct my_struct *s, *tmp;
	HASH_ITER(hh, words, s, tmp) {
    		SortedListIterator *iter = SLCreateIterator((*s).filenames);
    		char *file;
    		while((file = (char *)SLNextItem(iter)) != NULL) {
      			printf("word %s \t id %d \t file %s\n", (*s).word, (*s).id, file);
    		}
    		SLDestroyIterator(iter);
  	}
  	unsigned int num_words;
  	num_words = HASH_COUNT(words);
  	printf("number of informations %u\n", num_words);
}



void print_list(SortedList *list){
	
	if (list != NULL) {
		SortedListIterator *iter = SLCreateIterator(list);
		char *item;
		while((item = (char *)SLNextItem(iter)) != NULL) {
			printf("%s\n", item);
		}
		SLDestroyIterator(iter);
	}else{
		return;
	}
}



void fill_hash(char * f_name){
	  FILE *fp;
	  char str[60];
	  int count=0;
	  
	  int todo=0;
	  
	  char dead[200];
	  strcpy(dead,"\n");

	 /* opening file for reading */

	 fp = fopen(f_name , "r");

	 if(fp == NULL) {
	   perror("Error opening file");
	   return;
	 }
	 char *keyf=NULL;
	 while( fgets (str, 1000000, fp)!=NULL ) {
		 
		
		if((strcmp(str,dead))==0){
			continue;
			puts("dead space");
		}
		
		printf("%d time: --------\n",count);
	   
		
		char *td_str=strtok(str," ");
		puts("print tokens: ");
		
		while(td_str!=NULL){
			
			if((strcmp(td_str,"<list>")==0)){
				puts("IT BEGINS AGAIN");
				todo=0;
			
			
			}else if((strchr(td_str,'/'))!=NULL && todo==2){ //this is not working
				puts("CONTINUING..TO DO IS 3"); //this should not be included cause its an end!
				todo=3;
				
			
			
			}else if((strchr(td_str,'\n'))!=NULL && todo==1){ //this is not working
				puts("CONTINUING"); //this should not be included cause its an end!
				//continue;
			
			
			}else if(todo==0){
				puts("THIS IS KEY");
				int num=strlen(td_str);
				keyf=(char*)malloc(sizeof(char)*num);
				strcpy(keyf,td_str);
				puts("key is:");
				puts(keyf);
				puts("");
				
				todo=1;
			
			}else if(todo==1){
				puts("THIS IS A FILE NAME");		
				int num=strlen(td_str);
				char* file=(char*)malloc(sizeof(char)*num);
				strcpy(file,td_str);
				
				printf("Word: %s\n",keyf);
				printf("file loc: %s\n",td_str);
				add_word(keyf,file);
				
				todo=2;
			
			}else if(todo==2){
				puts("THIS IS A NUMBER");
				todo=1;
			}
			
			printf("%s \n",td_str);
			td_str=strtok(NULL," ");
		}
			
		//count++;
	   
	   //puts(str);
	 }
	 
	 
	 print_all_words();
	 fclose(fp);
	
}

char *replace_str(const char *str, const char *old, const char *new)
{
	char *ret, *r;
	const char *p, *q;
	size_t oldlen = strlen(old);
	size_t count, retlen, newlen = strlen(new);
	int samesize = (oldlen == newlen);

	if (!samesize) {
		for (count = 0, p = str; (q = strstr(p, old)) != NULL; p = q + oldlen)
			count++;
		/* This is undefined if p - str > PTRDIFF_MAX */
		retlen = p - str + strlen(p) + count * (newlen - oldlen);
	} else
		retlen = strlen(str);

	if ((ret = malloc(retlen + 1)) == NULL)
		return NULL;

	r = ret, p = str;
	while (1) {
		/* If the old and new strings are different lengths - in other
		 * words we have already iterated through with strstr above,
		 * and thus we know how many times we need to call it - then we
		 * can avoid the final (potentially lengthy) call to strstr,
		 * which we already know is going to return NULL, by
		 * decrementing and checking count.
		 */
		if (!samesize && !count--)
			break;
		/* Otherwise i.e. when the old and new strings are the same
		 * length, and we don't know how many times to call strstr,
		 * we must check for a NULL return here (we check it in any
		 * event, to avoid further conditions, and because there's
		 * no harm done with the check even when the old and new
		 * strings are different lengths).
		 */
		if ((q = strstr(p, old)) == NULL)
			break;
		/* This is undefined if q - p > PTRDIFF_MAX */
		ptrdiff_t l = q - p;
		memcpy(r, p, l);
		r += l;
		memcpy(r, new, newlen);
		r += newlen;
		p = q + oldlen;
	}
	strcpy(r, p);

	return ret;
}

int main(int argc,char *argv[]){
	
	char *f_name;
	
	if(argc==2){
		f_name=argv[1]; //fname is fill_hash
		printf("fname is: %s \n",f_name);
		fill_hash(f_name);
	}
	
	int b_read;
	size_t n_bytes=200;
	char *in_string;
	in_string=(char *)malloc(n_bytes +1);
	int cmp=1;
	
	char quit[200]; //variable used to compare input if it should quit
	strcpy(quit,"q");
	
	char menu[200];
	strcpy(menu,"menu");
	
	char smile[200];
	strcpy(smile,"smile");

	char frown[200];
	strcpy(frown,"frown");
	
	char option[256];
	
	int correct=0;

	
	
	
	puts("option choices are:(sa <list of items>,so <list of items>,q,menu,smile)");


	while(cmp!=10){
		//puts("enter an option:");
		
		b_read= getline(&in_string,&n_bytes,stdin);
	
		//puts("you typed: ");
		//puts(in_string);
		//printf("bytes read: %d \n",b_read);
		
		//checking for sa option-
		strcpy(option,"sa ");
		if(memcmp(in_string,option,3)==0){
			//puts("we are in option 1");
			
			char *td_str=strtok(in_string," ");
			td_str+=3;
			SortedListPtr ANDList=SLCreate(compareStrings,destroyAllStrings);
			
			int ct=0;
			while(td_str!=NULL){
				printf("%s \n",td_str);
				
				int s_num=strlen(td_str);
				char *now= malloc(sizeof(char)*s_num);
				strcpy(now,td_str);
				
				SortedListPtr r=(SortedListPtr)malloc(sizeof(SortedListPtr));
				r=find_word(now);
				
				merge_lists(ANDList,r);
				
				ct++;  
				//printf("ct is %d",ct);

				td_str=strtok(NULL," ");
			}
			
			AND_funct(ANDList,ct); //this has to be one less than the given inputs
			
			correct=1;
		}
		
		//checking for so option-
		strcpy(option,"so ");
		if((memcmp(in_string,option,3))==0){
			puts("we are in option 2");
			char *td_str=strtok(in_string," ");
			td_str+=3;
			//char str[] = *td_str;
			//printf ("%s\n",str);
			
			//SortedListPtr list_OR=SLCreate(compareStrings,destroyAllStrings);
			//SortedListPtr merged= merge_lists(list_OR,merged);
			//td_str=strtok(NULL,"so ");
			td_str=strtok(NULL," ");
			SortedListPtr now_SL=SLCreate(compareStrings,destroyAllStrings);
			while(td_str!=NULL){
				puts("1");
				int s_num=strlen(td_str);
				char *now= malloc(sizeof(char)*s_num);
				strcpy(now,td_str);
				
				puts("2");
				SortedListPtr r=(SortedListPtr)malloc(sizeof(SortedListPtr));
				r=find_word(now);
				
				//now_SL=r;
				print_list(r);
				
				
				puts("3");
				//merged=merge_lists(merged,r->filenames);
				
				//print_list(list_OR);
				
				puts("4");
				printf("%s \n",td_str);
				printf("td_str is: %s\n",td_str);
				td_str=strtok(NULL," ");
				//printf("td_str is: %s\n",td_str);
				//printf("td_str len is: %zu\n",strlen(td_str));
				if(!*td_str) break;
				
			}puts("out");
			
			//SortedListPtr or_answer =OR_funct(merged);
			//SLDestroy(or_answer);
			//SLDestroy(list_OR);
			correct=1;
		}
		
		//checking for menu option- 
		cmp=strcmp(in_string,menu);
		
		if(cmp==10){
			puts("WELCOME TO THE MENU");
			puts("1. SYNTAX:        sa <list of items>");
			puts("          the sa option searches for documents which contain all of the items which are listed in the list");
			puts("          sa can be considered as a logical AND for word searches");
			puts(" ");
			puts("2. SYNTAX:        so <list of items>");
			puts("          the so option searches for documents which contain any of the items which are listed in the list");
			puts("          so can be considered as a logical OR for word searches");
			puts(" ");
			puts("3. SYNTAX:        smile");
			puts("          this option lets you smile");
			puts(" ");
			puts("4. SYNTAX:        frown");
			puts("          hopefully, this option won't make you sad.. and give us extra credit instead");
			puts(" ");
			puts("5. SYNTAX:        q");
			puts("          this option lets you gracefully exit this program");
			puts(" ");
					
			cmp=9;		 	
		}
		
		
		//checking for smile option
		cmp=strcmp(in_string,smile);
		
		if(cmp==10){
	puts("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
	puts("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
	puts("@@@@@     @@@@@@@@@@@@@@@@@@@@@@@@@     @@@@@");
	puts("@@@@@     @@@@@@@@@@@@@@@@@@@@@@@@@     @@@@@");
	puts("@@@@@     @@@@@@@@@@@@@@@@@@@@@@@@@     @@@@@");
	puts("@@@@@     @@@@@@@@@@@@@@@@@@@@@@@@@     @@@@@");
	puts("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
	puts("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
	puts("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
	puts("@@@@@   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@@");
	puts("@@@@@@   @@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@@@");
	puts("@@@@@@@   @@@@@@@@@@@@@@@@@@@@@@@@@   @@@@@@@");
	puts("@@@@@@@@   @@@@@@@@@@@@@@@@@@@@@@@   @@@@@@@@");
	puts("@@@@@@@@@                           @@@@@@@@@");
	puts("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
	printf("%s@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n", KGRN);
    printf("%s@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n", KNRM);
					
			cmp=9;		 	
		}		
				//checking for smile option

		cmp=strcmp(in_string,frown);
		
		if(cmp==10){
	puts("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
	puts("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
	puts("@@@@@     @@@@@@@@@@@@@@@@@@@@@@@@@     @@@@@");
	puts("@@@@@     @@@@@@@@@@@@@@@@@@@@@@@@@     @@@@@");
	puts("@@@@@     @@@@@@@@@@@@@@@@@@@@@@@@@     @@@@@");
	puts("@@@@@     @@@@@@@@@@@@@@@@@@@@@@@@@     @@@@@");
	puts("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
	puts("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
	puts("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");	
	puts("@@@@@@@@@                           @@@@@@@@@");
	puts("@@@@@@@@   @@@@@@@@@@@@@@@@@@@@@@@   @@@@@@@@");
	puts("@@@@@@@   @@@@@@@@@@@@@@@@@@@@@@@@@   @@@@@@@");
	puts("@@@@@@   @@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@@@");
	puts("@@@@@   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@@");
	puts("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
	printf("%s@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n", KGRN);
    printf("%s@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n", KNRM);
					
			cmp=9;		 	
		}		
		
	
		
		//checking for q option
		cmp=strcmp(in_string,quit);
	
	}
	

	


	return 0;

}


/*
int main(int argc, char *argv[]) {
		//fill_hash("index3");
	
	
	 
	// fill_hash("index3");
	
	 
	 Node a;
	 
	 a.data= "hello.txt";
	 a.next=NULL;
	 
	 NodePtr a_ptr= &a;
	 puts(a_ptr->data);
	 
	 
	
	//messing around with lists here-----
	// sorted list A----
	SortedListPtr lista=SLCreate(compareStrings,destroyAllStrings);
	
	if(lista==NULL){
		puts("shit didnt work");
	}
	
	char *alpha="aa";
	
	int len=strlen(alpha);	
	char *for_hash=malloc(sizeof(char)*len);
	strcpy(for_hash,alpha);
	SLInsert(lista,for_hash);
	
	alpha="aa";
	len=strlen(alpha);
	for_hash=malloc(sizeof(char)*len);
	strcpy(for_hash,alpha);
	SLInsert(lista,for_hash);
	
	puts("list a: ");
	print_list(lista);
	puts("");
	
	int size_a=lista->size;
	printf("list a size: %d \n",size_a);
	
	//--------
	
	
	SortedListPtr listb=SLCreate(compareStrings,destroyAllStrings);
	
	char *beta="aa";
	
	len=strlen(beta);	
	for_hash=malloc(sizeof(char)*len);
	strcpy(for_hash,beta);
	SLInsert(listb,for_hash);
	
	beta="aa";
	len=strlen(beta);	
	for_hash=malloc(sizeof(char)*len);
	strcpy(for_hash,beta);
	SLInsert(listb,for_hash);
		
	puts("list b: ");
	print_list(listb);
	puts("");
	
	//--3rd list
	SortedListPtr listc=SLCreate(compareStrings,destroyAllStrings);
	
	beta="aa";
	
	len=strlen(beta);	
	for_hash=malloc(sizeof(char)*len);
	strcpy(for_hash,beta);
	SLInsert(listc,for_hash);
	
	beta="aa";
	len=strlen(beta);	
	for_hash=malloc(sizeof(char)*len);
	strcpy(for_hash,beta);
	SLInsert(listc,for_hash);
	puts("list c: ");
	print_list(listc);
	puts("");
	
	//-----
		
	SortedListPtr merged=merge_lists(lista,listb);
	merged=merge_lists(merged,listc);
	print_list(merged);
	puts("");
	
	puts("print OR test:");
	
	SortedListPtr or_test=OR_funct(merged);
	print_list(or_test);

	
	SLDestroy(or_test);
	SLDestroy(merged);
	SLDestroy(lista);
	SLDestroy(listb); //this deletes both lista and listb since listb is just a pointer to lista 


	puts("it went well!");


	//return(0);





	fill_hash("index3");
	
	char *alpha="zumba";
	char *filename="hello.txt";
	add_word(alpha, filename);
	char *alpha2="zumba";
	char *filename2="hello2.txt";
	add_word(alpha2, filename2);
**

	
	struct my_struct *r= malloc(sizeof(struct my_struct));
	r = find_word("hello");
	
	
	printf("test2\n");
	
	//printf("ID : %d\n",(*r).id);
	//printf("Word: %s\n",(*r).word);
	//print_list(r->filenames);
	print_all_words();
	print_all_words();
	return 0;
}*/
