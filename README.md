# Application-Security-CS-GY-9163
Repository for the Fall'19 graduate course on Application Security.

## Assignment 1
3 subsections of task were given this week.

- **Week 1**
	- Setup Git on local machine
	- Setup a GPG key to sign your commits
	- Setup a github account, add GPG key to account
	- Setup Travis CI
- **Week 2**
	- Implement a spell checker in C. Use the prototypes of functions provided in the dictionary.h file
	- Use aspell utility after installing to generate the wordlist for standard uniformity.
	- Analyze the program using Valgrind.
	- Make a writeup that includes the following.
		- exactly how the program works
 		- The output of Valgrind
		- what bugs you expect may exist in your code
		- why those bugs might occur
		- what steps you took to mitigate or fix those bugs. 
- **Week 3**
	- Write test cases for the program written in Week 2; ensure substantial coverage
	- Once test cases have been written, fuzz the C code using AFL fuzzer.
	- Identify the bugs AFL reports and then fix them. Write test cases to cover the identified bugs.
	- Write a report that includes the following
		- all of the bugs found by your tests
		- all of the bugs found by fuzzing
		- how the bugs were fixed
		- how similar bugs can be avoided in the future. 
