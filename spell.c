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
int x =900;
fprintf(stdout,"Contents of bucket %d\n",x);
print_bucket(x,hashtable);
printf("DEBUG: 5\n");
//bucket 1331
if(check_word("improvements\n",hashtable))
{
	printf("Spelled correctly\n");
}
else
{
	printf("Spelled incorrectly\n");
}
//printf("%d",check_life());

return 0;
}
