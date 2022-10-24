%{
#include <stdio.h>
#include <stddef.h>
int yywrap();
int yylex();
size_t num_of_lines = 0;
size_t num_of_words = 0;
%}


%%
\n([\t ]*)\n {printf(""); num_of_lines++;}
^([\t ]+) {(printf(""));}
[\t ]+ {printf(" ");}
([\t ]+)$ {(printf(""));}
\n {printf("\n"); num_of_lines++;}
[^\t ]+ {printf("%s", yytext); num_of_words++;};
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