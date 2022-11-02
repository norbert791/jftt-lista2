%{
#include <stdio.h>
#include <stddef.h>
int yywrap();
int yylex();

enum EAction {
  PLUS,
  MINUS,
  MULT,
  DIV,
  MOD,
  POW,
  NUMBER,
}

typedef struct Token {
  long long value,
  EAction type;
}

%}

%x STRING_SINGLE STRING_DOUBLE STRING_LINES COMMENT

%%

%%

int yywrap() {

  return 1;
}

int main(int argc, char* argv[]) {
  if (argc == 1) {return EXIT_FAILURE;}
  FILE* input = fopen(argv[1], "r");
  if (input == NULL) {return EXIT_FAILURE;}
  yyset_in(input);
  yylex();
  fclose(input);
}