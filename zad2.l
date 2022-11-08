%{
#include <stdio.h>
#include <stddef.h>
int yywrap();
int yylex();
%}

%x STRING_SINGLE STRING_DOUBLE STRING_LINES COMMENT

%%
[\'] {BEGIN(STRING_SINGLE); ECHO;}
<STRING_SINGLE>[\'] {BEGIN(INITIAL); ECHO;}
[\"]{3} {BEGIN(STRING_LINES); ECHO;}
<STRING_LINES>[\"]{3} {BEGIN(INITIAL); ECHO;}
[\"] {BEGIN(STRING_DOUBLE); ECHO;}
<STRING_DOUBLE>[\"] {BEGIN(INITIAL); ECHO;}
[#] {printf(""); BEGIN(COMMENT);}
<COMMENT>(.) {printf("");}
<COMMENT>[\n] {ECHO; BEGIN(INITIAL);}
<STRING_SINGLE,STRING_DOUBLE>[\n] {ECHO; BEGIN(INITIAL);}
<STRING_SINGLE,STRING_DOUBLE,STRING_LINES>(.|[\n]) {ECHO;}
(.|\n) {ECHO;}
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
