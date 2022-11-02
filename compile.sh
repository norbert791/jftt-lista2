#!/bin/bash
lex zad1.l
clang lex.yy.c -o zad1
lex zad2.l
clang lex.yy.c -o zad2
lex zad3.l
clang lex.yy.c -o zad3