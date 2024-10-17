#ifndef EXPR_ADDITION_HPP
#define EXPR_ADDITION_HPP

#include"../../../fd_decs.hpp"

namespace mymath
{
  void ExpressionTreeNodePtr::operator+=(const ExpressionTreeNodePtr other)
  {
    applyBinaryOperation(*this, DT_SUM_CHAIN, other);

    formatArithmeticChains(*this);
  }
  void ExpressionTreeNodePtr::operator+=(const Token& other)
  {
    applyBinaryOperation(*this, DT_SUM_CHAIN, other);

    formatArithmeticChains(*this);
  }

  ExpressionTreeNodePtr operator+(const ExpressionTreeNodePtr a, const ExpressionTreeNodePtr b)
  {
    ExpressionTreeNodePtr a_copy = new ExpressionTreeNode(*a);
    a_copy += b;
    return a_copy;
  }
  ExpressionTreeNodePtr operator+(const ExpressionTreeNodePtr a, const Token& b)
  {
    ExpressionTreeNodePtr a_copy = new ExpressionTreeNode(*a);
    a_copy += b;
    return a_copy;
  }
  ExpressionTreeNodePtr operator+(const Token& a, const ExpressionTreeNodePtr b)
  {
    ExpressionTreeNodePtr b_copy = new ExpressionTreeNode(*b);
    b_copy += a;
    return b_copy;
  }
}

#endif