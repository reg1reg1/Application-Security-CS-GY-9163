#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"
node* hashtable[HASH_SIZE];

//Takes the name of file to be read as Input

int main()
{
char s[]="wordlist.txt";
load_dictionary(s,hashtable);
print_bucket(hashtable);
//printf("%d",check_life());

return 0;
}
