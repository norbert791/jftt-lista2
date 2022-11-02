#include "rev_pol_calc.h"
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

typedef struct Token
{
  union {
    long long value;
    ERevPolCalcOperator operator;
  };
  bool isValue;
} Token;


typedef struct RevPolCalc {
  size_t max_size;
  size_t current_size;
  Token tokens[]; //FAM
} RevPolCalc;

RevPolCalc* rev_pol_calc_new(size_t num_of_elements) {
  RevPolCalc* result = calloc(sizeof(*result) + sizeof(result->tokens[0]) * num_of_elements, 1);
  if (result == NULL) {return NULL;}
  result->max_size = num_of_elements;
}

void rev_pol_calc_delete(RevPolCalc* calc) {
  free(calc);
}

void rev_pol_calc_push_operator(RevPolCalc* calc, ERevPolCalcOperator op) {
  if (calc == NULL) {errno = NULL_PTR;}
  if (calc->current_size == calc->max_size) {
    errno = CALC_MEM_MAX;
  }
  calc->tokens[calc->current_size].isValue = false;
  calc->tokens[calc->current_size].operator = op;
}

void rev_pol_calc_push_value(RevPolCalc* calc, long long value) {
  if (calc == NULL) {errno = NULL_PTR;}
  if (calc->current_size == calc->max_size) {
    errno = CALC_MEM_MAX;
  }
  calc->tokens[calc->current_size].isValue = true;
  calc->tokens[calc->current_size].value = value;
}

long long rev_pol_calc_resolve(RevPolCalc* calc);