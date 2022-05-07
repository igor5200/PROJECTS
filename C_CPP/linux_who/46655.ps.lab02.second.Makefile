main: 46655.ps.lab02.second.main.o 46655.ps.lab02.second.lib.o
	gcc 46655.ps.lab02.second.main.o 46655.ps.lab02.second.lib.o -o main


46655.ps.lab02.second.lib.o : 46655.ps.lab02.second.lib.c
	gcc -c 46655.ps.lab02.second.lib.c


46655.ps.lab02.second.main.o : 46655.ps.lab02.second.main.c
	gcc -c 46655.ps.lab02.second.main.c
