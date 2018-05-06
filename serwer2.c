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

void WyslijBilety (int gniazdo) {
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
  char bufor[1024], wiadomosc[1024];
  char * miasta[6] = {"Berlin","Warszawa","Madryt","Poznań","Londyn","Moskwa"};
  int gniazdo, gniazdo2, iloscBiletow;
  struct sockaddr_in adr, nadawca;
  socklen_t dl = sizeof(struct sockaddr_in);

  size_t len = sizeof(miasta) / sizeof(miasta[0]);
  printf("%lu\n", len);
  iloscBiletow =10;
  FILE *f = fopen("bilety.txt", "w");
  if (f == NULL) {
    printf("%s\n", "Nieotwarto pliku");
    exit(1);
  }
  for (int i = 0; i < iloscBiletow; i++) {
    fprintf(f, "%s\n", miasta[rand()%len]);
    fprintf(f, "%s\n", "#");
    fprintf(f, "%s\n", miasta[rand()%len]);
    fprintf(f, "%s\n", "#");
    fprintf(f, "%u.%u.%u %u:%u\n", rand()%30,rand()%12,2018,rand()%24,rand()%59);
    fprintf(f, "%s\n", "#");
    fprintf(f, "%u:%u\n", rand()%8,rand()%59);
    fprintf(f, "%s\n", "#");
    fprintf(f, "R: %u M: %u\n", rand()%16,rand()%8);
    fprintf(f, "%s\n", "#");
    fprintf(f, "%u\n", rand()%300+50);
    fprintf(f, "%s\n", "#");
  }
  fclose(f);

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
    WyslijBilety(gniazdo2);
    printf("Wyslano bilety\n");
    close(gniazdo2);
  }
  close(gniazdo);

  return 0;
}
