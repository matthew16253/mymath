#ifndef EXPR_UTILITY_FUNCTIONS_HPP
#define EXPR_UTILITY_FUNCTIONS_HPP

#include"../../fd_decs.hpp"

namespace mymath
{

  bool isAddNode(ExpressionTreeNode* tree)
  {
    return (tree->data.type == OP_ADD);
  }
  bool isMulNode(ExpressionTreeNode* tree)
  {
    return (tree->data.type == OP_MULTIPLY);
  }
  bool isDivNode(ExpressionTreeNode* tree)
  {
    return (tree->data.type == OP_DIVIDE);
  }
  bool isMulDivNode(ExpressionTreeNode* tree)
  {
    return (isDivNode(tree) || isMulNode(tree));
  }
  bool isPowNode(ExpressionTreeNode* tree)
  {
    return (tree->data.type == OP_POWER);
  }
  bool isMulChainNode(ExpressionTreeNode* tree)
  {
    return (tree->data.type == DT_MUL_CHAIN);
  }
  bool isAddChainNode(ExpressionTreeNode* tree)
  {
    return (tree->data.type == DT_SUM_CHAIN);
  }

  void simpleCollectLikeTerms(ExpressionTreeNode* tree)
  {
    ;
  }

}

#endif