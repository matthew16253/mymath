#ifndef EXPR_UTILITY_FUNCTIONS_HPP
#define EXPR_UTILITY_FUNCTIONS_HPP

#include<numeric>

#include"../../fd_decs.hpp"

namespace mymath
{

  bool isAddNode(ExpressionTreeNodePtr tree)
  {
    if(std::holds_alternative<TokenType>(tree->data.dataVariant))
    {
      return std::get<TokenType>(tree->data.dataVariant) == OP_ADD;
    }
    return false;
  }
  bool isMulNode(ExpressionTreeNodePtr tree)
  {
    if(std::holds_alternative<TokenType>(tree->data.dataVariant))
    {
      return std::get<TokenType>(tree->data.dataVariant) == OP_MULTIPLY;
    }
    return false;
  }
  bool isDivNode(ExpressionTreeNodePtr tree)
  {
    if(std::holds_alternative<TokenType>(tree->data.dataVariant))
    {
      return std::get<TokenType>(tree->data.dataVariant) == OP_DIVIDE;
    }
    return false;
  }
  bool isMulDivNode(ExpressionTreeNodePtr tree)
  {
    return (isDivNode(tree) || isMulNode(tree));
  }
  bool isPowNode(ExpressionTreeNodePtr tree)
  {
    if(std::holds_alternative<TokenType>(tree->data.dataVariant))
    {
      return std::get<TokenType>(tree->data.dataVariant) == OP_POWER;
    }
    return false;
  }
  bool isMulChainNode(ExpressionTreeNodePtr tree)
  {
    if(std::holds_alternative<TokenType>(tree->data.dataVariant))
    {
      return std::get<TokenType>(tree->data.dataVariant) == DT_MUL_CHAIN;
    }
    return false;
  }
  bool isAddChainNode(ExpressionTreeNodePtr tree)
  {
    if(std::holds_alternative<TokenType>(tree->data.dataVariant))
    {
      return std::get<TokenType>(tree->data.dataVariant) == DT_SUM_CHAIN;
    }
    return false;
  }

  void addRealNodeToRealNode(ExpressionTreeNodePtr real_this, ExpressionTreeNodePtr real_other)
  {
    *std::get<long double*>(real_this->data.dataVariant) += *std::get<long double*>(real_other->data.dataVariant);
  }
  void addRealNodeToRealFraction(ExpressionTreeNodePtr realFraction_this, ExpressionTreeNodePtr real_other)
  {
    *std::get<long double*>(realFraction_this->children.at(0)->data.dataVariant) += (*std::get<long double*>(realFraction_this->children.at(1)->data.dataVariant)) * (*std::get<long double*>(real_other->data.dataVariant));
  }
  void addRealFractionToRealFraction(ExpressionTreeNodePtr realFraction_this, ExpressionTreeNodePtr realFraction_other)
  {
    *std::get<long double*>(realFraction_this->children.at(0)->data.dataVariant) = (*std::get<long double*>(realFraction_this->children.at(0)->data.dataVariant)) * (*std::get<long double*>(realFraction_this->children.at(1)->data.dataVariant))
                                +
                                (*std::get<long double*>(realFraction_other->children.at(0)->data.dataVariant)) * (*std::get<long double*>(realFraction_this->children.at(1)->data.dataVariant));
    *std::get<long double*>(realFraction_this->children.at(1)->data.dataVariant) = (*std::get<long double*>(realFraction_this->children.at(1)->data.dataVariant)) * (*std::get<long double*>(realFraction_this->children.at(1)->data.dataVariant));

    simplifyRealFraction(realFraction_this);
  }
  void addRealFractionToRealNode(ExpressionTreeNodePtr real_this, ExpressionTreeNodePtr realFraction_other)
  {
    ExpressionTreeNodePtr realFraction_other_copy = new ExpressionTreeNode(*realFraction_other);
    addRealNodeToRealFraction(realFraction_other_copy, real_this);
    *real_this = *realFraction_other_copy;
  }
  void simplifyRealFraction(ExpressionTreeNodePtr realFraction_this)
  {
    // do later pls :DDDD
  }
  bool isRealFraction(ExpressionTreeNodePtr tree)
  {
    return isDivNode(tree) && isReal(tree->children[0]->data) && isReal(tree->children[1]->data);
  }
}

#endif