#ifndef EXPR_SUBTRACTION_HPP
#define EXPR_SUBTRACTION_HPP

#include"../../../fd_decs.hpp"

namespace mymath
{
  void ExpressionTreeNodePtr::operator-=(const ExpressionTreeNodePtr other)
  {
    ExpressionTreeNodePtr neg_other_copy = new ExpressionTreeNode(*(-other));
    *this += neg_other_copy;
  }
  void ExpressionTreeNodePtr::operator-=(const Token& other)
  {
    *this += (-other);
  }
  
  ExpressionTreeNodePtr operator-(const ExpressionTreeNodePtr a)
  {
    ExpressionTreeNodePtr a_copy = new ExpressionTreeNode(*a);

    applyBinaryOperation(a_copy, DT_MUL_CHAIN, Token(dec_float("-1")));

    return a_copy;
  }

  ExpressionTreeNodePtr operator-(const ExpressionTreeNodePtr a, const ExpressionTreeNodePtr b)
  {
    ExpressionTreeNodePtr a_copy = new ExpressionTreeNode(*a);
    a_copy += (-b);
    return a_copy;
  }
  ExpressionTreeNodePtr operator-(const ExpressionTreeNodePtr a, const Token& b)
  {
    ExpressionTreeNodePtr a_copy = new ExpressionTreeNode(*a);
    a_copy += (-b);
    return a_copy;
  }
  ExpressionTreeNodePtr operator-(const Token& a, const ExpressionTreeNodePtr b)
  {
    ExpressionTreeNodePtr b_copy = new ExpressionTreeNode(*(-b));
    b_copy += a;
    return b_copy;
  }
}

#endif