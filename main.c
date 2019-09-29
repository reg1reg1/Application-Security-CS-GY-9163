#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ctype.h>
#include "dictionary.h"

//global variable declaration to reduce heap memory space


int main(int argc, char *argv[] )
{	
	node* hashtable[HASH_SIZE];
	char* misspelled[MAX_MISSPELLED];
	bool status=false;
	int misspell_count=0;
	
	if (argc!=3)
	{
		fprintf(stdout,"Usage incorrect!\n");
		fprintf(stdout,"./spell_check <dictionary.txt> <file_to_be.txt>\n");
		
	}
	else
	{	
		const char *dictionary = argv[2];
		const char *inputText = argv[1];
		fprintf(stdout,"%s %s\n",dictionary,inputText);
		status=load_dictionary(dictionary,hashtable);
		if(!status)
		{
			fprintf(stdout,"Could not load dictionary file,...\n Aborting ...\n");
			return 0;
		}
		FILE *fp = fopen(inputText,"r");
		if(fp == NULL) {
         perror("Unable to open file!");
          exit(1);
     	}
     	misspell_count= check_words(fp,hashtable,misspelled);
     	//fclose(fp);
     	if(misspell_count>0)
     	{	 fprintf(stdout,"\n\n\n\n**************%d Mispelled words***************\n",misspell_count);

     		 print_mispelled(misspelled,misspell_count);
     		 fprintf(stdout,"\n\n\n\n************************************************\n");
     	}
     	else
     	{
     		fprintf(stdout,"All words are spelled correctly!\n");
     	}

	}
	exit(0);
}

/** Commands to execute
gcc -Wall -c dictionary.c dictionary.h
gcc -Wall -c spell.c
gcc -Wall -c main.c
gcc -Wall -o spell_check dictionary.o spell.o main.o
**/