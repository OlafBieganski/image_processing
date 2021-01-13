
COPTS=  -g
CC= gcc
HEADERS= convertingfunc.h

all: przetw_obr
convertingfunc.o: convertingfunc.c $(HEADERS)
	$(CC) -Wall $ $(COPTS) -c convertingfunc.c

mainfinal.o: mainfinal.c $(HEADERS)
	$(CC) -Wall $(COPTS) -c mainfinal.c

przetw_obr: mainfinal.o convertingfunc.o
	gcc -o przetw_obr mainfinal.o convertingfunc.o

clean:
	rm *.o *~
