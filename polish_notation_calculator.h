#ifndef TOKEN_QUEUE
#define TOKEN_QUEUE

#include<stddef.h>

typedef enum OPERATOR {
  PLUS,
  MINUS,
  MULT,
  DIV,
  MOD,
  POW,
} OPERATOR;

typedef ;
TokenQueue* create_queue(size_t num_of_elements);
void TokenQueue* delete_queue(TokenQueue* queue);


#endif /* TOKEN_QUEUE */