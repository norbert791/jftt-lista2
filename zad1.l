%{
#include <stdio.h>
#include <stddef.h>
int yywrap();
int yylex();
static size_t num_of_lines = 0;
static size_t num_of_words = 0;
%}


%%
^[\t ]([\t ]*) {printf("");}
([\t ]*)[\t ]$ {printf("");}
[\t ]+ {printf(" ");}
[^\t \n]+ {ECHO; num_of_words++;};
(\n([\t \n]*))/([^\t ]) {printf("\n"); num_of_lines++;}
\n([\t \n]*) {printf(""); num_of_lines++;}
%%

int yywrap() {
    printf("\nNum of lines: %zu\nNum of words: %zu\n", num_of_lines, num_of_words);
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