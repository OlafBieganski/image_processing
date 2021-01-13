#ifndef CONVERTINGFUNC_H
#define CONVERTINGFUNC_H

#define W_OK 0                   /* wartosc oznaczajaca brak bledow */
#define B_NIEPOPRAWNAOPCJA -1    /* kody bledow rozpoznawania opcji */
#define B_BRAKNAZWY   -2
#define B_BRAKWARTOSCI  -3
#define B_BRAKPLIKU   -4
#define DL_LINII 1024      /* Dlugosc buforow pomocniczych */

/* struktura przegowujca obraz .pgm*/
typedef struct 
{
  char typ[2];
  int wymx;
  int wymy;
  int odcieni;
  void *pikseli, *czerwony, *zielony, *niebieski;
} tab_obraz;

/* strukura do zapamietywania opcji podanych w wywolaniu programu */
typedef struct {
  FILE *plik_we, *plik_wy;        /* uchwyty do pliku wej. i wyj. */
  int negatyw,progowanie,konturowanie,wyswietl;      /* opcje */
  int w_progu;                    /* wartosc progu dla opcji progowanie */
  char kolor;
  char *nazwa_pliku_wy;               
} t_opcje;

/*******************************************************/
/* Funkcja inicjuje strukture wskazywana przez wybor   */
/* PRE:                                                */
/*      poprawnie zainicjowany argument wybor (!=NULL) */
/* POST:                                               */
/*      "wyzerowana" struktura wybor wybranych opcji   */
/*******************************************************/
void wyzeruj_opcje(t_opcje * wybor);

/************************************************************************/
/* Funkcja rozpoznaje opcje wywolania programu zapisane w tablicy argv  */
/* i zapisuje je w strukturze wybor                                     */
/* Skladnia opcji wywolania programu                                    */
/*         program {[-i nazwa] [-o nazwa] [-p liczba] [-n] [-r] [-d] }  */
/* Argumenty funkcji:                                                   */
/*         argc  -  liczba argumentow wywolania wraz z nazwa programu   */
/*         argv  -  tablica argumentow wywolania                        */
/*         wybor -  struktura z informacjami o wywolanych opcjach       */
/* PRE:                                                                 */
/*      brak                                                            */
/* POST:                                                                */
/*      funkcja otwiera odpowiednie pliki, zwraca uchwyty do nich       */
/*      w strukturze wyb�r, do tego ustawia na 1 pola dla opcji, ktore  */
/*	poprawnie wystapily w linii wywolania programu,                 */
/*	pola opcji nie wystepujacych w wywolaniu ustawione sa na 0;     */
/*	zwraca wartosc W_OK (0), gdy wywolanie bylo poprawne            */
/*	lub kod bledu zdefiniowany stalymi B_* (<0)                     */
/* UWAGA:                                                               */
/*      funkcja nie sprawdza istnienia i praw dostepu do plikow         */
/*      w takich przypadkach zwracane uchwyty maja wartosc NULL         */
/************************************************************************/

int przetwarzaj_opcje(int argc, char **argv, t_opcje *wybor);

/************************************************
 * Funkcja wykonuje negatyw obrazu              *
 * **********************************************
 * Parametry:                                   *
 * obraz_pgm- struktura danych na ktora skladaja*
 * sie:                                         *
 * pikseli- wskaznik na pierwszy element obrazu *
 * (alokowana w programie dynamiczna tablice    *
 * rzutowana do int)                            *
 * wymx- szerokosc obrazu                       *
 * wymy- wysokosc obrazu                        *
 * odcieni- ilosc odcieni szarosci              *
 ************************************************/
void negatyw(tab_obraz* obraz_pgm, t_opcje *wybor);

/**********************************************************
 * Funkcja wykonuje operacje progowania dla podanego progu*
 * ********************************************************
 * Parametry:
 *  obraz_pgm- struktura danych na ktora skladaja         *
 * sie:                                                   *
 * pikseli- wskaznik na pierwszy element obrazu           *
 * (alokowana w programie dynamiczna tablice              *
 * rzutowana do int)                                      * 
 * wymx- szerokosc obrazu                                 *
 * wymy- wysokosc obrazu                                  *
 * odcieni- ilosc odcieni szarosci                        *
 * prog- wartosc progu dlaoperacji progowania             *
 * ********************************************************
 * PRE:                                                   *
 * -wartosc progu musi byc w                              *
 * przedziale <0;100>                                     *
 * ********************************************************/
void progowanie(tab_obraz* obraz_pgm, t_opcje *wybor);

/**********************************************************
 * Funkcja wykonuje operacje konturowania                 *
 * ********************************************************
 * Parametry:                                             *
 * *obraz_pgm- wskaznik na strukture danych na ktora      *
 * skladaja  sie:                                         *
 * pikseli- wskaznik na pierwszy element obrazu           *
 * (alokowana w programie dynamiczna tablice              *
 * rzutowana do int)                                      * 
 * wymx- szerokosc obrazu                                 *
 * wymy- wysokosc obrazu                                  *
 * odcieni- ilosc odcieni szarosci                        *
 * ********************************************************/
void konturowanie(tab_obraz* obraz_pgm, t_opcje *wybor);
// funkcja jako parametr przyjmuje wskaznik

int czytaj(tab_obraz* obraz_pgm, t_opcje* wybor);

int wyswietl(t_opcje *wybor);

int zapisz(tab_obraz *obraz_pgm, t_opcje *wybor);

int szarosc(tab_obraz *obraz_pgm, t_opcje *wybor);

void ftekstowy(tab_obraz *obraz_pgm);


#endif
