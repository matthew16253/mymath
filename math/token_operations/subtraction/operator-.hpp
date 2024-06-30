#ifndef TOKEN_OPERATOR_SUBTRACT_HPP
#define TOKEN_OPERATOR_SUBTRACT_HPP

#include"../../../fd_decs.hpp"

namespace mymath
{
mymath::Token operator-(const mymath::Token& a, const mymath::Token& b) // DO NOT USE THE INPUTTED TOKENS
{
  mymath::Token new_token = a;
  new_token -= b;
  return new_token;
}


mymath::Token operator-(const mymath::Token& a)
{
  using namespace mymath;

  Token new_token(a);

  switch(new_token.type)
  {
    case DT_REAL:
    {
      *new_token.real_ptr *= -1;
      break;
    }
    case DT_COMPLEX:
    {
      *new_token.complex_ptr = - *new_token.complex_ptr;
      break;
    }
    case DT_VECTOR:
    {
      *new_token.vec_ptr = - *new_token.vec_ptr;
      break;
    }
    case DT_MATRIX:
    {
      *new_token.mat_ptr = - *new_token.mat_ptr;
      break;
    }
    case DT_ALGEBRAIC_EXPR:
    {
      ExpressionTreeNodePtr new_tree = new_token.expr_ptr;
      applyUnaryOperation(new_tree, OP_NEGATE);
      new_token.dataptr = new_tree;
      break;
    }
    default:
    {
      TokenType arr[] = {a.type};
      new_token = Token(new InfoLog<1,TokenType>(arr), ERROR_INVALID_1_OPERANDS);
      break;
    }
  }
  return new_token;
}

}

#endif