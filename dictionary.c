/****************************************************************************
 * dictionary.c
 *
 * Application Security, Assignment 1
 *
 * Adapted from code written by Ben Halperin.
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

// Hash table is an array of linked lists.
hashmap_t hashtable[HASH_SIZE];


// Maps a word to an integer value to place it in the hash table.
// Sum the value of each character in the word, then find the 
// remainder after dividing by the size of the hash table.
int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[])
{
	//
	fprintf(stdout,"DEBUG:ENTER check_words()\n");
	int bucket=-1;
    int count=0;
   
 	if(fp == NULL) {
         perror("Unable to open file!");
          exit(1);
     }
 
     char word1[46];
 
     while(EOF != fscanf(fp, "%[^\n]\n", word1)) {
     	
     	
     	
        int count =0;
        
		fprintf(stdout,"CHECKING %s\n",word1);
		const char* checkword = word1; 
		

		if(check_word(checkword,hashtable))
		{
			fprintf(stdout,"Correct spelling \n",checkword);
		}
		else
		{	if(count>1000)
			{

				fprintf(stdout,"WARN: Max misspelled words limit reached\n");
				return -1;
			}
			misspelled[count]=malloc(strlen(word1)+1);
			snprintf(misspelled[count],sizeof(word1),word1);
			fprintf(stdout,"MISSPELLED >%s\n",misspelled[count]);
			count+=1;
		}
	}
	int i;
	fprintf(stdout,"DEBUG: START Mispelled words\n");
	
    fprintf(stdout,"DEBUG:MISPELLED check_words()\n");
	fprintf(stdout,"DEBUG:EXIT check_words()\n");
    return count;
}

bool check_word(const char* word, hashmap_t hashtable[])
{	
	fprintf(stdout,"DEBUG:ENTER check_word()\n");
	fflush(stdout);
	
	int bucket=-1;
	char* pch = word;
	char* p;
	//fprintf(stdout,"DEBUG 9:\n");
	if(strlen(word)>LENGTH || strlen(word)<=0)
	{
		fprintf(stdout,"ERROR:Size violation, word does not exist\n");
		return false;
	}
	
	//fprintf(stdout,"%s\n",lower);
	//ignoring cases in the spellchecker


	bucket= hash_function(word);
	//
	hashmap_t head = hashtable[bucket];
	hashmap_t temp = head;
	bool found=false;
	fprintf(stdout,"DEBUG: bucket of hash %d\n",bucket);
	while(temp!=NULL)
	{	//fprintf(stdout,"%s\n",temp->word);
		if(strcmp(temp->word,word)==0)
		{	
			found=true;
		}
		temp=temp->next;
	}
	fprintf(stdout,"DEBUG:EXIT check_word()\n");
	return found;
}


//the input is the filename
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[])
{
    //inserting into hashmap_t
   	fprintf(stdout,"DEBUG: ENTER load_dictionary()\n");
    int bucket=-1;
    int count=0;
    FILE *fp = fopen(dictionary_file, "r");
 	if(fp == NULL) {
         perror("Unable to open file!");
          exit(1);
     }
 
     char word1[46];
 
     while(EOF != fscanf(fp, "%[^\n]\n", word1)) {
         //fputs(chunk, stdout);

        //fprintf(stdout,">>%s<<\n",word1);

        

   		

		bucket = hash_function(word1);
        hashmap_t head = hashtable[bucket];

        if(head==NULL)
        {
        	//head is empty
        	hashtable[bucket] = (node *) malloc(sizeof(node));
        	hashtable[bucket]->next = NULL;
        	//fprintf(stdout,"Head attained\n");
        	
        	
        	if(strlen(word1)<LENGTH)
        	{	
        		//change strcpy
            	snprintf(hashtable[bucket]->word,sizeof(hashtable[bucket]->word),word1);

            	
        	}
        	else
        	{
        		fprintf(stdout,"Error: Could not add word due to size violation");
        		fprintf(stdout,"Exiting......");
        	}
        	//fprintf(stdout,"DEBUG: word %s placed in bucket:%d at head: word_count=%ld\n",hashtable[bucket]->word,bucket,count);
        }
        else
        {
        	 hashmap_t temp=head;

        	//fprintf(stdout,"bucket attained %d\n",bucket);

        	int pos=0;
        	while(temp->next!=NULL)
        	{   pos=pos+1;
        		//fprintf(stdout,"%s\n",word1);
           	 	//fprintf(stdout,"Inserted word %s at bucket %d, %d POS\n",word1,bucket,pos);
        		//fprintf(stdout,"Possible infinite loop\n");
            	temp=temp->next;
            
        	}
        	hashmap_t newnode = (node *) malloc(sizeof(node));
        	if(strlen(word1)<=LENGTH)
        	{
            	snprintf(newnode->word,sizeof(newnode->word),word1);
            
        	}
        	newnode->next=NULL;
        	temp->next = newnode;

        	//if(bucket==966)
        	//{
        	//fprintf(stdout,"DEBUG: word %s placed in bucket:%d at pos %d: word_count=%ld\n",word1,bucket,pos+1,count);
        	//fprintf(stdout,"\n>>>>>\n");
        	

        }
        //fprintf(stdout,"DEBUG:3");
        count++;
         //fputs("|*\n", stdout);  // marker string used to show where the content of the chunk array has ended
     }
 
     fclose(fp);
     fprintf(stdout,"DEBUG: EXIT load_dictionary()\n");
     return true;
 	
}

//check function to print all words in a bucket (same hashfunction value)
int print_bucket(int bucket,hashmap_t x[])
{	
	fprintf(stdout,"DEBUG: ENTER print_bucket()\n");
    hashmap_t head = hashtable[bucket];
    hashmap_t temp = head;
    while(temp!=NULL)
        {         
        	fprintf(stdout,"%s\n",temp->word);  
            if(temp->next!=NULL)
            {	//fprintf(stdout,"debug 2:");
            	temp=temp->next;
            }
            else
            {	//fprintf(stdout,"debug 3:");
            	break;
            }
            
            
        }
        
        free(temp);

        //fprintf(stdout,"\n");
        fprintf(stdout,"DEBUG: EXIT print_bucket()\n");
        return 0;
}

int hash_function(const char* word)
{
    int sum = 0;
    int word_length = strlen(word);

    for (int i = 0; i < word_length; i++)
    {
        sum += word[i];
    }
	    
    int bucket = sum % HASH_SIZE;
    return bucket;

}
//each bucket is a linkedlist of arrays with the same sum value
//a bucket also indicates a collision in the hash table
