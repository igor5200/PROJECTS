//PS IS1 324 LAB03
//Igor Stodolny
//si46655@zut.edu.pl
main : libshowgroup.a 46655.ps.lab03.static.main.c
	gcc 46655.ps.lab03.static.main.c -o main -L. -lshowgroup

libshowgroup.a : 46655.ps.lab03.static.lib.o
	ar cr libshowgroup.a 46655.ps.lab03.static.lib.o

46655.ps.lab03.static.lib.o : 46655.ps.lab03.static.lib.c
	gcc -c  46655.ps.lab03.static.lib.c
