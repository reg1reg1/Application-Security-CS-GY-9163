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

char * read_file(const char* str)
{   
    char * buffer = 0;
    long length;
    FILE * fptr = fopen (str, "rb");
    if (fptr)
    {   //predetermine the s
        fseek (fptr, 0, SEEK_END);
        length = ftell (fptr);
        fseek (fptr, 0, SEEK_SET);
        buffer = malloc (length+1);
        if (buffer)
        {
            fread (buffer, 1, length, fptr);
        }
        fclose (fptr);
        buffer[length] = '\0';
    }
    if (buffer)
    {   
        return buffer;
        
        
    }
    return NULL;
}
// Maps a word to an integer value to place it in the hash table.
// Sum the value of each character in the word, then find the 
// remainder after dividing by the size of the hash table.
int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[])
{
	//
    return 0;
}

bool check_word(const char* word, hashmap_t hashtable[])
{
	printf("TRACE:Inside checkword");
	int bucket=-1;
	char* pch = word;
	char* p;
	if(strlen(word)>LENGTH || strlen(word)<=0)
	{
		printf("ERROR:Size violation, word does not exist");
		return false;
	}
	for ( ; *p; ++p) *p = tolower(*pch);
	const char* lower = p;
	printf("%s",lower);
	//ignoring cases in the spellchecker


	bucket= hash_function(lower);
	//
	hashmap_t head = hashtable[bucket];
	hashmap_t temp = head;
	bool found=false;
	printf("DEBUG: bucket of hash %d",bucket);
	while(temp->next!=NULL)
	{
		if(strcmp(temp->word,lower)==0)
		{
			found=true;
		}
	}
	return found;
}


//the input is the filename
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[])
{
    //inserting into hashmap_t
    char *buffer = read_file(dictionary_file);
    
    char* pch = NULL;
    char* word1;
    pch = strtok(buffer, "\n");
    /* split string and append tokens to 'res' */
    if(!buffer)
    {
        return false;
        
    }
    long count=0;
    int bucket=-1;
    char *nullv = NULL;
    while (pch != NULL)
    {   printf("WTF");
        //printf("%ld %s\n",count,pch);
        
        pch = strtok(nullv, "\n");
        word1 = pch;
        
        //printf("%d %s hello\n",strlen(word1),word1);
        //printf("%d %s\n",count,word1);
        if(!word1)
        {	
        	printf("No more words....");
        	break;
        	
        }
        word1[strlen(word1)-1] = 0;
        bucket = hash_function(word1);
        
        hashmap_t head = hashtable[bucket];
        if(head==NULL)
        {
        	//head is empty
        	hashtable[bucket] = (node *) malloc(sizeof(node));
        	hashtable[bucket]->next = NULL;
        	//printf("Head attained\n");
        	if(strlen(word1)<LENGTH)
        	{
            	strcpy(hashtable[bucket]->word,word1);
            	
        	}
        	else
        	{
        		printf("Error: Could not add word due to size violation");
        		printf("Exiting......");
        	}
        }
        else
        {
        	 hashmap_t temp=head;

        	//printf("bucket attained %d\n",bucket);
        	int pos=0;
        	while(temp->next!=NULL)
        	{   pos=pos+1;
        		//printf("%s\n",word1);
           	 	//printf("Inserted word %s at bucket %d, %d POS\n",word1,bucket,pos);
        		//printf("Possible infinite loop\n");
            	temp=temp->next;
            
        	}

        	temp->next = (node *) malloc(sizeof(node));
        	//printf("Head attained\n");
        	if(strlen(word1)<LENGTH)
        	{
            	strcpy(temp->next->word,word1);
            
        	}
        	//if(bucket==966)
        	//{
        	printf("DEBUG: word %s placed in bucket:%d at pos %d: word_count=%ld\n",word1,bucket,pos+1,count);
        	//printf("\n>>>>>\n");
        	//}
        	//printf("%s %s %d %d\n---------\n",temp->word,word1,pos,bucket);
        	temp->next->next=NULL;

        }
        count++;
        
    }
    printf("End of while loop");
    
   
    return true;
}

//check function to print all words in a bucket (same hashfunction value)
void print_bucket(int bucket,hashmap_t x[])
{
    hashmap_t head = hashtable[bucket];
    hashmap_t temp = head;
    while(temp!=NULL)
        {   
            printf("%s-->",temp->word);
            temp=temp->next;
            
        }
        printf("\n");
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
