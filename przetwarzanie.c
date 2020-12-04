#include <stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX 512            /* Maksymalny rozmiar wczytywanego obrazu */
#define DL_LINII 1024      /* Dlugosc buforow pomocniczych */


int czytaj(FILE *plik_we,int obraz_pgm[][MAX],int *wymx,int *wymy, int *szarosci) {
  char buf[DL_LINII];      /* bufor pomocniczy do czytania naglowka i komentarzy */
  int znak;                /* zmienna pomocnicza do czytania komentarzy */
  int koniec=0;            /* czy napotkano koniec danych w pliku */
  int i,j;

  /*Sprawdzenie czy podano prawid³owy uchwyt pliku */
  if (plik_we==NULL) {
    fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");
    return(0);
  }

  /* Sprawdzenie "numeru magicznego" - powinien byæ P2 */
  if (fgets(buf,DL_LINII,plik_we)==NULL)   /* Wczytanie pierwszej linii pliku do bufora */
    koniec=1;                              /* Nie udalo sie? Koniec danych! */

  if ( (buf[0]!='P') || (buf[1]!='2') || koniec) {  /* Czy jest magiczne "P2"? */
    fprintf(stderr,"Blad: To nie jest plik PGM\n");
    return(0);
  }

  /* Pominiecie komentarzy */
  do {
    if ((znak=fgetc(plik_we))=='#') {         /* Czy linia rozpoczyna sie od znaku '#'? */
      if (fgets(buf,DL_LINII,plik_we)==NULL)  /* Przeczytaj ja do bufora                */
	koniec=1;                   /* Zapamietaj ewentualny koniec danych */
    }  else {
      ungetc(znak,plik_we);                   /* Gdy przeczytany znak z poczatku linii */
    }                                         /* nie jest '#' zwroc go                 */
  } while (znak=='#' && !koniec);   /* Powtarzaj dopoki sa linie komentarza */
                                    /* i nie nastapil koniec danych         */

  /* Pobranie wymiarow obrazu i liczby odcieni szarosci */
  if (fscanf(plik_we,"%d %d %d",wymx,wymy,szarosci)!=3) {
    fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
    return(0);
  }
  /* Pobranie obrazu i zapisanie w tablicy obraz_pgm*/
  for (i=0;i<*wymy;i++) {
    for (j=0;j<*wymx;j++) {
      if (fscanf(plik_we,"%d",&(obraz_pgm[i][j]))!=1) {
	fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
	return(0);
      }
    }
  }
  return *wymx**wymy;   /* Czytanie zakonczone sukcesem    */
}                       /* Zwroc liczbe wczytanych pikseli */

/* Wyswietlenie obrazu o zadanej nazwie za pomoca programu "display"   */
void wyswietl(char *n_pliku) {
  char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */

  strcpy(polecenie,"display ");  /* konstrukcja polecenia postaci */
  strcat(polecenie,n_pliku);     /* display "nazwa_pliku" &       */
  strcat(polecenie," &");
  printf("%s\n",polecenie);      /* wydruk kontrolny polecenia */
  system(polecenie);             /* wykonanie polecenia        */
}

/*funkcja zapisuje przetworzony obraz do pliku pgm*/
int zapisz(int obraz[][MAX], int wymx, int wymy, int odcieni)
{
  int i, j;
  FILE *plikz;
  
  plikz= fopen("plik.pgm", "w");
  fprintf(plikz, "P2\n");
  fprintf(plikz, "%d %d\n%d\n", wymx, wymy, odcieni);
  // wpisujemy do pliku wymiary, max szrosci i znak P2

  for(i=0; i<wymy; i++)
        {
            for(j=0; j<wymx; j++)
            {
              fprintf(plikz, "%d ", obraz[i][j]); // wpisujemy poszczegolne piksle obrazu z tablicy do pliku
            }                            
        }

  fclose(plikz);
  return 0;
}

/*funkcja wykonuje negatyw obrazu*/
void negatyw(int obraz[][MAX], int wymx, int wymy, int odcieni)  // wykonuje negatyw i na tablicy 
{      
       int i, j;
        
        for(i=0; i<wymy; i++)
        {
            for(j=0; j<wymx; j++)
            {
            obraz[i][j]=odcieni-obraz[i][j]; //przypisujemy pikselowi roznice max szarosci i jego wartosci
            }                             
        }
}

/*funkcja wykonuje operacje progowania*/
void progowanie(int obraz[][MAX], int wymx, int wymy, int odcieni)
{
  int i, j, prog=0, procent=0;

  printf("Podaj wartosc progu w procentach: ");
  scanf("%d", &procent);
  prog =(odcieni*procent)/100;
  // podajemy wartosc progu w % i zamieniamy na wartosc progu w postaci liczby calkowitej

  for( i=0; i<wymy; i++)
  {
    for(j=0; j<wymx; j++)
    {
      if(obraz[i][j]<=prog)
      {
        obraz[i][j]=0; // dla wartosci piksela mniejszej od progu przypisujemy mu wartosc 0
      }
      else
      {
        obraz[i][j]=odcieni;   // dla wartosci piksela wiekszej od progu przypisujemy mu wartosc max szarosci  
      }
    }
  }
}

