lab4:	lab4.o
	gcc -o lab4 lab4.o

lab4.o:	lab4.c
	gcc  -g -c -o lab4.o lab4.c

clean:
	rm *.o lab4