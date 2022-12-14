%{
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
int yywrap();
int yylex();
static bool docs_comments_on = false;
%}

ESC_NEW ([\\][\n])?

%x COMMENT_LINE COMMENT_MULTI STRING CHAR DOC_COMMENT_LINE DOC_COMMENT_MULTI

%%
(([/]{ESC_NEW}){2}[/])|([/]{ESC_NEW}[/]{ESC_NEW}[!]) {if (docs_comments_on) {BEGIN(DOC_COMMENT_LINE); ECHO;} else {BEGIN(COMMENT_LINE);}}
[/]{ESC_NEW}[/] {BEGIN(COMMENT_LINE);}
<COMMENT_LINE,STRING,CHAR,DOC_COMMENT_LINE>[\\][\n] {printf("\n");}
<COMMENT_LINE,STRING,CHAR,DOC_COMMENT_LINE>[\n] {BEGIN(INITIAL); ECHO;}
([/]{ESC_NEW}[*]{ESC_NEW}[*])|([/]{ESC_NEW}[*]{ESC_NEW}[!]) {if (docs_comments_on) {BEGIN(DOC_COMMENT_MULTI); ECHO;} else {BEGIN(COMMENT_MULTI);}}
([/]{ESC_NEW}[*]) {BEGIN(COMMENT_MULTI);}
<COMMENT_MULTI>([*]{ESC_NEW}[/]) {BEGIN(INITIAL);}
<DOC_COMMENT_MULTI>([*]{ESC_NEW}[/]) {BEGIN(INITIAL); ECHO;}
<STRING>["] {BEGIN(INITIAL); ECHO;}
<CHAR>['] {BEGIN(INITIAL); ECHO;}
["] {BEGIN(STRING); ECHO;}
['] {BEGIN(CHAR); ECHO;}
<COMMENT_LINE>(.) {}
<DOC_COMMENT_LINE>(.) {ECHO;}
<COMMENT_MULTI>(.) {}
<COMMENT_MULTI>[\n] {ECHO;}
<DOC_COMMENT_MULTI>(.|\n) {ECHO;}
%%

int yywrap() {
  return 1;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {return EXIT_FAILURE;}
  if (argc > 2 && strcmp(argv[2], "--docs") == 0) {docs_comments_on = true;}
  FILE* input = fopen(argv[1], "r");
  if (input == NULL) {return EXIT_FAILURE;}
  yyset_in(input);
  yylex();
  fclose(input);
}