/*funkcja stworzona na potrzebe operacji na poszczegolnych pikselach w fukcji konturowanie
tworzy ona kopie tablicy obraz*/
void kopiuj(int kopia[][MAX], int obraz[][MAX], int wymx, int wymy)  // wypisuje elementy tablicy
{
    int i, j;
    for(i=0; i<wymy; i++)
    {
        for(j=0; j<wymx; j++)
        {
          kopia[i][j]=obraz[i][j]; // przepisujemy wartosci z tablicy obraz do tablicy kopia
        }
    }
}

/*funkcja wykonuje operacje konturowania*/
void konturowanie(int obraz[][MAX], int wymx, int wymy)
{
  int i, j, kopia[MAX][MAX];

  kopiuj(kopia, obraz, wymx, wymy); // tworzymy kopie tablicy obraz
  
  for( i=0; i<wymy; i++)
  {
    for(j=0; j<wymx; j++)
    {
      if(i==wymx) // dla ostatniego wiersza
      {
        if(j!=wymy) // dla wszystkich kolumn oprocz ostatniej
        {
          obraz[i][j]=abs(kopia[i-1][j]-kopia[i][j]) + abs(kopia[i][j+1]-kopia[i][j]); 
        }
        else // dla ostatniej kolumny
        {
          obraz[i][j]=abs(kopia[i-1][j]-kopia[i][j]) + abs(kopia[i][j-1]-kopia[i][j]);
        }
        
      }
      else // dla wszystkich wierszy oprocz ostatniego
      {
        if(j!=wymy) // dla wszystkich kolumn oprocz ostatniej
        {
          obraz[i][j]=abs(kopia[i+1][j]-kopia[i][j]) + abs(kopia[i][j+1]-kopia[i][j]);
        }
        else // dla ostatniej kolumny
        {
          obraz[i][j]=abs(kopia[i+1][j]-kopia[i][j]) + abs(kopia[i][j-1]-kopia[i][j]);
        }
        
      }
      
    }
  }
}

/*funkcja wyswietla poszczegolne piksele obrazu w formacie tekstowym*/
void ftekstowy( int obraz[][MAX], int wymx, int wymy)
{
    int i, j;
    for(i=0; i<wymy; i++)
    {
        for(j=0; j<wymx; j++)
        {
            printf("%d\t", obraz[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    int opcja ;
    int obraz[MAX][MAX] ;
    int wymx,wymy,odcieni;

    int odczytano = 0;
    FILE *plik;
    char nazwa[100];

    while(opcja!=8)
    {
        printf("\nMenu:\n");
        printf("\t1 - Wczytaj plik\n");
        printf("\t2 - Wyswietl wczytany obraz\n");
        printf("\t3 - Wyswietl w formacie tekstowym\n");
        printf("\t4 - Negatyw\n");
        printf("\t5 - Zapisz do pliku\n");
        printf("\t6 - Progowanie\n");
        printf("\t7 - Konturowanie\n");
        printf("\t8 - Zakończ działanie\n");
        printf("Twój wybór: ");
        scanf("%d", &opcja);

        switch(opcja)
        {
            case 1:
            /* Wczytanie zawartosci wskazanego pliku do pamieci */
            printf("Podaj nazwe pliku:\n");
            scanf("%s",nazwa);
            plik=fopen(nazwa,"r");

            if (plik != NULL) {       /* co spowoduje zakomentowanie tego warunku */
            odczytano = czytaj(plik,obraz,&wymx,&wymy,&odcieni);
            fclose(plik);
            }
            else
            {
              printf("Blad! Nie ma takiego pliku."); // gy podano nieistniejacy plik
            }
            
            break;

            case 2:
            if (odczytano != 0)
            wyswietl(nazwa);  // wyswietla obraz
            break;

            case 3:
            ftekstowy(obraz, wymx, wymy); // wyswietla obraz w formacie tekstowym
            break;

            case 4:
            negatyw( obraz, wymx, wymy, odcieni); // negatyw
            break;

            case 5:
            zapisz(obraz,wymx, wymy, odcieni); // zapis do pliku
            break;

            case 6:
            progowanie(obraz, wymx, wymy, odcieni); // progowanie
            break;

            case 7:
            konturowanie(obraz, wymx, wymy); // konturowanie
            break;

            case 8:
            printf("koniec dzialania\n");
            break;

            default:
            printf("Nie ma takiej opcji!\n");
        }
    }
    return 0;
}