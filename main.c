#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ctype.h>
#include "dictionary.h"
node* hashtable[HASH_SIZE];
char* mispelled[1005];


int main(int argc, char *argv[] )
{	char s[]="wordlist.txt";
	
	int x=0;
	load_dictionary(s,hashtable);
	FILE *fp;
	
	fp = fopen("check.txt", "r");
	x=check_words(fp,hashtable,mispelled);
	fprintf(stdout,"Mispell count %d\n",x);
	print_mispelled(mispelled,x);
	return 0;
}

/** Commands to execute
gcc -Wall -c dictionary.c dictionary.h
gcc -Wall -c spell.c
gcc -Wall -c main.c
gcc -Wall -o spell_check dictionary.o spell.o main.o
**/