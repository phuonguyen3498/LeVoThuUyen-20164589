#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

typedef struct wordList{
    char word[20];
    int count;
    int lineAt[10];
    struct wordList *next;
} wordList;


wordList *head = NULL;
wordList *current = NULL;

wordList *createNode(char a[10], int c, int l){
    wordList *temp;
    temp = (wordList*) malloc(sizeof(wordList));
    temp->next = NULL;
    strcpy(temp->word,a);
    temp->count = c;
    temp->lineAt[0] = l;
    return temp;
}

typedef struct sWord {
    char stop[10];
    struct sWord *next;
}sWord;

sWord *first = NULL;
sWord *now = NULL;

sWord *createStopNode(char b[10]){
    sWord *temp;
    temp = (sWord*) malloc(sizeof(sWord));
    temp->next = NULL;
    strcpy(temp->stop,b);
    return temp;
}

void insertStopWord(char b[10]) {
   sWord *temp = createStopNode(b);
	if(first == NULL){
        first = temp; 
    }else{
        temp->next = first; 
        first = temp; 
    }
}

void printStopList() {
   sWord *ptr = first;
   while(ptr != NULL)
	{        
      printf("%s \n",ptr->stop);
      ptr = ptr->next;
   }
}

void printList() {
   wordList *ptr = head;
   while(ptr != NULL)
	{        
      printf("%s %d ",ptr->word,ptr->count);
      int c = ptr->count;
      int i;
      for (i=0;i<c;i++) {
          printf("%d ", ptr->lineAt[i]);
      }
      printf("\n");
      ptr = ptr->next;
   }
}

void insertWord(char a[10], int c, int l) {
   wordList *temp = createNode(a,c,l);
	if(head == NULL){
        head = temp; 
    }else{
        temp->next = head; 
        head = temp; 
    }
}

int length() {
    int length = 0;
    wordList *current;
    for(current = head; current != NULL; current = current->next) {
      length++;
    }
	
   return length;
}


void sort() {
    wordList *ptr ;
    wordList *temp;
    int size = length();
    int k = size;
    char node[20];
    int g,i,j;
    int h;
    for ( i = 0 ; i < size - 1 ; i++, k-- ) {
        ptr = head ;
        temp = head->next ; 
        for ( j = 1 ; j < k ; j++ ){
            if (strcmp(ptr->word, temp->word) < 0)  {
                strcpy(node, ptr->word);
                strcpy(ptr->word, temp->word);
                strcpy(temp->word, node);

                g = ptr->count;
                ptr->count = temp->count;
                temp->count = g;

                for (int c=0; c<ptr->count;c++) {
                    h = ptr->lineAt[c];
                    ptr->lineAt[c] = temp->lineAt[c];
                    temp->lineAt[c] = h;
                }
                
            }
            ptr = ptr->next;
            temp = temp->next;
        }
        
    }
}

void stopWord() {
    char b[10];
    int i = 0;
    FILE *fstopw;
    fstopw = fopen("stopw.txt","r");
    if (fstopw != NULL)
        while (! feof(fstopw)) {
            fscanf(fstopw,"%s\n",b);
            insertStopWord(b);
        }
    fclose(fstopw);
}

int isSpecialCharacter(char c) {
    if (c>='A' && c<='Z') {
        return 0;
    } 
    else if (c>='a' && c<='z') {
        return 0;
    } 
    else if (c==' ') {
        return 0;
    } 
    else if (c==0) {
        return 0;
    }
    else return 1;
}

int deleteACharacterAt(char *s, int d) {
    int i;
    for (i=d; i<strlen(s); i++){
        s[i] = s[i+1];
    }
}

char * wordFixed(char * s) {
    int i = 0;
    while (1) {
        if (i > strlen(s)) return s;

        if (isSpecialCharacter(s[i])) {
            deleteACharacterAt(s, i);
            i--;
        }

        i++;
    }
}

int checkStopWord(char *s) {
    sWord *temp = first;
    while(temp != NULL ) {
        if (strcmp(s, temp->stop) == 0) {
            return 0;
            break;
        } else
        temp = temp->next;
    }
    return 1;
}

int checkWord(char *s,int i) {
    wordList *temp = head;
    while(temp != NULL) {
        if (strcmp(s, temp->word) == 0) {
            temp->lineAt[temp->count] = i;
            temp->count ++;
            return 1;
            break;
        } else      
        temp = temp->next;
    }
}

int getTextInEachLine(int i, char *s){
    if (*s == '\0') return 0;
    char *p;
    char text[100];
    p = strtok(s," ");
    while (p != NULL) {
        strcpy(text, p);
        wordFixed(text);
        if ((isalpha(text[0]) != 0) && (text[0] <=90))
                text[0] += 32;
        p = strtok(NULL," ");
        if ((strlen(text) != 0) && (checkStopWord(text) != 0) ) {
            if (checkWord(text, i) == 0)
            insertWord(text,1,i);
        }
    }
}

int getText() {
    int i=1;
    FILE *ftext;
    ftext = fopen("vanban.txt","r");
    if (ftext != NULL)
        while (! feof(ftext)) {
            char s[1000];
            fscanf(ftext,"%[^\n]\n",s);
            getTextInEachLine(i,s);
            i++;
        }
    fclose(ftext);
}

int main() {
    stopWord();
    getText();
    sort();
    printList();
    return 0;
}