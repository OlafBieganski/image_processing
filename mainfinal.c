#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "convertingfunc.h"

#define BLAD 1

int main(int argc, char **argv)
{
    t_opcje wybor1;
    tab_obraz obraz1;
    t_opcje *wybor=&wybor1;
    tab_obraz *obraz=&obraz1;
    int a;

    a=przetwarzaj_opcje(argc, argv, wybor);
    switch(a)
    {
        case -1:
        fprintf(stderr,"Blad: Program wywolany niepoprawnie!\n");
        return BLAD;
        break;

        case -2:
        fprintf(stderr,"Blad: Brak nazwy pliku!\n");
        return BLAD;
        break;

        case -3:
        fprintf(stderr,"Blad: Brak wartosci progu lub koloru przetwarzania!\n");
        return BLAD;
        break;

        case -4:
        fprintf(stderr,"Blad: Plik o podanej nazwie nie istnieje!\n");
        return BLAD;
        break;

        default:
        break;
    }
    
    if(czytaj(obraz, wybor)<1) return BLAD;
    if(wybor->negatyw==1) negatyw(obraz, wybor);
    if(wybor->progowanie==1) progowanie(obraz, wybor);
    if(wybor->konturowanie==1) konturowanie(obraz, wybor);
    if(wybor->kolor=='s') szarosc(obraz, wybor);
    if(zapisz(obraz, wybor)!=0) return BLAD;
    if(wybor->wyswietl==1) wyswietl(wybor);
    return 0;
}
