#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"
#undef calculate
#undef getrusage
#define expected_size 118819
// default dictionary
#define DICTIONARY "wordlist.txt"
#define BODICTIONARY "bo_wordlist.txt"
#define MAX_MISSPELLED 1000
int main(int argc, char** argv) {
    hashmap_t hashtable[HASH_SIZE];
    if (argc < 3) {
        fprintf(stderr, "Error: Insufficient arguments!\n");
        fprintf(stderr, "Usage: ./program to_check.txt wordlist.txt\n");
        exit(-1);
    }
    char read_mode[2] = "r";
    FILE* fp = fopen(argv[1], read_mode);
    char * dictionary = argv[2]; 
    load_dictionary(dictionary, hashtable);
    char* misspelled[MAX_MISSPELLED];
    int num_wrong = check_words(fp, hashtable, misspelled);
    for (int i = 0; i < num_wrong; i++) {
        printf("%s\n", misspelled[i]);
    }
}
