#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "convertingfunc.h"
#include<string.h>

#define W_OK 0                   /* wartosc oznaczajaca brak bledow */
#define B_NIEPOPRAWNAOPCJA -1    /* kody bledow rozpoznawania opcji */
#define B_BRAKNAZWY   -2
#define B_BRAKWARTOSCI  -3
#define B_BRAKPLIKU   -4
#define DL_LINII 1024      /* Dlugosc buforow pomocniczych */

int czytaj(tab_obraz *obraz_pgm, t_opcje *wybor) {
  char buf[DL_LINII];      /* bufor pomocniczy do czytania naglowka i komentarzy */
  int znak;                /* zmienna pomocnicza do czytania komentarzy */
  int koniec=0;            /* czy napotkano koniec danych w pliku */
  int i,j;

  /*Sprawdzenie czy podano prawid³owy uchwyt pliku */
  if (wybor->plik_we==NULL) {
    fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");
    return(0);
  }

  /* Sprawdzenie "numeru magicznego" - powinien byæ P2 lub P3 */
  if (fgets(buf,DL_LINII,wybor->plik_we)==NULL)   /* Wczytanie pierwszej linii pliku do bufora */
    koniec=1;                              /* Nie udalo sie? Koniec danych! */

  if ( (buf[0]!='P') || ((buf[1]!='2') && (buf[1]!='3'))|| koniec) {  /* Czy jest magiczne "P2", "P3"? */
    fprintf(stderr,"Blad: To nie jest plik formatu PGM/PPM\n");
    return(0);
  }
  else // ustawiamy typ obrazu na zgodny z odczytanym
  {
    obraz_pgm->typ[0]=buf[0];
    obraz_pgm->typ[1]=buf[1];
  }
  

  /* Pominiecie komentarzy */
  do {
    if ((znak=fgetc(wybor->plik_we))=='#') {         /* Czy linia rozpoczyna sie od znaku '#'? */
      if (fgets(buf,DL_LINII,wybor->plik_we)==NULL)  /* Przeczytaj ja do bufora                */
	koniec=1;                   /* Zapamietaj ewentualny koniec danych */
    }  else {
      ungetc(znak,wybor->plik_we);                   /* Gdy przeczytany znak z poczatku linii */
    }                                         /* nie jest '#' zwroc go                 */
  } while (znak=='#' && !koniec);   /* Powtarzaj dopoki sa linie komentarza */
                                    /* i nie nastapil koniec danych         */

  /* Pobranie wymiarow obrazu i liczby odcieni szarosci */
  if (fscanf(wybor->plik_we,"%d %d %d",&obraz_pgm->wymx,&obraz_pgm->wymy,&obraz_pgm->odcieni)!=3) {
    fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
    return(0);
  }
  /*alokujemy dynamiczna tablice dwuwymiarowa dla pliku pgm*/
  if(obraz_pgm->typ[1] == '2' || wybor->kolor == 's')
  {
    obraz_pgm->pikseli = malloc(obraz_pgm->wymx*obraz_pgm->wymy*sizeof(int));
    int (*pikseli)[obraz_pgm->wymx];
    pikseli=(int(*)[obraz_pgm->wymx]) obraz_pgm->pikseli;

    if(obraz_pgm->typ[1] == '2')
    {
      for(i=0; i<obraz_pgm->wymy; i++)
      {
        for(j=0; j<obraz_pgm->wymx; j++)
        {
          if (fscanf(wybor->plik_we,"%d",&(pikseli[i][j]))!=1) fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
        }
      }
    }
  }
  if(obraz_pgm->typ[1]== '3')
  {
    if(wybor->kolor=='0')
    {
      fprintf(stderr, "Blad: Nie podano koloru przetwarzania!\n");
      return 0;
    }
    obraz_pgm->czerwony = malloc(obraz_pgm->wymx*obraz_pgm->wymy*sizeof(int));
    int (*czerwony)[obraz_pgm->wymx];
    czerwony=(int(*)[obraz_pgm->wymx]) obraz_pgm->czerwony;

    obraz_pgm->zielony = malloc(obraz_pgm->wymx*obraz_pgm->wymy*sizeof(int));
    int (*zielony)[obraz_pgm->wymx];
    zielony=(int(*)[obraz_pgm->wymx]) obraz_pgm->zielony;

    obraz_pgm->niebieski = malloc(obraz_pgm->wymx*obraz_pgm->wymy*sizeof(int));
    int (*niebieski)[obraz_pgm->wymx];
    niebieski=(int(*)[obraz_pgm->wymx]) obraz_pgm->niebieski;

    for(i=0; i<obraz_pgm->wymy; i++)
    {
      for(j=0; j<obraz_pgm->wymx; j++)
      {
        if (fscanf(wybor->plik_we,"%d %d %d",&(czerwony[i][j]), &(zielony[i][j]), &(niebieski[i][j]))!=3) fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
      }
    }
  }
  
  /* Pobranie obrazu i zapisanie w tablicy obraz_pgm*/
  fclose(wybor->plik_we);  //zamykamy plik_we
  return obraz_pgm->wymx*obraz_pgm->wymy;   /* Czytanie zakonczone sukcesem    */
}                       /* Zwroc liczbe wczytanych pikseli */

