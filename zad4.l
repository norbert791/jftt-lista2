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

enum {
  Token_Queue_Size = 2000;
};

static size_t current_size = 0;

static int token_push(Token token)
static int token_resolve(Token token)

static Token queue[Token_Queue_Size];

static int token_push(Token token) {
  
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