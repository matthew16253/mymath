#ifndef EXPR_BASIC_OPS_HPP
#define EXPR_BASIC_OPS_HPP

#include"../../fd_decs.hpp"
#include<memory>

namespace mymath
{
  void applyBinaryOperation(ExpressionTreeNodePtr& tree, mymath::TokenType op, const mymath::Token& other)
  {
    ExpressionTreeNodePtr new_tree = new mymath::ExpressionTreeNode(mymath::Token(op));
    new_tree->children = {tree, new mymath::ExpressionTreeNode(other)};
    tree = new_tree;
  }
  // void applyBinaryOperation(const mymath::Token& other, mymath::TokenType op, ExpressionTreeNodePtr& tree)
  // {
  //   ExpressionTreeNodePtr new_tree = new mymath::ExpressionTreeNode(mymath::Token(op));
  //   new_tree->children = {new mymath::ExpressionTreeNode(other), tree};
  //   tree = new_tree;
  // }
  void applyBinaryOperation(ExpressionTreeNodePtr& tree, mymath::TokenType op, ExpressionTreeNodePtr other)
  {
    ExpressionTreeNodePtr new_tree = new mymath::ExpressionTreeNode(mymath::Token(op));
    new_tree->children = {tree, new mymath::ExpressionTreeNode(*other)};
    tree = new_tree;
  }

  void applyUnaryOperation(ExpressionTreeNodePtr& tree, mymath::TokenType op)
  {
    ExpressionTreeNodePtr new_tree = new mymath::ExpressionTreeNode(mymath::Token(op));
    new_tree->children = {tree};
    tree = new_tree;
  }
}
#endif