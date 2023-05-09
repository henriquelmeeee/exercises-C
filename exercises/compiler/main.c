#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int symbol_literal_number = 0;

char* map_literals(FILE* code, FILE* asm_code) {
  fputs("section .rodata\n", asm_code);
  
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  
  rewind(code);

  while( (read = getline(&line, &len, code)) != -1 ) {
    int n = 0;
    if( strchr(line, '"') ) {
      while(line[n] != '"') {n++;}
      n++;
      char buffer[512];
      int x = 0;
      while(line[n] != '"') { 
        buffer[x] = line[n];
        n++; x++;
      }
      buffer[x+1] = '\0';
      fputs("l", asm_code);
      char str[8];
      sprintf(str, "%d", symbol_literal_number);
      fputs(str, asm_code);
      fputs(" db \"", asm_code);
      fputs(buffer, asm_code);
      fputs("\"", asm_code);
      fputs(", 0\n", asm_code);

      symbol_literal_number++;
    }
  free(line);
  }
    
  
  return "Map Literals finalized successfuly";
}

char* read_instructions(FILE* code, FILE* asm_code) {
  fputs("section .text\nglobal _start\n_start:\n", asm_code);

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  char instruction[16];
  int literals_buf = 0;

  while( (read = getline(&line, &len, code)) != -1 ) {
    int n = 0;
    while(line[n] != ' ') {
      instruction[n] = line[n];
      n++;
    }
    //instruction[n+1] = '\0';
    if(strcmp(instruction, "print")) {
      fputs("mov rax, 1\n", asm_code);
      fputs("mov rdi, 1\n", asm_code);
      fputs("mov rsi, l", asm_code);
      char buffer[12]; // BUFFER FOR LITERALS_BUF (INT -> STRING)
      sprintf(buffer, "%d", literals_buf);
      fputs(buffer, asm_code);
      fputs("\n", asm_code);
      fputs("mov rdx, l_len", asm_code);
      fputs(buffer, asm_code);
      fputs("\nsyscall\n", asm_code);
      literals_buf++;
    } else if (strcmp(instruction, "asm")) {
      fputs(line+5, asm_code);
      fputs("\n", asm_code);
    }
    free(line);
  }

  return "Reading Instructions finalized successfuly";
}

char* start_compile_proccess(FILE* code) {
  char line_buffer[64];
  char asm_code[1024];
  FILE* f = fopen("main.asm", "r+");
  char buffer[1024];
  fgets(buffer, 1024, code);
  map_literals(code, f);
  rewind(code); // volta o ponteiro para o início do arquivo
  read_instructions(code, f);
  fclose(f);
  fclose(code);
  return "Compile Proccess finalized successfuly";
}

int main(int argc, char *argv[]) {
  if(argc < 2) {
    printf("Informe o arquivo a ser compilado.");
    return 1;
  } else {
    FILE* f = fopen(argv[1], "r+");
    //char buffer[1024]; // conteúdo de até 1KB de código
    //fgets(buffer, 1024, f); // coloca o conteúdo do código no buffer
    start_compile_proccess(f); // começa o processo de compilação
  }
}
