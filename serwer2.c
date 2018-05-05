#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>


void ObsluzPolaczenie(int gn)
{
    char sciezka[512];
    long dl_pliku, wyslano, wyslano_razem, przeczytano;
    struct stat fileinfo;
    FILE* plik;
    unsigned char bufor[1024];
    
    memset(sciezka, 0, 512) = "bilety.txt";
     sciezka = "bilety.txt";
    if ( sciezka <= 0)
    {
        printf("Potomny: blad przy odczycie sciezki\n");
        return;
    }
    
    printf("Potomny: klient chce plik %s\n", sciezka);
    
    if (stat(sciezka, &fileinfo) < 0)
    {
        printf("Potomny: nie moge pobrac informacji o pliku\n");
        return;
    }
    
    if (fileinfo.st_size == 0)
    {
        printf("Potomny: rozmiar pliku 0\n");
        return;
    }
    
    printf("Potomny: dlugosc pliku: %d\n", fileinfo.st_size);
    
    dl_pliku = htonl((long) fileinfo.st_size);
    
    if (send(gn, &dl_pliku, sizeof(long), 0) != sizeof(long))
    {
        printf("Potomny: blad przy wysylaniu wielkosci pliku\n");
        return;
    }
    
    dl_pliku = fileinfo.st_size;
    wyslano_razem = 0;
    plik = fopen(sciezka, "rb");
    if (plik == NULL)
    {
        printf("Potomny: blad przy otwarciu pliku\n");
        return;
    }
    
    while (wyslano_razem < dl_pliku)
    {
        przeczytano = fread(bufor, 1, 1024, plik);
        wyslano = send(gn, bufor, przeczytano, 0);
        if (przeczytano != wyslano)
            break;
        wyslano_razem += wyslano;
        printf("Potomny: wyslano %d bajtow\n", wyslano_razem);
    }
    
    if (wyslano_razem == dl_pliku)
        printf("Potomny: plik wyslany poprawnie\n");
    else
        printf("Potomny: blad przy wysylaniu pliku\n");
    fclose(plik);
    return;    
}

int main (void) {
  unsigned int port;
  char bufor[1024], wiadomosc[1024];
  char * miasta[6] = {"Berlin","Warszawa","Madryt","Poznań","Londyn","Moskwa"};
  int gniazdo, gniazdo2;
  struct sockaddr_in adr, nadawca;
  socklen_t dl = sizeof(struct sockaddr_in);

  FILE *f = fopen("bilety.txt", "w");
  if (f == NULL) {
    printf("%s\n", "Nieotwarto pliku");
    exit(1);
  }
  for (int i = 0; i < 6; i++) {
    fprintf(f, "%s\n", miasta[rand()%6]);
    fprintf(f, "%s\n", miasta[rand()%6]);
    fprintf(f, "%u.%u.%u %u:%u\n", rand()%30,rand()%12,2018,rand()%24,rand()%59);
    fprintf(f, "%u:%u\n", rand()%8,rand()%59);
    fprintf(f, "R: %u M: %u\n", rand()%16,rand()%8);
    fprintf(f, "%u\n", rand()%300+50);
  }
  fclose(f);
  
  printf("%s\n", "Podaj wiadomosc dla klientow");

  fflush(stdout);
  fgets(wiadomosc, 1024, stdin);

  printf("Podaj port : \n");
  scanf("%u", &port);
  gniazdo = socket(PF_INET, SOCK_STREAM, 0);
  adr.sin_family = AF_INET;
  adr.sin_port = htons(port);
  adr.sin_addr.s_addr = INADDR_ANY;
  if (bind(gniazdo, (struct sockaddr*) &adr, sizeof(adr))<0) {
    printf("Bind nie powiódł się.\n");
    return 1;
  }
  if (listen(gniazdo, 10) < 0) {
    printf("%s\n", "Listen nie powiódł się.\n");
    return 1;
  }

  printf("%s\n","Czekam na połączenie \n");


  while ((gniazdo2 = accept(gniazdo, (struct sockaddr*) &nadawca, &dl)) > 0) {
    memset(bufor, 0, 1024);
    recv(gniazdo2, bufor, 1024, 0);
    printf("Wiadomość od %s: %s\n", inet_ntoa(nadawca.sin_addr), bufor);
    if (fork() == 0)
        {
            /* proces potomny */
            printf("Potomny: zaczynam obsluge\n");
            ObsluzPolaczenie(gniazdo2);
            printf("Potomny: zamykam gniazdo\n");
            close(gniazdo2);
            printf("Potomny: koncze proces\n");
            exit(0);
        }
    printf("%s\n", "Wiadomosc wyslana!\n");
  }
  close(gniazdo);

  return 0;
}
