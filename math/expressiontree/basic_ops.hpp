#ifndef EXPR_BASIC_OPS_HPP
#define EXPR_BASIC_OPS_HPP

#include"../../fd_decs.hpp"
#include<memory>

void applyBinaryOperation(mymath::ExpressionTreeNode*& tree, mymath::TokenType op, const mymath::Token& other)
{
  mymath::ExpressionTreeNode* new_tree = new mymath::ExpressionTreeNode(std::move(mymath::Token(nullptr,op)));
  new_tree->children.push_back(tree);
  new_tree->children.push_back(new mymath::ExpressionTreeNode(std::move(other)));
  tree = new_tree;
}
void applyBinaryOperation(const mymath::Token& other, mymath::TokenType op, mymath::ExpressionTreeNode*& tree)
{
  mymath::ExpressionTreeNode* new_tree = new mymath::ExpressionTreeNode(std::move(mymath::Token(nullptr,op)));
  new_tree->children.push_back(new mymath::ExpressionTreeNode(std::move(other)));
  new_tree->children.push_back(tree);
  tree = new_tree;
}
void applyBinaryOperation(mymath::ExpressionTreeNode*& tree, mymath::TokenType op, mymath::ExpressionTreeNode*& other)
{
  mymath::ExpressionTreeNode* new_tree = new mymath::ExpressionTreeNode(std::move(mymath::Token(nullptr,op)));
  new_tree->children.push_back(tree);
  new_tree->children.push_back(new mymath::ExpressionTreeNode(*other));
}

void applyUnaryOperation(mymath::ExpressionTreeNode*& tree, mymath::TokenType op)
{
  mymath::ExpressionTreeNode* new_tree = new mymath::ExpressionTreeNode(std::move(mymath::Token(nullptr,op)));
  new_tree->children.push_back(tree);
  tree = new_tree;
}

#endif