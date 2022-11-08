#!/bin/bash
lex zad1.l
clang lex.yy.c -o zad1.bin
lex zad2.l
clang lex.yy.c -o zad2.bin
lex zad3.l
clang lex.yy.c -o zad3.bin
lex zad4.l
clang lex.yy.c rev_pol_calc.c -o zad4.bin -lm
rm lex.yy.c