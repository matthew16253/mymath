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
}

#endif