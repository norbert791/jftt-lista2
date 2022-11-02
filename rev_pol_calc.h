#ifndef REV_POL_CALC_H
#define REV_POL_CALC_H

#include<stddef.h>

typedef enum ERevPolCalcOperator {
  PLUS,
  MINUS,
  MULT,
  DIV,
  MOD,
  POW,
} ERevPolCalcOperator;

typedef enum ERevPolCalcError {
  NULL_PTR = 1,
  CALC_MEM_MAX,
} ERevPolCalcError;

typedef struct RevPolCalc RevPolCalc;
RevPolCalc* rev_pol_calc_new(size_t num_of_elements);
void rev_pol_calc_delete(RevPolCalc* calc);
void rev_pol_calc_push_operator(RevPolCalc* calc, ERevPolCalcOperator op);
void rev_pol_calc_push_value(RevPolCalc* calc, long long value);
long long rev_pol_calc_resolve(RevPolCalc* calc);

#endif /* REV_POL_CALC_H */