int wyswietl(t_opcje *wybor) {
  char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */

  if(wybor->nazwa_pliku_wy==NULL || wybor->plik_wy==stdout)
  {
    fprintf(stderr,"Blad: Brak pliku do wyswietlenia!\n"); // asercja dla wyswietlenia
    return -1;
  }
  strcpy(polecenie,"/usr/bin/eog ");  /* konstrukcja polecenia postaci */
  strcat(polecenie,wybor->nazwa_pliku_wy);     /* /usr/bin/eog "nazwa_pliku"        */
  printf("%s\n",polecenie);      /* wydruk kontrolny polecenia */
  system(polecenie);             /* wykonanie polecenia        */
  return 0;
}

int zapisz(tab_obraz *obraz_pgm, t_opcje *wybor)
{
  int i, j;
  fprintf(wybor->plik_wy, "%c%c\n", obraz_pgm->typ[0], obraz_pgm->typ[1]);
  fprintf(wybor->plik_wy, "%d %d\n%d\n", obraz_pgm->wymx, obraz_pgm->wymy, obraz_pgm->odcieni);
  // wpisujemy do pliku wymiary, zakres odcieni i znak P2 lub P3

  if(obraz_pgm->typ[1] == '2' || wybor->kolor == 's')
  {
    int i, j;
    int (*pikseli)[obraz_pgm->wymx];
    pikseli=(int(*)[obraz_pgm->wymx]) obraz_pgm->pikseli;

    for(i=0; i<obraz_pgm->wymy; i++)
    {
      for(j=0; j<obraz_pgm->wymx; j++)
      {
        if(fprintf(wybor->plik_wy, "%d ", pikseli[i][j])<0)
        {
          fprintf(stderr, "Blad zapisu obrazu pgm!\n"); // wpisujemy poszczegolne piksle obrazu z tablicy do pliku
          return 1;
        }
        if((j%15)==1) fprintf(wybor->plik_wy, "\n"); // co 15 znakow robimy nowa linia
      }
    }
  }

  if(obraz_pgm->typ[1] == '3' && wybor->kolor != 's')
  {
    int (*czerwony)[obraz_pgm->wymx];
    czerwony=(int(*)[obraz_pgm->wymx]) obraz_pgm->czerwony;

    int (*zielony)[obraz_pgm->wymx];
    zielony=(int(*)[obraz_pgm->wymx]) obraz_pgm->zielony;

    int (*niebieski)[obraz_pgm->wymx];
    niebieski=(int(*)[obraz_pgm->wymx]) obraz_pgm->niebieski;
    
    for(i=0; i<obraz_pgm->wymy; i++)
    {
      for(j=0; j<obraz_pgm->wymx; j++)
      {
        if(fprintf(wybor->plik_wy, "%d %d %d ", czerwony[i][j], zielony[i][j], niebieski[i][j])<0)
        {
          fprintf(stderr, "Blad zapisu obrazu pgm!\n"); // wpisujemy poszczegolne piksle obrazu z tablicy do pliku
          return 1;
        }
      }
      fprintf(wybor->plik_wy, "\n");
    }
  }

  fclose(wybor->plik_wy);
  return 0;
}

