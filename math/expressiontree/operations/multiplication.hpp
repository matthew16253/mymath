#ifndef EXPR_MULTIPLICATION_HPP
#define EXPR_MULTIPLICATION_HPP

#include"../../../fd_decs.hpp"

namespace mymath
{
  void ExpressionTreeNodePtr::operator*=(const ExpressionTreeNodePtr other)
  {
    applyBinaryOperation(*this, DT_MUL_CHAIN, other);

    formatArithmeticChains(*this);
  }
  void ExpressionTreeNodePtr::operator*=(const Token& other)
  {
    applyBinaryOperation(*this, DT_MUL_CHAIN, other);

    formatArithmeticChains(*this);
  }

  ExpressionTreeNodePtr operator*(const ExpressionTreeNodePtr a, const ExpressionTreeNodePtr b)
  {
    ExpressionTreeNodePtr a_copy = new ExpressionTreeNode(*a);
    a_copy *= b;
    return a_copy;
  }
  ExpressionTreeNodePtr operator*(const ExpressionTreeNodePtr a, const Token& b)
  {
    ExpressionTreeNodePtr a_copy = new ExpressionTreeNode(*a);
    a_copy *= b;
    return a_copy;
  }
  ExpressionTreeNodePtr operator*(const Token& a, const ExpressionTreeNodePtr b)
  {
    ExpressionTreeNodePtr b_copy = new ExpressionTreeNode(*b);
    b_copy *= a;
    return b_copy;
  }
}

#endif