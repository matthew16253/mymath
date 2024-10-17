#ifndef EXPR_UTILITY_FUNCTIONS_HPP
#define EXPR_UTILITY_FUNCTIONS_HPP

#include<numeric>

#include"../../fd_decs.hpp"

namespace mymath
{

  bool isAdd(Token& token)
  {
    if(token.is_type<TokenType>())
    {
      return (token.get<TokenType>() == OP_ADD);
    }
    return false;
  }
  bool isMul(Token& token)
  {
    if(token.is_type<TokenType>())
    {
      return (token.get<TokenType>() == OP_MULTIPLY);
    }
    return false;
  }
  bool isDiv(Token& token)
  {
    if(token.is_type<TokenType>())
    {
      return token.get<TokenType>() == OP_DIVIDE;
    }
    return false;
  }
  bool isMulDiv(Token& token)
  {
    return (isDiv(token) || isMul(token));
  }
  bool isPow(Token& token)
  {
    if(token.is_type<TokenType>())
    {
      return token.get<TokenType>() == OP_POWER;
    }
    return false;
  }
  bool isMulChain(Token& token)
  {
    if(token.is_type<TokenType>())
    {
      return token.get<TokenType>() == DT_MUL_CHAIN;
    }
    return false;
  }
  bool isAddChain(Token& token)
  {
    if(token.is_type<TokenType>())
    {
      return token.get<TokenType>() == DT_SUM_CHAIN;
    }
    return false;
  }
  bool isDiv(ExpressionTreeNodePtr tree)
  {
    return isDiv(tree->data);
  }
  bool isPow(ExpressionTreeNodePtr tree)
  {
    return isPow(tree->data);
  }
  bool isMulChain(ExpressionTreeNodePtr tree)
  {
    return isMulChain(tree->data);
  }
  bool isAddChain(ExpressionTreeNodePtr tree)
  {
    return isAddChain(tree->data);
  }
  // bool isCommutativeOperator(ExpressionTreeNodePtr tree)
  // {
  //   ;
  // }

  void addRealNodeToRealNode(ExpressionTreeNodePtr& real_this, ExpressionTreeNodePtr real_other)
  {
    *std::get<dec_float*>(real_this->data.dataVariant) += *std::get<dec_float*>(real_other->data.dataVariant);
  }
  void addRealNodeToRealFraction(ExpressionTreeNodePtr& realFraction_this, ExpressionTreeNodePtr real_other)
  {
    *std::get<dec_float*>(realFraction_this->children.at(0)->data.dataVariant) += (*std::get<dec_float*>(realFraction_this->children.at(1)->data.dataVariant)) * (*std::get<dec_float*>(real_other->data.dataVariant));
  }
  void addRealFractionToRealFraction(ExpressionTreeNodePtr& realFraction_this, ExpressionTreeNodePtr realFraction_other)
  {
    *std::get<dec_float*>(realFraction_this->children.at(0)->data.dataVariant) = (*std::get<dec_float*>(realFraction_this->children.at(0)->data.dataVariant)) * (*std::get<dec_float*>(realFraction_this->children.at(1)->data.dataVariant))
                                +
                                (*std::get<dec_float*>(realFraction_other->children.at(0)->data.dataVariant)) * (*std::get<dec_float*>(realFraction_this->children.at(1)->data.dataVariant));
    *std::get<dec_float*>(realFraction_this->children.at(1)->data.dataVariant) = (*std::get<dec_float*>(realFraction_this->children.at(1)->data.dataVariant)) * (*std::get<dec_float*>(realFraction_this->children.at(1)->data.dataVariant));

    simplifyRealFraction(realFraction_this);
  }
  void addRealFractionToRealNode(ExpressionTreeNodePtr& real_this, ExpressionTreeNodePtr realFraction_other)
  {
    ExpressionTreeNodePtr realFraction_other_copy = new ExpressionTreeNode(*realFraction_other);
    addRealNodeToRealFraction(realFraction_other_copy, real_this);
    delete real_this;
    real_this = realFraction_other_copy;
  }
  
  void realNodeAndRealFractionNodePlusAssign(ExpressionTreeNodePtr& a, ExpressionTreeNodePtr b)
  {
    if(isReal(a->data) && isReal(b->data))
    {
      addRealNodeToRealNode(a, b);
    }
    else if(isReal(a->data) && isRealFraction(b))
    {
      addRealFractionToRealNode(a, b);
    }
    else if(isRealFraction(a) && isReal(b->data))
    {
      addRealNodeToRealFraction(a, b);
    }
    else if(isRealFraction(a) && isRealFraction(b))
    {
      addRealFractionToRealFraction(a, b);
    }
    else
    {
      throw std::runtime_error("not real node or real fraction");
    }
  }

  void simplifyRealFraction(ExpressionTreeNodePtr realFraction_this)
  {
    // do later pls :DDDD
  }
  bool isRealFraction(ExpressionTreeNodePtr tree)
  {
    return isDiv(tree->data) && isReal(tree->children[0]->data) && isReal(tree->children[1]->data);
  }
}

#endif