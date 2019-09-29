#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%CHECK_WORDS%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int check_words(FILE *fp, hashmap_t hashtable[], char * misspelled[])
{	
    fflush(stdout);
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
  		if(c == ' ' || c == '\n' || c == '\0' || c == '\t' || c=='.' || c==',' || c==';' || c=='?' || c=='!' || c==':') {
    		//wordfunction(word)
    		fprintf(stdout,"DEBUG: IF Entered\n");
    		fprintf(stdout,"INFO: Checking word->***%s*** (Mispell Count) %d\n",word,count);
    		const char* w1 = word;
    		//consider the word spelling
    		if(check_word(w1,hashtable))
    		{
    			fprintf(stdout,"INFO: Correct spelling <%s>\n",word);

    		}
    		else
    		{	
                    if(count>=2000)
                    {   
                    fprintf(stdout, "ERROR: Mispell words exceed acceptable count limit of 2000. Aborting....\n");
                    fclose(fp);
                    return false;
                    }
    				misspelled[count]=malloc(strlen(word)+1);
					snprintf(misspelled[count],strlen(word)+1,word);
    				fprintf(stdout,"INFO: Mispelled-Case-1: |%s| Added to mispelled \n",misspelled[count]);
    				count+=1;
    		}
    		word[0] = 0; //Reset word
    		index = 0; //end the word here (0 is the ascii value for null)
  		} 
  		else 
  		{	//fprintf(stdout,"\n DEBUG: ELSE Entered %d \n",index);
  			//increment the word pointer and keep advancing the null terminator
    		
    		if(index>45)
    		{	//fprintf(stdout,"Begin Char ignore frm>> %c\n",c);
    			while(true){
    				c = (char)fgetc(fp);
    				
    			// Could put this inside a function, but this a lazy way to do the same.
    			if(c == ' ' || c == '\n' || c == '\0' || c == '\t' || c=='.' || c==',' || c==';' || c=='?' || c=='!' || c==':' || c==EOF)
    			{
    				break;
    			}	

    			}
    			//fprintf(stdout,"Stopping char ignore at %c\n",c);
    			
    			word[index++]='~';
    			word[index]='\0';
    			index=0;
                if(count>=2000)
                {   
                    fprintf(stdout, "ERROR: Mispell words exceed acceptable count limit of 2000. Aborting....\n");
                    fclose(fp);
                    return false;
                }
    			misspelled[count]=malloc(strlen(word)+1);
				snprintf(misspelled[count],strlen(word)+1,word);
                fprintf(stdout,"INFO: Mispelled-Case-2: |%s| Added to mispelled \n",misspelled[count]);
    			//fprintf(stdout,"INFO: Mispelled-Case-2: |%s|, truncated and added to mispelled \n",misspelled[count]);
    			word[0] = 0;
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

    //last word of the file, may have trailing spaces or newline which must be manually removed
	if(word!=NULL)
	{          
            //for that edge case where last word is not a stream of carriages.
            
            if(!newLineOrSpaces(word))
            {   int last= strlen(word)-1;
                if(word[last]==' ' || word[last]=='\n' || word[last]=='\t' || word[last]==EOF)
                {
                    //trim the word
                    //fprintf(stdout,"TRACE: Trim required\n");
                    word[last]='\0';
                    //fprintf(stdout,"TRACE: Trim done ||%s||\n",word);
                }

            }
			const char* w2 = word;
    		//consider the word spelling
            //fprintf(stdout,"TRACE: Last word of the file ||%s||\n",word);
    		if(check_word(w2,hashtable))
    		{
    			fprintf(stdout,"INFO: Correct spelling <%s>\n",word);

    		}
    		else
    		{	    
                    if(count>=2000)
                    {   
                    fprintf(stdout, "ERROR: Mispell words exceed acceptable count limit of 2000. Aborting....\n");
                    fclose(fp);
                    return false;
                    }

    				misspelled[count]=malloc(strlen(word)+1);
					snprintf(misspelled[count],strlen(word)+1,word);
    				fprintf(stdout,"INFO: Mispelled-Case-3: |%s| Added to mispelled \n",misspelled[count]);
    				count+=1;
    		}
	}
	
	fprintf(stdout,"DEBUG:Exit check_words_spaces\n");
	fflush(stdout);
    fclose(fp);
	return count;
	
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%newLineOrSpaces%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/**
    effectively ignores a stream of punctuation marks. Punctuation marks are treated as delimiters
**/
bool newLineOrSpaces(const char* word)
	{	
		//fprintf(stdout,"DEBUG: Enter newLineOrSpaces\n");
		bool condition=true;
		int length = strlen(word);
		int i=0;
		while(i<length)
		{
			if(!(word[i]=='\t' || word[i]==' ' || word[i]=='\n' || word[i]=='\r' || word[i]=='.' || word[i]==',' || word[i]==';'|| word[i]=='?' || word[i]=='!' || word[i]==':' || word[i]==EOF))
			{
				condition=false;
				break;
			}
			i++;
		}
		//fprintf(stdout,"DEBUG:Exit newLineOrSpaces condition %s\n",condition ? "true" : "false");
		return condition;
	}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%isNumber%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    
/**
Checks whether a string is constructed only based on numbers
**/
bool isNumber(const char* word)
{	
	//fprintf(stdout,"DEBUG: Enter isNumber\n");
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
	//fprintf(stdout,"DEBUG:Exit isNumber condition %s\n",condition ? "true" : "false");
	return condition;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%CHECK_WORD%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/**

**/
bool check_word(const char* word, hashmap_t hashtable[])
{	
	//fprintf(stdout,"DEBUG:ENTER check_word()\n");
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
	char *lower=(char*)malloc(strlen(word)+1);
	memcpy(lower,word,strlen(word));
	lower[strlen(word)]=0;
	int i=0;
	for(i=0;i<strlen(lower);i++)
		{
			if(isalpha(lower[i]))
			{
			lower[i]=tolower(lower[i]);
			}
		}
	
	bucket= hash_function(lower);
	
	//
	hashmap_t head = hashtable[bucket];
	hashmap_t temp = head;
	bool found=false;
	fprintf(stdout,"DEBUG: bucket of hash %d\n",bucket);
	while(temp!=NULL)
	{	
		//fprintf(stdout,"<<%s>>||<<%s>>\n",convertToLower(temp->word,strlen(temp->word)),lower);
		if(strcmp(temp->word,lower)==0)
		{	
			found=true;
			break;
		}
		temp=temp->next;
	}
	free(lower);
	fprintf(stdout,"DEBUG:EXIT check_word()\n");
	return found;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%LOAD_DICTIONARY%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



/**
This function assumes dictionary is a wordlist with newline separated words
It does not split words on the basis of other delimiters such as space and full stop.

**/
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[])
{   
    fprintf(stdout,"DEBUG:ENTER load_dictionary()\n");
    for (int i = 0; i < HASH_SIZE; i++) {
        hashtable[i] = NULL;
    }
    int bucket=-1;
    int count=0;
    char c;
    int index =0;
    FILE *fp = fopen(dictionary_file, "r");
    if(fp == NULL) {
         perror("Unable to open file!");
         return false;
          
     }
     char word1[47];
     do {
        c = (char)fgetc(fp);
        if(c == ' ' || c == '\n' || c == '\0' || c == '\t' || c=='.' || c==',' || c==';' || c=='?' || c=='!' || c==':') {
            //wordfunction(word)
            //fprintf(stdout,"DEBUG: Loading %s \n",word1);
            
            if(newLineOrSpaces(word1) || isNumber(word1))
            {
                //fprintf(stdout,"TRACE:Not a word, so not adding to dictionary |%s|\n",word1);
                //do not do anything, ignore the word
            }
            else
            {   
                char* newword =0;
                if(word1[strlen(word1)-1]==10)
                {
                    newword = (char*)malloc(strlen(word1));
                    memcpy(newword,word1,strlen(word1)-1);
                    newword[strlen(word1)-1] = 0;
                    int i=0;
                    for(i=0;i<strlen(newword);i++)
                    {
                        if(isalpha(newword[i]))
                        {
                        newword[i]=tolower(newword[i]);
                        }
                    }
           
                }
                else
                {
                    newword = (char*)malloc(strlen(word1)+1);
                    memcpy(newword,word1,strlen(word1));
                    newword[strlen(word1)] = 0;
                    int i=0;
                    for(i=0;i<strlen(newword);i++)
                    {
                        if(isalpha(newword[i]))
                        {
                            newword[i]=tolower(newword[i]);
                        }
                    }
            
                }
                bucket = hash_function(newword);
                hashmap_t head = hashtable[bucket];

            if(head==NULL)
            {
                //head is empty
                hashtable[bucket] = (node *) malloc(sizeof(node));
                hashtable[bucket]->next = NULL;
            //fprintf(stdout,"Head attained\n");
            
            
                if(strlen(newword)<=LENGTH)
                {   
                    //change strcpy
                    snprintf(hashtable[bucket]->word,sizeof(hashtable[bucket]->word),newword);
                    free(newword);
                
                }
                else
                {
                    fprintf(stdout,"WARN: Could not add word from dictionary due to size violation");
                    fprintf(stdout,"Skipping ......");
                }
            //fprintf(stdout,"DEBUG: word |%s| placed in bucket:%d at head: word_count=%d\n",hashtable[bucket]->word,bucket,count);
            }
            else
            {
                 hashmap_t temp=head;

            //fprintf(stdout,"bucket attained %d\n",bucket);

                int pos=0;
                while(temp->next!=NULL)
                {   
                    pos=pos+1;
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
            }   
        }

            
                

            word1[0] = 0; //Reset word
            index = 0; //end the word here (0 is the ascii value for null)
        } 
        else 
        {   
            if(index>45)
            {   //fprintf(stdout,"Begin Char ignore frm>> %c\n",c);
                while(true){
                    c = (char)fgetc(fp);
                    
                // Could put this inside a function, but this a lazy way to do the same.
                if(c == ' ' || c == '\n' || c == '\0' || c == '\t' || c=='.' || c==',' || c==';' || c=='?' || c=='!' || c==':' || c==EOF)
                {
                    break;
                }   

                }
                //fprintf(stdout,"Stopping char ignore at %c\n",c);
                
                
                word1[index]='\0';
                fprintf(stdout,"Word is too long, ignoring it %s",word1); //word length size violation
                index=0;
                /**
                Write if you want to add the truncated word to dictionary
                **/
                word1[0] = 0;
                count+=1;
                
            }
            else
            {
                word1[index++] = c;
                word1[index] = 0;
            }
 
     
            } 
        }while(c != EOF);
        fclose(fp);
        fprintf(stdout,"LAST word %s\n",word1);
        if(word1!=NULL)
        {   
            //redoing the same thing which is done when a word is found in load_dictionary

            fprintf(stdout,"DEBUG:Case:last_word OF dictionary ||%d||\n",strlen(word1));
            if(newLineOrSpaces(word1) || isNumber(word1))
            {
                fprintf(stdout,"TRACE:LAST word was not added as it is not alphabetical |%s|\n",word1);
                //do not do anything, ignore the word
            }
            else
            {   
                char* newword =0;
                int last= strlen(word1)-1;
                if(word1[last]==' ' || word1[last]=='\n' || word1[last]=='\t' || word1[last]==EOF)
                {
                    //trim the word
                    //fprintf(stdout,"TRACE: Trim required\n");
                    word1[last]='\0';
                    //fprintf(stdout,"TRACE: Trim done ||%s||\n",word);
                }
                if(word1[strlen(word1)-1]==10)
                {
                    newword = (char*)malloc(strlen(word1));
                    memcpy(newword,word1,strlen(word1)-1);
                    newword[strlen(word1)-1] = 0;
                    int i=0;
                    for(i=0;i<strlen(newword);i++)
                    {
                        if(isalpha(newword[i]))
                        {
                        newword[i]=tolower(newword[i]);
                        }
                    }
           
                }
                else
                {
                    newword = (char*)malloc(strlen(word1)+1);
                    memcpy(newword,word1,strlen(word1));
                    newword[strlen(word1)] = 0;
                    int i=0;
                    for(i=0;i<strlen(newword);i++)
                    {
                        if(isalpha(newword[i]))
                        {
                            newword[i]=tolower(newword[i]);
                        }
                    }
            
                }
                bucket = hash_function(newword);
                hashmap_t head = hashtable[bucket];

            if(head==NULL)
            {
                //head is empty
                hashtable[bucket] = (node *) malloc(sizeof(node));
                hashtable[bucket]->next = NULL;
            //fprintf(stdout,"Head attained\n");
            
            
                if(strlen(newword)<=LENGTH)
                {   
                    //change strcpy
                    snprintf(hashtable[bucket]->word,sizeof(hashtable[bucket]->word),newword);
                    free(newword);
                
                }
                else
                {
                    fprintf(stdout,"WARN: Could not add word from dictionary due to size violation");
                    fprintf(stdout,"Skipping ......");
                }
            fprintf(stdout,"DEBUG: word |%s| placed in bucket:%d at head: word_count=%d\n",hashtable[bucket]->word,bucket,count);
            }
            else
            {
                 hashmap_t temp=head;

            //fprintf(stdout,"bucket attained %d\n",bucket);

                int pos=0;
                while(temp->next!=NULL)
                {   
                    pos=pos+1;
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
                fprintf(stdout,"DEBUG: word |%s| placed in bucket:%d at pos %d: word_count=%d\n",newword,bucket,pos+1,count);
            }   
        }
        }
        fprintf(stdout,"DEBUG:EXIT load_dictionary()\n");
        return true;
}



//check function to print all words in a bucket (same hashfunction value)
void print_bucket(int bucket,hashmap_t x[])
{	
	fprintf(stdout,"DEBUG: ENTER print_bucket()\n");
    hashmap_t head = x[bucket];
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
        return ;
}

void print_mispelled(char * mispelled[],int count)
{	
	fprintf(stdout,"DEBUG:spell.c:ENTER printf_mispelled()\n");
	char **ptr;
	ptr= mispelled;
	int i=0;
	while(i<count)
	{
		fprintf(stdout,"%d. <<%s>>\n",i+1,*(ptr+i));
		i+=1;
	}
	
	fprintf(stdout,"DEBUG:spell.c:EXIT printf_mispelled()\n");
	return;
}
