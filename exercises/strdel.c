#include <stdio.h>
#include <string.h>

#define POSICAO 2

char origem[] = "test";

void strdel(){
  //char origem[] = "test";
  char destino[strlen(origem)];
  int x = 0;
  for(int n = 0; n<4; n++) {
    if(!(n == POSICAO)) {
      destino[x] = origem[n];
      ++x;
    }
  }
  printf("%s", destino);
}

int main() {
  strdel();
}
