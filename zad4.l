%{
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <signal.h>
#include "rev_pol_calc.h"
int yywrap();
int yylex();
static RevPolCalc* calc = NULL;
static bool continueWork = true;

static inline bool parseNumber(const char str[const static 1]) {
  errno = 0;
  long long result = strtoll(str, NULL, 10);
  if (errno != 0) {
    printf("\nERROR: '%s' is too large\n", str);
    errno = 0;
    return false;
  }
  rev_pol_calc_push_value(calc, result);
  if (errno != 0) {
    puts("Mem error: too many operators/operands");
    return false;
  }
  errno = 0;
  return true;
}

static inline ERevPolCalcOperator convertStrOp(const char str[const static 2]) {
  const char symbol = str[0];
  switch (symbol) {
    case '+': return SUM;
    case '-': return SUBT;
    case '*': return MULT;
    case '/': return DIV;
    case '%': return MOD;
    case '^': return POW;
  }
  return 0;
}

static inline bool parseOperator(const char str[const static 2]) {
  errno = 0;
  rev_pol_calc_push_operator(calc, convertStrOp(str));
  if (errno != 0) {
    puts("Mem error: too many operators/operands");
    return false;
  }
  errno = 0;
  return true;
}

static inline void finilizeComputation(RevPolCalc* calc) {
  errno = 0;
  long long result = rev_pol_calc_resolve(calc);
  switch (errno) {
    case 0:
      printf("The result is %lld\n", result);
      break;
    case NULL_PTR:
      puts("NULL_PTR error");
      break;
    case CALC_MEM_MAX:
      puts("Memory fail: the number of operations/operands is too large");
      break;
    case CALC_NULL_DIVISION:
      puts("Illegal operation: division by 0");
      break;
    case CALC_EMPTY:
      break;
    case CALC_ERROR:
      puts("The number of operands and operations does not match");
      break;
  }
  errno = 0;
}

static void cleanCalc(void) {
  puts("Leaving...");
  rev_pol_calc_delete(calc);
}
%}

%%
[-]?([0-9])+ {if(!continueWork || !parseNumber(yytext)) {continueWork = false;}}
[+\-*/%^] {if (!continueWork || !parseOperator(yytext)) {continueWork = false;}}
[\n] {if (continueWork) {puts(""); finilizeComputation(calc);} rev_pol_calc_clear(calc);}
^((quit)|(q)) {exit(EXIT_SUCCESS);}
[^\t \n] {if(continueWork) {printf("\nUnknown symbol: %s\n", yytext); rev_pol_calc_clear(calc); continueWork = false;}}
%%

int yywrap() {

  return 1;
}


int main(void) {
  sigset_t mask;
  sigfillset(&mask);
  sigprocmask(SIG_SETMASK, &mask, NULL);
  calc = rev_pol_calc_new(200);
  if (calc == NULL || atexit(cleanCalc) != 0 || at_quick_exit(cleanCalc)) {
    puts("calc init error");
    return EXIT_FAILURE;
  }
  yylex();
  
}
