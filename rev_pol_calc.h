#ifndef REV_POL_CALC_H
#define REV_POL_CALC_H

#include<stddef.h>

typedef enum ERevPolCalcOperator {
  SUM,
  SUBT,
  MULT,
  DIV,
  MOD,
  POW,
} ERevPolCalcOperator;

typedef enum ERevPolCalcError {
  NULL_PTR = 1,
  CALC_MEM_MAX,
  CALC_ERROR, //Divide into mismatch between num of operands and operators, misplaced operand and empty stack
} ERevPolCalcError;

typedef struct RevPolCalc RevPolCalc;
RevPolCalc* rev_pol_calc_new(size_t num_of_elements);
void rev_pol_calc_delete(RevPolCalc* calc);
void rev_pol_calc_push_operator(RevPolCalc* calc, ERevPolCalcOperator op);
void rev_pol_calc_push_value(RevPolCalc* calc, long long value);
void rev_pol_calc_clear(RevPolCalc* calc);
long long rev_pol_calc_resolve(RevPolCalc* calc);

#endif /* REV_POL_CALC_H */