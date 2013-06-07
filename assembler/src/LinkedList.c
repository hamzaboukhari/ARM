#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "LinkedList.h"


void *allocElem(void){
  node_t *res = malloc(sizeof(node_t));
  if(res == NULL){
   perror("allocElem fail \n");
   exit(EXIT_FAILURE);
  }
 return res;
}

void freeElem(node_t *elem){
 free(elem -> key);
 free(elem); 
}

void init(table_t *t){
 t -> head = allocElem();
 t -> foot = allocElem();
 t -> head -> prev = NULL;
 t -> foot -> next = NULL;
 t -> head -> next = t -> foot;
 t -> foot -> prev = t -> head;
}

iterator start(table_t *t){
 return t->head->next;
}

iterator end(table_t *t){
 return t->foot;
}

iterator next(iterator i){
 return i -> next;
}

char *getkey(iterator i){
 return i->key;
}

void insert(table_t *t,iterator i, char *k, int v,int type){
 node_t *new = allocElem();
 new -> key  = malloc(strlen(k) * sizeof(char));
 strcpy(new->key,k);
 new -> value = v;
 new -> type = type;

 new -> prev = i -> prev;
 new -> next = i;
 i -> prev -> next = new;
 i -> prev = new;
}


void insertInFront(table_t *t, char *k, int v,int type){
 insert(t,start(t),k,v,type);
}

void insertInEnd(table_t *t, char *k, int v,int type){
 insert(t,end(t),k,v,type);
}

void insertExpression(table_t *t,char *k ,int val,int loc){
 insert(t,end(t),k,val,loc);
}

//Only to be used when making a symbol table NOT any other;
void insertElements(table_t *t,char keys[24][100],int values[24], int types[24]){
 for(int i=0; i<24; i++){
  insertInEnd(t,keys[i],values[i],types[i]);
 }
 printf("Insertion Complete ! \n");
}

int getValue(table_t *t, char *k){
 iterator i = start(t);
 while(i != t->foot){
  if(strcmp(k,i->key) == 0){
   return i->value;
  }
  i = next(i);
 }
 perror("Value Not Found !! \n");
 return -1;
}

int getType(table_t *t, char *k){
 //printf("Getting type \n");
 iterator i = start(t);
 while(i != t->foot){
  if(strcmp(k,i->key) == 0){
   return i->type;
   printf("Got type \n");
  }
  i = next(i);
 }
 //printf("***Type [%s] Not Found !! \n",k);
 return -1;
}


void freeTable(table_t *t){
 iterator elem = start(t);
 elem = next(elem); 
 while(elem != t -> foot){
  freeElem(elem -> prev); 
 }
 freeElem(elem);
}

void printList(table_t *t){
 iterator elem = start(t);
 while(elem != t->foot){
  printf("Key: %s \n",elem -> key);
  printf("Value: %d \n", elem -> value);
  printf("Type: %d \n\n", elem -> type);
  elem = next(elem);
 }
}
