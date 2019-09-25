#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"
node* hashtable[HASH_SIZE];
char* mispelled[1005];
//Takes the name of file to be read as Input


// Maps a word to an integer value to place it in the hash table.
// Sum the value of each character in the word, then find the 
// remainder after dividing by the size of the hash table.
int check_words(FILE *fp, hashmap_t hashtable[], char * misspelled[])
{	fflush(stdout);
	fprintf(stdout,"DEBUG:Enter check_words_spaces\n");
	char word[47];
	char c;
	if(fp == NULL) {
         perror("Unable to open file!");
          exit(1);
     }
	int index = 0;
	int count=0;
	do {
  		c = (char)fgetc(fp);
  		if(c == ' ' || c == '\n' || c == '\0' || c == '\t') {
    		//wordfunction(word)
    		fprintf(stdout,"DEBUG: IF Entered\n");
    		fprintf(stdout,"INFO: Checking word->***%s***%d\n",word,strlen(word));
    		const char* w1 = word;
    		//consider the word spelling
    		if(check_word(w1,hashtable))
    		{
    			fprintf(stdout,"INFO: Correct spelling <%s>\n",word);

    		}
    		else
    		{	

    				misspelled[count]=malloc(strlen(word)+1);
					snprintf(misspelled[count],strlen(word)+1,word);
    				fprintf(stdout,"INFO: Mispelled: |%s| Added to mispelled \n",misspelled[count]);
    				count+=1;
    		}
    		word[0] = 0; //Reset word
    		index = 0;
  		} 
  		else 
  		{	//fprintf(stdout,"\n DEBUG: ELSE Entered %d \n",index);
  			//increment the word pointer and keep advancing the null terminator
    		
    		if(index>45)
    		{	//fprintf(stdout,"Begin Char ignore frm>> %c\n",c);
    			while(true){
    				c = (char)fgetc(fp);
    				
    				//fprintf(stdout,"Ignoring |%c|",c);
    			if(c == ' ' || c == '\n' || c == '\0' || c == '\t' || c==EOF)
    			{
    				break;
    			}	

    			}
    			//fprintf(stdout,"Stopping char ignore at %c\n",c);
    			
    			word[index++]='~';
    			word[index]='\0';
    			index=0;
    			misspelled[count]=malloc(strlen(word)+1);
				snprintf(misspelled[count],strlen(word)+1,word);
    			fprintf(stdout,"Mispelled: |%s|, truncated and added to mispelled \n",misspelled[count]);
    			count+=1;
    			
    		}
    		else
    		{
    			word[index++] = c;
    			word[index] = 0;
    		}
    		//fprintf(stdout,"DEBUG: ||%c::%d||%s||",c,index,word);
  		}	
  	
	} while(c != EOF);
	if(word!=NULL)
	{
			const char* w2 = word;
    		//consider the word spelling
    		if(check_word(w2,hashtable))
    		{
    			fprintf(stdout,"INFO: Correct spelling <%s>\n",word);

    		}
    		else
    		{	

    				misspelled[count]=malloc(strlen(word)+1);
					snprintf(misspelled[count],strlen(word)+1,word);
    				fprintf(stdout,"Mispelled: |%s| Added to mispelled \n",misspelled[count]);
    				count+=1;
    		}
	}
	
	fprintf(stdout,"DEBUG:Exit check_words_spaces\n");
	fflush(stdout);
	return count;
	
}

bool newLineOrSpaces(const char* word)
	{	
		fprintf(stdout,"DEBUG: Enter newLineOrSpaces\n");
		bool condition=true;
		int length = strlen(word);
		int i=0;
		while(i<length)
		{
			if(!(word[i]=='\t' || word[i]==' ' || word[i]=='\n' || word[i]=='\r' || word[i]==EOF))
			{
				condition=false;
				break;
			}
			i++;
		}
		fprintf(stdout,"DEBUG:Exit newLineOrSpaces condition %s\n",condition ? "true" : "false");
		return condition;
	}
bool isNumber(const char* word)
{	
	fprintf(stdout,"DEBUG: Enter isNumber\n");
	bool condition=true;
	int length=strlen(word);
	int i=0;
	while(i<length)
	{
		if(!isdigit(word[i]))
		{
			condition=false;
			break;
		}
		i++;
	}
	fprintf(stdout,"DEBUG:Exit isNumber condition %s\n",condition ? "true" : "false");
	return condition;
}
//function to ignore case when alphabets are involved
char* convertToLower(const char* word, int size_word)
{	
	char *x= (char*)malloc(strlen(word));
	//pointing to string literal passed
	int i=0;
	for(i=0;i<size_word;i++)
	{
		if(isalpha(word[i]))
		{
			x[i]=tolower(word[i]);
		}
	}
	x[i]='\0';
	return x;
}

