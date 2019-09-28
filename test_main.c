#include <check.h>
#include "dictionary.h"
#include <stdlib.h>

#define DICTIONARY "wordlist.txt"
#define TESTDICT "test_wordlist.txt"
#define TESTDICT2 "test_dictionary.txt"


START_TEST(test_dictionary_normal)
{   
    node* hashtable[HASH_SIZE];
    ck_assert(load_dictionary(TESTDICT, hashtable));
    // Here we can test if certain words ended up in certain buckets
    // to ensure that our load_dictionary works as intended. I leave
    // this as an exercise.
}
END_TEST

START_TEST(test_check_word_normal)
{   
    node* hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* correct_word = "Justice";
    const char* punctuation_word_2 = "pl.ace";
    const char* numerics = "12345678";
    const char* punctuation ="Where?";
    const char* punctuation_only = ";!?.,";
    ck_assert(check_word(correct_word, hashtable));
    ck_assert(!check_word(punctuation_word_2, hashtable));
    ck_assert(check_word(numerics, hashtable));
    ck_assert(check_word(punctuation, hashtable));
    ck_assert(check_word(punctuation_only, hashtable));
}
END_TEST

//checking case sensitiveness
START_TEST(test_check_word_case)
{   
    node* hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* correct_word = "IEEE";
    const char* lcase = "ieee";
    ck_assert(check_word(correct_word, hashtable));
    ck_assert(check_word(lcase, hashtable));
}
END_TEST





START_TEST(test_check_words_normal)
{   
    node* hashtable[HASH_SIZE];
    char* misspelled[MAX_MISSPELLED];
    load_dictionary(DICTIONARY, hashtable);
    char* expected[3];
    expected[0] = "sogn";
    expected[1] = "skyn";
    expected[2] = "betta";
    FILE *fp = fopen("test1.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 3);
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
}
END_TEST

/**
    This test case is for checking length and edge cases
    A word of length> 45 is stored as follows
    word is given a length of 45 chars and appended by character
    caused truncation and appended by ~. Hence
    [45 A's][GGG] will be shown as [45 A's][G][~]
    This test file also checks that spaces and newlines are not treated
    as mispelled words
**/
START_TEST(test_check_words_huge)
{   
    node* hashtable[HASH_SIZE];
    char* misspelled[MAX_MISSPELLED];
    load_dictionary(DICTIONARY, hashtable);
    char* expected[3];
    expected[0] = "oons";
    expected[1] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAG~";
    expected[2] = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBCC~";
    FILE *fp = fopen("test_wordlist_2.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 3);
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
}
END_TEST




Suite *
check_word_suite(void)
{
    Suite * suite;
    TCase * check_word_case;
    suite = suite_create("check_word");
    check_word_case = tcase_create("Core");
    tcase_add_test(check_word_case, test_check_word_normal);
    tcase_add_test(check_word_case, test_check_words_normal);
    tcase_add_test(check_word_case, test_check_words_huge);
    tcase_add_test(check_word_case, test_check_word_case);
    suite_add_tcase(suite, check_word_case);

    return suite;
}

int
main(void)
{
    int failed;
    Suite *suite;
    SRunner *runner;
    
    suite = check_word_suite();
    runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

