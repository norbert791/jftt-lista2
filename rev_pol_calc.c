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

typedef struct RevPolCalc {
  size_t max_size;
  size_t current_size;
  Token tokens[]; //FAM
} RevPolCalc;

static inline bool isValue(register const Token* const token) {return token->isValue;}
static inline bool isOperand(register const Token* const token) {return !(token->isValue);}
static inline long long integerPow(register long long base, register long long exp);
static inline void resolve_round(register RevPolCalc* const calc);
static inline void performOperation(register const long long leftVal, register const long long rightVal,
                                    register Token* const result, register const ERevPolCalcOperator operator);

RevPolCalc* rev_pol_calc_new(const size_t num_of_elements) {
  register RevPolCalc* const result = calloc(sizeof(*result) + sizeof(result->tokens[0]) * num_of_elements, 1);
  if (result == NULL) {return NULL;}
  result->max_size = num_of_elements;
  return result;
}

void rev_pol_calc_delete(register RevPolCalc* const calc) {
  free(calc);
}

void rev_pol_calc_push_operator(register RevPolCalc* const calc, const ERevPolCalcOperator op) {
  if (calc == NULL) {errno = NULL_PTR;}
  if (calc->current_size == calc->max_size) {
    errno = CALC_MEM_MAX;
  }
  calc->tokens[calc->current_size].isValue = false;
  calc->tokens[calc->current_size].operator = op;
  calc->current_size++;
}

void rev_pol_calc_push_value(register RevPolCalc* const calc, const long long value) {
  if (calc == NULL) {errno = NULL_PTR;}
  if (calc->current_size == calc->max_size) {
    errno = CALC_MEM_MAX;
  }
  calc->tokens[calc->current_size].isValue = true;
  calc->tokens[calc->current_size].value = value;
  calc->current_size++;
}

void rev_pol_calc_clear(register RevPolCalc* const calc) {
  if (calc == NULL) {
    errno = NULL_PTR;
    return;
  }
  calc->current_size = 0;
}

long long rev_pol_calc_resolve(register RevPolCalc* const calc) {
  if (calc == NULL) {
    errno = NULL_PTR;
    return 0;
  }
  size_t last_size = calc->current_size;
  while (calc->current_size > 1) {
    errno = 0;
    resolve_round(calc);
    if (errno != 0) {return 0;}
    if (last_size == calc->current_size) {
      break;
    }
    last_size = calc->current_size;
  }

  if (calc->current_size > 1) {
    errno = CALC_ERROR;
    return 0;
  }

  if (calc->current_size == 0) {
    errno = CALC_EMPTY;
    return 0;
  }

  return calc->tokens[0].value;  
}

static inline void resolve_round(register RevPolCalc* const calc) {
  register size_t index = calc->current_size > 0 ? calc->current_size - 1 : 0;
  while (index > 1) {
    if (isOperand(&calc->tokens[index]) && isValue(&calc->tokens[index - 1]) &&
        isValue(&calc->tokens[index - 2])) {
      
      performOperation(calc->tokens[index - 2].value, calc->tokens[index - 1].value,
                      &calc->tokens[index], calc->tokens[index].operator);
      memmove(&calc->tokens[index - 2], &calc->tokens[index], sizeof(calc->tokens[index]) * (calc->current_size - index));
      index = index > 2 ? index - 3 : 0;
      calc->current_size -= 2;
    } else {
      index--;
    }
  }
}

static inline void performOperation(register const long long leftVal, register const long long rightVal,
                                    register Token* const result, register const ERevPolCalcOperator operator) {
  /*Note: overflow (underflow) detection would be appreciated*/
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
      if (rightVal == 0) {
        errno = CALC_NULL_DIVISION;
        result->value = 0;
        break;
      }
      result->value = leftVal / rightVal;
      break;
    case MOD:
      if (rightVal == 0) {
          errno = CALC_NULL_DIVISION;
          result->value = 0;
          break;
      }
      result->value = leftVal % rightVal;
      break;
    case POW:
      result->value = integerPow(leftVal, rightVal);
      break;
  }

  result->isValue = true;
}

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
