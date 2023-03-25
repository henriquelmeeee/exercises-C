#include <stdio.h>

int main() {
  int matriz[3][3] = {
    {1,2,3},
    {4,5,6},
    {7,8,9}
  };
  for(int x = 0; x<3; x++) {
    for(int y = 0; y<3; y++) {
      printf("Linha %d coluna %d VALOR: %d\n", x, y, matriz[x][y]);
    }
  }
}