void negatyw(tab_obraz *obraz_pgm, t_opcje *wybor)
{
  int i, j;

  if(obraz_pgm->typ[1] == '2') 
  {
    int (*pikseli)[obraz_pgm->wymx];
    pikseli=(int(*)[obraz_pgm->wymx]) obraz_pgm->pikseli;

    for(i=0; i<obraz_pgm->wymy; i++)
    {
      for(j=0; j<obraz_pgm->wymx; j++)
      {
        pikseli[i][j]=obraz_pgm->odcieni-pikseli[i][j]; //przypisujemy pikselowi roznice max szarosci i jego wartosci
      }
    }
  }

  if(obraz_pgm->typ[1] == '3')
  {
    if(wybor->kolor == 'r')
    {
      int (*czerwony)[obraz_pgm->wymx];
      czerwony=(int(*)[obraz_pgm->wymx]) obraz_pgm->czerwony;
      for(i=0; i<obraz_pgm->wymy; i++)
      {
        for(j=0; j<obraz_pgm->wymx; j++)
        {
          czerwony[i][j]=obraz_pgm->odcieni-czerwony[i][j]; //przypisujemy pikselowi roznice max szarosci i jego wartosci
        }
      }   
    }
    
    if(wybor->kolor == 'g')
    {
      int (*zielony)[obraz_pgm->wymx];
      zielony=(int(*)[obraz_pgm->wymx]) obraz_pgm->zielony;
      for(i=0; i<obraz_pgm->wymy; i++)
      {
        for(j=0; j<obraz_pgm->wymx; j++)
        {
          zielony[i][j]=obraz_pgm->odcieni-zielony[i][j]; //przypisujemy pikselowi roznice max szarosci i jego wartosci
        }
      }   
    }
    
    if(wybor->kolor == 'b')
    {
      int (*niebieski)[obraz_pgm->wymx];
      niebieski=(int(*)[obraz_pgm->wymx]) obraz_pgm->niebieski;
      for(i=0; i<obraz_pgm->wymy; i++)
      {
        for(j=0; j<obraz_pgm->wymx; j++)
        {
          niebieski[i][j]=obraz_pgm->odcieni-niebieski[i][j]; //przypisujemy pikselowi roznice max szarosci i jego wartosci
        }
      }   
    }
  }     
}

