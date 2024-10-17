#ifndef EXPR_DIVISION_HPP
#define EXPR_DIVISION_HPP

#include"../../../fd_decs.hpp"

namespace mymath
{
  void ExpressionTreeNodePtr::operator/=(const ExpressionTreeNodePtr other)
  {
    applyBinaryOperation(*this, OP_DIVIDE, other);

    formatArithmeticChains(*this);
  }
  void ExpressionTreeNodePtr::operator/=(const Token& other)
  {
    applyBinaryOperation(*this, OP_DIVIDE, other);

    formatArithmeticChains(*this);
  }

  ExpressionTreeNodePtr operator/(const ExpressionTreeNodePtr a, const ExpressionTreeNodePtr b)
  {
    ExpressionTreeNodePtr a_copy = new ExpressionTreeNode(*a);
    a_copy /= b;
    return a_copy;
  }
  ExpressionTreeNodePtr operator/(const ExpressionTreeNodePtr a, const Token& b)
  {
    ExpressionTreeNodePtr a_copy = new ExpressionTreeNode(*a);
    a_copy /= b;
    return a_copy;
  }
  ExpressionTreeNodePtr operator/(const Token& a, const ExpressionTreeNodePtr b)
  {
    ExpressionTreeNodePtr a_copy = new ExpressionTreeNode(a);
    a_copy /= b;
    return a_copy;
  }
}

#endif