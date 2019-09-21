#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"
node* hashtable[HASH_SIZE];
char* mispelled[2001];
//Takes the name of file to be read as Input
void print_mispelled(char * mispelled[])
{	
	fprintf(stdout,"DEBUG:spell.c:ENTER printf_mispelled()\n");
	char **ptr;
	if(!mispelled)
	{
		fprintf(stdout,"No mispelled words");
	}
	ptr = mispelled;
	while(*ptr!="")
	{
		  printf ("Mispelled words\n>:%s\n",  *ptr++); //* takes precedence over ++
	}
	fprintf(stdout,"DEBUG:spell.c:EXIT printf_mispelled()\n");
	return;
}


int main()
{
char s[]="wordlist.txt";
load_dictionary(s,hashtable);
int x =756;
fprintf(stdout,"Contents of bucket %d\n",x);
print_bucket(x,hashtable);
//check_word("bucket",hashtable);
FILE *fp = fopen("check.txt", "r");
//check_words(fp,hashtable,mispelled);
 int i=0;

//print_mispelled(mispelled);
//printf("%d",check_life());

return 0;
}
