#ifndef EXPR_BASIC_OPS_HPP
#define EXPR_BASIC_OPS_HPP

#include<fd_decs.hpp>
#include<memory>

void applyBinaryOperation(std::shared_ptr<mymath::ExpressionTreeNode>& tree, mymath::TokenType op, const mymath::Token& other)
{
  std::shared_ptr<mymath::ExpressionTreeNode> new_tree = std::make_shared<mymath::ExpressionTreeNode>(mymath::Token(nullptr,op));
  new_tree->children.push_back(tree);
  new_tree->children.push_back(std::make_shared<mymath::ExpressionTreeNode>(other,mymath::MoveTag{}));
  tree = new_tree;
}
void applyBinaryOperation(const mymath::Token& other, mymath::TokenType op, std::shared_ptr<mymath::ExpressionTreeNode>& tree)
{
  std::shared_ptr<mymath::ExpressionTreeNode> new_tree = std::make_shared<mymath::ExpressionTreeNode>(mymath::Token(nullptr,op));
  new_tree->children.push_back(std::make_shared<mymath::ExpressionTreeNode>(other,mymath::MoveTag{}));
  new_tree->children.push_back(tree);
  tree = new_tree;
}
void applyBinaryOperation(std::shared_ptr<mymath::ExpressionTreeNode>& tree, mymath::TokenType op, std::shared_ptr<mymath::ExpressionTreeNode>& other)
{
  std::shared_ptr<mymath::ExpressionTreeNode> new_tree = std::make_shared<mymath::ExpressionTreeNode>(mymath::Token(nullptr,op));
  new_tree->children.push_back(tree);
  new_tree->children.push_back(std::make_shared<mymath::ExpressionTreeNode>(*other));
}

void applyUnaryOperation(std::shared_ptr<mymath::ExpressionTreeNode>& tree, mymath::TokenType op)
{
  std::shared_ptr<mymath::ExpressionTreeNode> new_tree = std::make_shared<mymath::ExpressionTreeNode>(mymath::Token(nullptr,op));
  new_tree->children.push_back(tree);
  tree = new_tree;
}

#endif