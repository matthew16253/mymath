#ifndef EXPR_UTILITY_FUNCTIONS_HPP
#define EXPR_UTILITY_FUNCTIONS_HPP

#include"../../fd_decs.hpp"

namespace mymath
{

  bool isAddNode(ExpressionTreeNodePtr tree)
  {
    return (tree->data.type == OP_ADD);
  }
  bool isMulNode(ExpressionTreeNodePtr tree)
  {
    return (tree->data.type == OP_MULTIPLY);
  }
  bool isDivNode(ExpressionTreeNodePtr tree)
  {
    return (tree->data.type == OP_DIVIDE);
  }
  bool isMulDivNode(ExpressionTreeNodePtr tree)
  {
    return (isDivNode(tree) || isMulNode(tree));
  }
  bool isPowNode(ExpressionTreeNodePtr tree)
  {
    return (tree->data.type == OP_POWER);
  }
  bool isMulChainNode(ExpressionTreeNodePtr tree)
  {
    return (tree->data.type == DT_MUL_CHAIN);
  }
  bool isAddChainNode(ExpressionTreeNodePtr tree)
  {
    return (tree->data.type == DT_SUM_CHAIN);
  }

  void addRealNodeToRealNode(ExpressionTreeNodePtr real_this, ExpressionTreeNodePtr real_other)
  {
    *(real_this->data.real_ptr) += *(real_other->data.real_ptr);
  }
  void addRealNodeToRealFraction(ExpressionTreeNodePtr realFraction_this, ExpressionTreeNodePtr real_other)
  {
    *(realFraction_this->children.at(0)->data.real_ptr) += (*(realFraction_this->children.at(1)->data.real_ptr)) * (*(real_other->data.real_ptr));
  }
  void addRealFractionToRealFraction(ExpressionTreeNodePtr realFraction_this, ExpressionTreeNodePtr realFraction_other)
  {
    *(realFraction_this->children.at(0)->data.real_ptr) = (*(realFraction_this->children.at(0)->data.real_ptr)) * (*(realFraction_this->children.at(1)->data.real_ptr))
                                +
                                (*(realFraction_other->children.at(0)->data.real_ptr)) * (*(realFraction_this->children.at(1)->data.real_ptr));
    *(realFraction_this->children.at(1)->data.real_ptr) = (*(realFraction_this->children.at(1)->data.real_ptr)) * (*(realFraction_this->children.at(1)->data.real_ptr));

    simplifyRealFraction(realFraction_this);
  }
  void addRealFractionToRealNode(ExpressionTreeNodePtr real_this, ExpressionTreeNodePtr realFraction_other)
  {
    delete real_this;
    ExpressionTreeNodePtr realFraction_other_copy = new ExpressionTreeNode(*realFraction_other);
    addRealNodeToRealFraction(realFraction_other_copy, real_this);
    real_this = realFraction_other_copy;
  }
  void simplifyRealFraction(ExpressionTreeNodePtr realFraction_this)
  {
    int numerator_exponent;
    int denominator_exponent;
    std::frexp(*(realFraction_this->children.at(0)->data.real_ptr), &numerator_exponent);
    std::frexp(*(realFraction_this->children.at(1)->data.real_ptr), &denominator_exponent);
  }
  bool isRealFraction(ExpressionTreeNodePtr tree)
  {
    return isDivNode(tree) && isReal(tree->children[0]->data.type) && isReal(tree->children[1]->data.type);
  }
}

#endif