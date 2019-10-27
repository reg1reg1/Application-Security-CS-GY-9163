# Application-Security-CS-GY-9163
Repository for the Fall'19 graduate course on Application Security.

## Assignment 1: Code up the Binary

3 subsections of task were given this week.
At the end of this week, we were expected to have a reasonably stable and secure C program which will take 2 files as input and output correctly the mispelled words. 
The work done for this assignment is in the folder C-Code
A pdf is also attached.

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
	- All of the code must be written in a file called **spell.c**.
	- The program should accept 2 arguments file to be checked **1st argument** and wordlist **2nd argument**.
	
- **Week 3**
	- Write test cases for the program written in Week 2; ensure substantial coverage
	- Once test cases have been written, fuzz the C code using AFL fuzzer.
	- Identify the bugs AFL reports and then fix them. Write test cases to cover the identified bugs.
	- Write a report that includes the following
		- all of the bugs found by your tests
		- all of the bugs found by fuzzing
		- how the bugs were fixed
		- how similar bugs can be avoided in the future. 

## Assignment 2: Flask Web Application

**Week 4,5 and 6**
Setup a web application in flask with the login registration functionality. The spell_check binary programmed earlier is called by this web_app and feeds input to it from web users. This functionality should be only available to registered users.

- This web app must be made secure from different kinds of web attacks but not limited to such as follows. Since the web app uses "http" protocol level exploits are left out in this excercise.
- Once the web app was coded and programmed, unit test cases were written using a library called pytest to maximise the functional testing.
- On completion this web app was put through different kinds of attacks to see if it holds up to XSS , CSRF, Command Injection, Directory traversal exploits.