void progowanie(tab_obraz *obraz_pgm, t_opcje *wybor)
{
  int i, j;
 if(obraz_pgm->typ[1] == '2') 
  {
    int (*pikseli)[obraz_pgm->wymx];
    pikseli=(int(*)[obraz_pgm->wymx]) obraz_pgm->pikseli;
    for( i=0; i<obraz_pgm->wymy; i++)
    {
      for(j=0; j<obraz_pgm->wymx; j++)
      {
        if(pikseli[i][j]<=wybor->w_progu)
        {
          pikseli[i][j]=0; // dla wartosci piksela mniejszej od progu przypisujemy mu wartosc 0
        }
        else
        {
          pikseli[i][j]=obraz_pgm->odcieni;   // dla wartosci piksela wiekszej od progu przypisujemy mu wartosc max szarosci  
        }
      }
    }
  }

  if(obraz_pgm->typ[1] == '3')
  {
    if(wybor->kolor == 'r')
    {
      int (*czerwony)[obraz_pgm->wymx];
      czerwony=(int(*)[obraz_pgm->wymx]) obraz_pgm->czerwony;
      for( i=0; i<obraz_pgm->wymy; i++)
      {
        for(j=0; j<obraz_pgm->wymx; j++)
        {
          if(czerwony[i][j]<=wybor->w_progu)
          {
            czerwony[i][j]=0; // dla wartosci piksela mniejszej od progu przypisujemy mu wartosc 0
          }
          else
          {
            czerwony[i][j]=obraz_pgm->odcieni;   // dla wartosci piksela wiekszej od progu przypisujemy mu wartosc max szarosci  
          }
        }
      }
    }
    
    if(wybor->kolor == 'g')
    {
      int (*zielony)[obraz_pgm->wymx];
      zielony=(int(*)[obraz_pgm->wymx]) obraz_pgm->zielony;
      for( i=0; i<obraz_pgm->wymy; i++)
      {
        for(j=0; j<obraz_pgm->wymx; j++)
        {
          if(zielony[i][j]<=wybor->w_progu)
          {
            zielony[i][j]=0; // dla wartosci piksela mniejszej od progu przypisujemy mu wartosc 0
          }
          else
          {
            zielony[i][j]=obraz_pgm->odcieni;   // dla wartosci piksela wiekszej od progu przypisujemy mu wartosc max szarosci  
          }
        }
      }
    }
    
    if(wybor->kolor == 'b')
    {
      int (*niebieski)[obraz_pgm->wymx];
      niebieski=(int(*)[obraz_pgm->wymx]) obraz_pgm->niebieski;
      for( i=0; i<obraz_pgm->wymy; i++)
      {
        for(j=0; j<obraz_pgm->wymx; j++)
        {
          if(niebieski[i][j]<=wybor->w_progu)
          {
            niebieski[i][j]=0; // dla wartosci piksela mniejszej od progu przypisujemy mu wartosc 0
          }
          else
          {
            niebieski[i][j]=obraz_pgm->odcieni;   // dla wartosci piksela wiekszej od progu przypisujemy mu wartosc max szarosci  
          }
        }
      }
    }
  }     
}

