assembler :  assembler.o firstOver.o secondOver.o node.o files.o
	gcc -g -ansi -Wall -pedantic assembler.o  firstOver.o secondOver.o node.o files.o -o assembler

assembler.o :assembler.c global.h var.h database.h prototype.h 
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o

firstOver.o: firstOver.c var.h database.h  prototype.h
	gcc -c -ansi -Wall -pedantic firstOver.c -o firstOver.o


secondOver.o: secondOver.c var.h database.h  prototype.h
	gcc -c -ansi -Wall -pedantic secondOver.c -o secondOver.o

node.o:node.c var.h database.h prototype.h
	gcc -c -ansi -Wall -pedantic node.c -o node.o

files.o:files.c var.h database.h prototype.h
	gcc -c -ansi -Wall -pedantic files.c -o files.o


 
