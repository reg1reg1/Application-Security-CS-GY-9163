default: prog



get-deps:
	# Assuming Debian or Ubuntu here
	sudo apt-get install -y build-essential check

dictionary.o: dictionary.c
	gcc -Wall -c dictionary.c dictionary.h -g

spell.o: spell.c
	gcc -Wall -c spell.c -g

test.o: test_main.c
	gcc -Wall -c test_main.c -g

main.o: main.c
	gcc -Wall -c main.c -g

test: dictionary.o spell.o test_main.o
	gcc -Wall -o test_main test_main.o spell.o dictionary.o -lcheck -lm -lrt -lpthread -lsubunit
	./test_main

prog: dictionary.o spell.o main.o
	gcc -Wall -o spell_check dictionary.o spell.o main.o -g

clean:
	rm dictionary.o spell.o main.o test_main.o check_spell.o

cleanall:clean
	rm spell_check
	