//length check is already applied in check_words function so not needed here
bool check_word(const char* word, hashmap_t hashtable[])
{	
	fprintf(stdout,"DEBUG:ENTER check_word()\n");
	fflush(stdout);
	
	int bucket=-1;
	
	//fprintf(stdout,"DEBUG 9:\n");
	if(strlen(word)>LENGTH)
	{
		fprintf(stdout,"ERROR:Size violation, word does not exist\n");
		return false;
	}
	if(isNumber(word))
	{	
		fprintf(stdout,"INFO: Word has only numbers");
		return true;
	}
	if(newLineOrSpaces(word))
	{	
		fprintf(stdout,"INFO: Chunk has no characters except spaces,tabs,newline,carriages");
		return true;
	}


	//fprintf(stdout,"%s\n",lower);
	//ignoring cases in the spellchecker

	const char* lower=convertToLower(word,strlen(word));
	//fprintf(stdout,"LOWERCASE> ||%s||\n",lower);
	bucket= hash_function(lower);
	//
	hashmap_t head = hashtable[bucket];
	hashmap_t temp = head;
	bool found=false;
	fprintf(stdout,"DEBUG: bucket of hash %d\n",bucket);
	while(temp!=NULL)
	{	
		//fprintf(stdout,"<<%s>>||<<%s>>\n",convertToLower(temp->word,strlen(temp->word)),lower);
		if(strcmp(convertToLower(temp->word,strlen(temp->word)),lower)==0)
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
 
     while(fgets(word1, sizeof(word1), fp) != NULL) {
         //fputs(chunk, stdout);

        //fprintf(stdout,">>%s<<\n",word1);

        

   		char* newword =0;
        newword = (char*)malloc(strlen(word1));
		memcpy(newword,word1,strlen(word1)-1);
		newword[strlen(word1)-1] = 0;

		//const char* lower = convertToLower(newword,strlen(newword));

		bucket = hash_function(newword);
        hashmap_t head = hashtable[bucket];

        if(head==NULL)
        {
        	//head is empty
        	hashtable[bucket] = (node *) malloc(sizeof(node));
        	hashtable[bucket]->next = NULL;
        	//fprintf(stdout,"Head attained\n");
        	
        	
        	if(strlen(newword)<LENGTH)
        	{	
        		//change strcpy
            	snprintf(hashtable[bucket]->word,sizeof(hashtable[bucket]->word),newword);
            	free(newword);
            	
        	}
        	else
        	{
        		fprintf(stdout,"WARN: Could not add word from dicitonary due to size violation");
        		fprintf(stdout,"Skipping ......");
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
            	snprintf(newnode->word,sizeof(newnode->word),newword);
            	free(newword);
        	}
        	newnode->next=0;
        	temp->next = newnode;

        	//if(bucket==966)
        	//{
        	//fprintf(stdout,"DEBUG: word %s placed in bucket:%d at pos %d: word_count=%ld\n",newword,bucket,pos+1,count);
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
        	//fprintf(stdout,"%s\n",temp->word);  
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

void print_mispelled(char * mispelled[],int count)
{	
	fprintf(stdout,"DEBUG:spell.c:ENTER printf_mispelled()\n");
	char **ptr;
	ptr= mispelled;
	int i=0;
	while(i<count)
	{
		fprintf(stdout,"<<%s>>",*(ptr+i));
		i+=1;
	}
	
	fprintf(stdout,"DEBUG:spell.c:EXIT printf_mispelled()\n");
	return;
}



int main()
{
	char s[]="wordlist.txt";
	char y[]="CaNnibAL";
	int x=0;
	load_dictionary(s,hashtable);
	FILE *fp;
	const char* ptr=y;
	fp = fopen("check.txt", "r");
	x=check_words(fp,hashtable,mispelled);
	fprintf(stdout,"Mispell count %d\n",x);
	print_mispelled(mispelled,x);
	char *p=convertToLower(ptr,strlen(y));
	fprintf(stdout,"||%s||",p);
	return 0;
}