void konturowanie(tab_obraz *obraz_pgm, t_opcje *wybor)
{
  int i, j, a;

  if(obraz_pgm->typ[1] == '2') 
  {
    int (*pikseli)[obraz_pgm->wymx];
    pikseli=(int(*)[obraz_pgm->wymx]) obraz_pgm->pikseli;

    for(i=0; i<obraz_pgm->wymy; i++)
    {
      for(j=0; j<obraz_pgm->wymx; j++)
      {
        pikseli[i][j]=abs(pikseli[i+1][j]-pikseli[i][j]) + abs(pikseli[i][j+1]-pikseli[i][j]); // obliczmy wartosci pikseli ze wzoru na konturowanie
      }
    }
    obraz_pgm->wymx-=1;  // zmiejszamy wymiary obrazu by pozbyc sie blednie wyliczonych wartosci z ostaniego wiersza i kolumny
    obraz_pgm->wymy-=1;
    obraz_pgm->odcieni*=2;  // zwiekszenie zakresu odcieni by obraz byl czytany poprawnie
    
    int *pikseli2=(int*) malloc(obraz_pgm->wymx*obraz_pgm->wymy*sizeof(int));
    a=0;
    for(i=0; i<obraz_pgm->wymy; i++)
    {
      for(j=0; j<obraz_pgm->wymx; j++)
      {
        pikseli2[a]=pikseli[i][j];
        a++;
      }
    }
    obraz_pgm->pikseli=(void*) pikseli2;
  }
  /****************************************************/
  if(obraz_pgm->typ[1] == '3')
  {
    int (*czerwony)[obraz_pgm->wymx];
    czerwony=(int(*)[obraz_pgm->wymx]) obraz_pgm->czerwony;
    int (*zielony)[obraz_pgm->wymx];
    zielony=(int(*)[obraz_pgm->wymx]) obraz_pgm->zielony;
    int (*niebieski)[obraz_pgm->wymx];
    niebieski=(int(*)[obraz_pgm->wymx]) obraz_pgm->niebieski;
    
    if(wybor->kolor == 'r')
    {
      for(i=0; i<obraz_pgm->wymy; i++)
      {
        for(j=0; j<obraz_pgm->wymx; j++)
        {
          czerwony[i][j]=abs(czerwony[i+1][j]-czerwony[i][j]) + abs(czerwony[i][j+1]-czerwony[i][j]); // obliczmy wartosci pikseli ze wzoru na konturowanie
        }
      }   
    }
    
    if(wybor->kolor == 'g')
    {
      for(i=0; i<obraz_pgm->wymy; i++)
      {
        for(j=0; j<obraz_pgm->wymx; j++)
        {
          zielony[i][j]=abs(zielony[i+1][j]-zielony[i][j]) + abs(zielony[i][j+1]-zielony[i][j]); // obliczmy wartosci pikseli ze wzoru na konturowanie
        }
      }   
    }
    
    if(wybor->kolor == 'b')
    {
      for(i=0; i<obraz_pgm->wymy; i++)
      {
        for(j=0; j<obraz_pgm->wymx; j++)
        {
          niebieski[i][j]=abs(niebieski[i+1][j]-niebieski[i][j]) + abs(niebieski[i][j+1]-niebieski[i][j]); // obliczmy wartosci pikseli ze wzoru na konturowanie
        }
      }   
    }
    obraz_pgm->wymx-=1;  // zmiejszamy wymiary obrazu by pozbyc sie blednie wyliczonych wartosci z ostaniego wiersza i kolumny
    obraz_pgm->wymy-=1;
    obraz_pgm->odcieni*=2;  // zwiekszenie zakresu odcieni by obraz byl czytany poprawnie
    
    int *czerwony2=(int*) malloc(obraz_pgm->wymx*obraz_pgm->wymy*sizeof(int));
    int *zielony2=(int*) malloc(obraz_pgm->wymx*obraz_pgm->wymy*sizeof(int));
    int *niebieski2=(int*) malloc(obraz_pgm->wymx*obraz_pgm->wymy*sizeof(int));
    a=0;
    for(i=0; i<obraz_pgm->wymy; i++)
    {
      for(j=0; j<obraz_pgm->wymx; j++)
      {
        czerwony2[a]=czerwony[i][j];
        a++;
      }
    }
    obraz_pgm->czerwony=(void*) czerwony2;
    a=0;
    for(i=0; i<obraz_pgm->wymy; i++)
    {
      for(j=0; j<obraz_pgm->wymx; j++)
      {
        zielony2[a]=zielony[i][j];
        a++;
      }
    }
    obraz_pgm->zielony=(void*) zielony2;
    a=0;
    for(i=0; i<obraz_pgm->wymy; i++)
    {
      for(j=0; j<obraz_pgm->wymx; j++)
      {
        niebieski2[a]=niebieski[i][j];
        a++;
      }
    }
    obraz_pgm->niebieski=(void*) niebieski2;
  }
}

void wyzeruj_opcje(t_opcje *wybor) {
  wybor->plik_we=NULL;
  wybor->plik_wy=NULL;
  wybor->negatyw=0;
  wybor->konturowanie=0;
  wybor->progowanie=0;
  wybor->wyswietl=0;
  wybor->nazwa_pliku_wy=NULL;
  wybor->w_progu=0;
  wybor->kolor='0';
}

