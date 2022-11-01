%{
#include <stdio.h>
#include <stddef.h>
int yywrap();
int yylex();
size_t num_of_lines = 0;
size_t num_of_words = 0;
%}

%s STRING_SINGLE STRING_DOUBLE STRING_LINES

%%
<INITIAL>[\'] {BEGIN(STRING_SINGLE);puts("[']start"); ECHO;}
<STRING_SINGLE>[\'] {BEGIN(INITIAL);puts("[']end"); ECHO;}
<INITIAL>[\"]{3} {BEGIN(STRING_LINES);puts("[\"\"\"]start"); ECHO;}
<STRING_LINES>[\"]{3} {BEGIN(INITIAL);puts("[\"\"\"]end"); ECHO;}
<INITIAL>[\"] {BEGIN(STRING_DOUBLE);puts("[\"]start"); ECHO;}
<STRING_DOUBLE>[\"] {BEGIN(INITIAL);puts("[\"]end"); ECHO;}
<INITIAL>[#](.)* {printf("");}
(.|\n)* {printf(""); ECHO;}
%%

int yywrap() {
  puts("");
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