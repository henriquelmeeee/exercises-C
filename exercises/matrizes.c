#include <stdio.h>

int main() {
  int notas[5], soma;

  for(int n=1; n<=5; n++) {
    printf("\nDigite a nota %d: ", n);
    scanf("%d", &notas[n]);
    soma+=notas[n];
  }

  

  for(int n = 1; n<=5; n++) {
    printf("Nota %d: %d\n", n, notas[n]);
  }

  printf("Média: %d", soma/5);

}