int przetwarzaj_opcje(int argc, char **argv, t_opcje *wybor) {
  int i, prog;
  char *nazwa_pliku_we, *nazwa_pliku_wy;
  char kolor;

  wyzeruj_opcje(wybor);
  wybor->plik_wy=stdout;        /* na wypadek gdy nie podano opcji "-o" */

  for (i=1; i<argc; i++) {
    if (argv[i][0] != '-')  /* blad: to nie jest opcja - brak znaku "-" */
      return B_NIEPOPRAWNAOPCJA; 
    switch (argv[i][1]) {
    case 'i': {                 /* opcja z nazwa pliku wejsciowego */
      if (++i<argc) {   /* wczytujemy kolejny argument jako nazwe pliku */
	nazwa_pliku_we=argv[i];
	if (strcmp(nazwa_pliku_we,"-")==0) /* gdy nazwa jest "-"        */
	  wybor->plik_we=stdin;            /* ustwiamy wejscie na stdin */
	else                               /* otwieramy wskazany plik   */
	  wybor->plik_we=fopen(nazwa_pliku_we,"r");
      } else 
	return B_BRAKNAZWY;                   /* blad: brak nazwy pliku */
      break;
    }
    case 'o': {                 /* opcja z nazwa pliku wyjsciowego */
      if (++i<argc) {   /* wczytujemy kolejny argument jako nazwe pliku */
	nazwa_pliku_wy=argv[i];
	if (strcmp(nazwa_pliku_wy,"-")==0)/* gdy nazwa jest "-"         */
	  wybor->plik_wy=stdout;          /* ustwiamy wyjscie na stdout */
	else                              /* otwieramy wskazany plik    */
	  wybor->plik_wy=fopen(nazwa_pliku_wy,"w");
    wybor->nazwa_pliku_wy=nazwa_pliku_wy;
      } else 
	return B_BRAKNAZWY;                   /* blad: brak nazwy pliku */
      break;
    }
    case 'p': {
      if (++i<argc) { /* wczytujemy kolejny argument jako wartosc progu */
	if (sscanf(argv[i],"%d",&prog)==1) {
	  wybor->progowanie=1;
	  wybor->w_progu=prog;
	} else
	  return B_BRAKWARTOSCI;     /* blad: niepoprawna wartosc progu */
      } else 
	return B_BRAKWARTOSCI;             /* blad: brak wartosci progu */
      break;
    }
    case 'n': {                 /* mamy wykonac negatyw */
      wybor->negatyw=1;
      break;
    }
    case 'k': {                 /* mamy wykonac konturowanie */
      wybor->konturowanie=1;
      break;
    }
    case 'd': {                 /* mamy wyswietlic obraz */
      wybor->wyswietl=1;
      break;
    }
    case 'm' : {
      if (++i<argc) { /* wczytujemy kolejny argument jako kolor przetwarzania */
	if (sscanf(argv[i],"%c",&kolor)==1) {
	  wybor->kolor=kolor;
    if(kolor!='r' || kolor!='g' || kolor!='b' || kolor!='s') return B_NIEPOPRAWNAOPCJA;
	} else
	  return B_BRAKWARTOSCI;     /* blad: niepoprawna wartosc progu */
      } else 
	return B_BRAKWARTOSCI;             /* blad: brak wartosci progu */
      break;
    }
    default:                    /* nierozpoznana opcja */
      return B_NIEPOPRAWNAOPCJA;
    } /*koniec switch */
  } /* koniec for */

  if (wybor->plik_we!=NULL || wybor->plik_we==stdin)     /* ok: wej. strumien danych zainicjowany */
    return W_OK;
  else 
    return B_BRAKPLIKU;         /* blad:  nie otwarto pliku wejsciowego  */
}

int szarosc(tab_obraz *obraz_pgm, t_opcje *wybor)
{ 
  int i,j;
  int (*pikseli)[obraz_pgm->wymx];
  pikseli=(int(*)[obraz_pgm->wymx]) obraz_pgm->pikseli;
  int (*czerwony)[obraz_pgm->wymx];
  czerwony=(int(*)[obraz_pgm->wymx]) obraz_pgm->czerwony;
  int (*zielony)[obraz_pgm->wymx];
  zielony=(int(*)[obraz_pgm->wymx]) obraz_pgm->zielony;
  int (*niebieski)[obraz_pgm->wymx];
  niebieski=(int(*)[obraz_pgm->wymx]) obraz_pgm->niebieski;
  if(obraz_pgm->typ[1]!='3') return -1;

  for( i=0; i<obraz_pgm->wymy; i++)
  {
    for(j=0; j<obraz_pgm->wymx; j++)
    {
      pikseli[i][j]= (czerwony[i][j]+zielony[i][j]+niebieski[i][j])/3;
    }
  }
  obraz_pgm->typ[1]='2';
  return 0;
}
