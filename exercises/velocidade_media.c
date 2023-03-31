#include <stdio.h>

/*

Fórmula:

Vm = ΔS/Δt, onde:

Vm = velocidade média;
ΔS = distância em metros;
Δt = tempo médio.

*/

int S = 10; float V[5], Vm = 0;

void pegar_velocidades() {
  for(int n = 0; n<5; n++) {
    printf("Digite o tempo em segundos: %d:\n", n+1);
    scanf("%f", &V[n]);
  }
}

void calcular_velocidade_media() {
  for(int n = 0; n<5; n++) { // Calcula o tempo médio
    Vm += V[n]; 
  }
  Vm = S / (Vm / 5); // Calcula a velocidade média
}

int main() {
  pegar_velocidades(); calcular_velocidade_media();
  printf("Vm = %f m/s", Vm);
}
