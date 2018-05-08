#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

void ObsluzPolaczenie (int gniazdo) {
  char * sciezka = "bilety.txt";
  long dl_pliku, wyslano, wyslano_razem, przeczytano;
  struct stat fileinfo;
  FILE* plik; //file
  unsigned char bufor[1024];

  if (stat(sciezka, &fileinfo) < 0)
  {
printf("Potomny: nie moge pobrac informacji o pliku\n(Child: I can't obtain information about a file)\n");
return;
  }

  if (fileinfo.st_size == 0)
  {
printf("Potomny: rozmiar pliku (child: filesize) 0\n");
return;
  }

  printf("Potomny: dlugosc pliku (child: filesize): %ld\n", fileinfo.st_size);

  dl_pliku = htonl((long) fileinfo.st_size);

  if (send(gniazdo, &dl_pliku, sizeof(long), 0) != sizeof(long))
  {
printf("Potomny: blad przy wysylaniu wielkosci pliku (error at sending a file)\n");
return;
  }

  dl_pliku = fileinfo.st_size;
  wyslano_razem = 0;
  plik = fopen(sciezka, "rb");
  if (plik == NULL)
  {
printf("Potomny: blad przy otwarciu pliku (error at opening a file)\n");
return;
  }

  while (wyslano_razem < dl_pliku)
  {
    przeczytano = fread(bufor, 1, 1024, plik);
    wyslano = send(gniazdo, bufor, przeczytano, 0); //sent, buffer, read
    if (przeczytano != wyslano) //read (received) !=sent
      break;
    wyslano_razem += wyslano;
    printf("Potomny: wyslano %ld bajtow (total sent)\n", wyslano_razem);
  }

  if (wyslano_razem == dl_pliku)
    printf("Potomny: plik wyslany poprawnie (child: file sent ok)\n");
  else
    printf("Potomny: blad przy wysylaniu pliku (child: file sending error)\n"); //error at se
  fclose(plik);
  return;
}

int main (void) {
  unsigned int port;
  char bufor[1024];
  char * miasta[20] = {"Berlin","Warszawa","Madryt","Poznań","Londyn","Moskwa","Zbąszynek","San Francisco", "Paryż", "Kraków", "Budapeszt", "Rzym", "Sztokholm", "Dublin", "Tallin", "Tokio", "Nowy Jork", "Lizbona", "Ateny", "Wilno"};
  int gnasluch, gklient, iloscBiletow;
  long odebrano, rozmDanych;
  struct sockaddr_in adr;
  socklen_t dl = sizeof(struct sockaddr_in);

  size_t len = sizeof(miasta) / sizeof(miasta[0]);

  iloscBiletow = 60;

  FILE *f = fopen("bilety.txt", "w");
  if (f == NULL) {
    printf("%s\n", "Nieotwarto pliku");
    exit(1);
  }
  for (int i = 0; i < iloscBiletow; i++) {
    int idmiasto = rand()%len;
    int idmiasto2 = rand()%len;
    while(idmiasto==idmiasto2)
    {
      idmiasto2 = rand()%len;
    }
    fprintf(f, "%s", miasta[idmiasto]);
    fprintf(f, "%s", "#");
    fprintf(f, "%s", miasta[idmiasto2]);
    fprintf(f, "%s", "#");
    fprintf(f, "%u.%u.%u %u:%u", rand()%29 + 1,rand()%11+1,2018,rand()%24,rand()%59);
    fprintf(f, "%s", "#");
    fprintf(f, "%u:%u", rand()%8,rand()%59);
    fprintf(f, "%s", "#");
    fprintf(f, "R: %u M: %u", rand()%15+1,rand()%8+1);
    fprintf(f, "%s", "#");
    fprintf(f, "%u", rand()%300+50);
    fprintf(f, "%s", "#");
  }
  fclose(f);

  printf("Podaj port : \n");
  scanf("%u", &port);
  gnasluch = socket(PF_INET, SOCK_STREAM, 0);
  adr.sin_family = AF_INET;
  adr.sin_port = htons(port);
  adr.sin_addr.s_addr = INADDR_ANY;
  memset(adr.sin_zero, 0, sizeof(adr.sin_zero));

  if (bind(gnasluch, (struct sockaddr*) &adr, sizeof(adr))<0) {
    printf("Bind nie powiódł się.\n");
    return 1;
  }
  listen(gnasluch, 10);

  while(1)
  {
      dl = sizeof(struct sockaddr_in);
      gklient = accept(gnasluch, (struct sockaddr*) &adr, &dl);
      if (gklient < 0)
      {
          printf("Glowny: accept zwrocil blad\n");
          continue;
      }
      printf("Glowny: polaczenie od %s:%u\n",
          inet_ntoa(adr.sin_addr),
          ntohs(adr.sin_port)
          );
      printf("Glowny: tworze proces potomny\n");
      if (fork() == 0)
      {
          /* proces potomny */
          printf("Potomny: zaczynam obsluge\n");
          ObsluzPolaczenie(gklient);
          odebrano = recv(gklient, bufor, 1024, 0);
          FILE *o = fopen(inet_ntoa(adr.sin_addr), "w");
          fprintf(o, "Dane z adresu : %s:%u\n", inet_ntoa(adr.sin_addr), ntohs(adr.sin_port));
          fputs(bufor, o);
          printf("Potomny: zamykam gniazdo\n");
          close(gklient);
          printf("Potomny: koncze proces\n");
          exit(0);
      }
      else
      {
          /* proces macierzysty */
          printf("Glowny: wracam do nasluchu\n");
          continue;
      }
    }
/*
  while ((gniazdo2 = accept(gniazdo, (struct sockaddr*) &nadawca, &dl)) > 0) {
    WyslijBilety(gniazdo2);
    printf("Wyslano bilety\n");
    close(gniazdo2);
  }
  close(gniazdo);
*/
  return 0;
}
