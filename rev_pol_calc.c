#include "rev_pol_calc.h"
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <tgmath.h>

typedef struct Token
{
  union {
    long long value;
    ERevPolCalcOperator operator;
  };
  bool isValue;
} Token;

static inline bool isValue(register const Token* const token) {return token->isValue;}
static inline bool isOperand(register const Token* const token) {return !(token->isValue);}
static inline long long integerPow(register long long base, register long long exp){
  long long result = 1;

  while (true) {
    if (exp & 1) {result *= base;}
    exp >>= 1;
    if (exp == 0) {
      break;
    }
    base *= base;
  }

  return result;
}

static inline void performOperation(register const long long leftVal, register const long long rightVal,
                                    register Token* const result, register const ERevPolCalcOperator operator) {
  switch (operator) {
    case SUM:
      result->value = leftVal + rightVal;
      break;
    case SUBT:
      result->value = leftVal - rightVal;
      break;
    case MULT:
      result->value = leftVal * rightVal;
      break;
    case DIV:
      result->value = leftVal / rightVal;
      break;
    case MOD:
      result->value = leftVal % rightVal;
      break;
    case POW:
      result->value = integerPow(leftVal, rightVal);
      break;
  }

  result->isValue = true;
}

static inline void resolve_round(register RevPolCalc* const calc) {
  register size_t index = calc->current_size - 1;
  while (index > 1) {
    if (isOperand(&calc->tokens[index]) && isValue(&calc->tokens[index - 1].isValue) &&
        isValue(&calc->tokens[index - 2])) {
      
      performOperation(calc->tokens[index - 2].value, calc->tokens[index - 1].value,
                      &calc->tokens[index], calc->tokens[index].operator);
      memmove(&calc->tokens[index - 2], &calc->tokens[index], sizeof(calc->tokens[index]) * (calc->current_size - index));
      index = index - 3;
      calc->current_size -= 2;
    } else {
      index--;
    }
  }
}

typedef struct RevPolCalc {
  size_t max_size;
  size_t current_size;
  Token tokens[]; //FAM
} RevPolCalc;

RevPolCalc* rev_pol_calc_new(size_t num_of_elements) {
  register RevPolCalc* const result = calloc(sizeof(*result) + sizeof(result->tokens[0]) * num_of_elements, 1);
  if (result == NULL) {return NULL;}
  result->max_size = num_of_elements;
}

void rev_pol_calc_delete(register RevPolCalc* const calc) {
  free(calc);
}

void rev_pol_calc_push_operator(register RevPolCalc* const calc, ERevPolCalcOperator op) {
  if (calc == NULL) {errno = NULL_PTR;}
  if (calc->current_size == calc->max_size) {
    errno = CALC_MEM_MAX;
  }
  calc->tokens[calc->current_size].isValue = false;
  calc->tokens[calc->current_size].operator = op;
}

void rev_pol_calc_push_value(register RevPolCalc* const calc, long long value) {
  if (calc == NULL) {errno = NULL_PTR;}
  if (calc->current_size == calc->max_size) {
    errno = CALC_MEM_MAX;
  }
  calc->tokens[calc->current_size].isValue = true;
  calc->tokens[calc->current_size].value = value;
}

long long rev_pol_calc_resolve(register RevPolCalc* const calc) {
  size_t last_size = calc->current_size;
  size_t num_of_iterations = 0;
  while (calc->current_size > 1) {
    resolve_round(calc);
    if (last_size == calc->current_size) {
      break;
    }
  }

  if (calc->current_size != 1) {
    errno = CALC_ERROR;
    return 0;
  }

  return calc->tokens[0].value;  
}