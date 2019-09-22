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
 	int count =0;
    while(EOF != fscanf(fp, "%[^\n]\n", word1)) {
     	
     	
     	
        
        
		fprintf(stdout,"CHECKING %s\n",word1);
		const char* checkword = word1; 
		long long int wordLength= strlen(word1);
		
		if(wordLength>=0 && wordLength<46)
		{	
			//skip till end of line once the word length is violated
			int skipbytes= (strlen(wordLength)-45)*sizeof(char);
			misspelled[count]=malloc(strlen(word1)+1);
			snprintf(misspelled[count],sizeof(word1),word1);
			//set the file pointer correctly if needed using fseek
		}


		if(check_word(checkword,hashtable))
		{
			fprintf(stdout,"Correct spelling ,length->%d\n");
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
	
    return count;
}

bool check_word(const char* word, hashmap_t hashtable[])
{	
	fprintf(stdout,"DEBUG:ENTER check_word()\n");
	fflush(stdout);
	
	int bucket=-1;
	
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
        	newnode->next=0;
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
        fprintf(stdout,"DEBUG: EXIT print_bucket()\n");
        return 0;
}

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
check_word("yodeler",hashtable);
FILE *fp = fopen("check.txt", "r");
check_words(fp,hashtable,mispelled);
 int i=0;

//print_mispelled(mispelled);
//printf("%d",check_life());

return 0;
}
