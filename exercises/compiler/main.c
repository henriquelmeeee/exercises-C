#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int symbol_literal_number = 0;

struct variable {
  char *name;
  int size;
};

struct variable map_variables[512];
int number_of_variables = 0;

int check_variable_existence(char* name) {
  if (number_of_variables != 0) {
    for(int n = 0; n<=number_of_variables; n++) {
      struct variable testing = map_variables[n];
      if ( strcmp(name, testing.name) == 0 ) {
        return 1;
      }
    }
  }
  return 0;
}

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
      memset(buffer, 0, 512);
      int x = 0;
      while(line[n] != '"') { 
        buffer[x] = line[n];
        n++; x++;
      }

      // ADD CONST

      buffer[x+1] = '\0';
      fputs("l", asm_code);
      char str[8];
      sprintf(str, "%d", symbol_literal_number);
      fputs(str, asm_code);
      fputs(" db \"", asm_code);
      fputs(buffer, asm_code);
      fputs("\"", asm_code);
      fputs(", 0\n", asm_code);

      // ADD THEIR LENGTH
      
      fputs("l_len", asm_code);
      fputs(str, asm_code);
      fputs(" equ $-l", asm_code);
      fputs(str, asm_code);
      fputs("\n", asm_code);

      symbol_literal_number++;
    }
  }
    
  
  return "Map Literals finalized successfuly";
}

void handle_error(char * to_free, uint code) {
  switch (code) {
    case 5:
      printf("Você deve especificar o nome da variável em \"%s\"\nErro de código %d", to_free, code);
      break;
    default:
      printf("Ocorreu um erro desconhecido.\nLinha: %s", to_free);
  }
  free(to_free);
}

#define add_asm fputs(line+4, asm_code)

char* read_instructions(FILE* code, FILE* asm_code) {
  fputs("section .text\nglobal _start\n_start:\n", asm_code); 
  
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  char instruction[16];
  int literals_buf = 0;

  while( (read = getline(&line, &len, code)) != -1 ) {
    memset(instruction, 0, sizeof(instruction));
    int n = 0;
    while(line[n] != ' ') {
      instruction[n] = line[n];
      n++;
    }
    if(strcmp("print", instruction) == 0) {
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
    } else if (strcmp("asm", instruction) == 0) {
      add_asm;
    } else if (strcmp("set", instruction) == 0) {
      char *name;
      int size = 0; // TODO get size of variable
      if(check_variable_existence(name)) {

      } else {
        char *token = strtok(line, " ");
        token = strtok(NULL, " "); // ignora "set"
        if( (!token) || strcmp(token, "\n") == 0) {
          handle_error(line, 5);
        }
        strcpy(name, token);
        struct variable var_buffer = {name, size};

        char str[8]; // <-- ALLOCATION IN STACK
        sprintf(str, "%d", size);
        fputs("sub rsp, ", asm_code);
        fputs(str, asm_code);      

        fputs("\n", asm_code);
        map_variables[number_of_variables+1] = var_buffer;
        ++number_of_variables;
      }
    }
  }
  fputs("mov rax, 60\nmov rdi, 1\nsyscall\n", asm_code);
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
  } else if (argc < 3) {
    printf("Informe o arquivo de saída (binário).");
    return 1;
  } else {
    FILE* f = fopen(argv[1], "r+");
    start_compile_proccess(f); // começa o processo de compilação
    char* binary = argv[2];
    system("/bin/nasm -f elf64 main.asm -o main.o");
    char* command_init = "/bin/ld main.o -o ";
    char command[100];

    strcpy(command, command_init);
    strcat(command, binary);

    system(command);
  }
}
