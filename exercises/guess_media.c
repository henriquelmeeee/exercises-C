#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  srand(time(NULL));
  int random = (rand() % (100 - 1 + 1)) + 1;
  int last_digit;
  while(1) {
    printf("Digite um número de 1 a 100: ");
    scanf("%d", &last_digit);
    if(last_digit == random) {
      printf("\nParabéns, você acertou!");
      break;
    } else {
      printf("\nVocê errou!");
      if(last_digit > random) {
        printf("\nO número é menor.");
      } else {
        printf("\nO número é maior.");
      }
    }
  }
}
