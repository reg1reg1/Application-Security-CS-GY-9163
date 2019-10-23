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
