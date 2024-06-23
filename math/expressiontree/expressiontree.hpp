#ifndef EXPR_HPP
#define EXPR_HPP

#include"../../fd_decs.hpp"

#include<vector>


namespace mymath
{
  ExpressionTreeNode::ExpressionTreeNode() : data(Token(nullptr, mymath::TokenType::DT_UNINIT)), children({}) {}
  ExpressionTreeNode::ExpressionTreeNode(const Token& _data) : data(_data), children({})  {}
  ExpressionTreeNode::ExpressionTreeNode(Token&& _data) : data(_data), children({})  {}
  ExpressionTreeNode::ExpressionTreeNode(const ExpressionTreeNode& other)
  {
    data = other.data;
    for(int i = 0; i < other.children.size(); i++)
    {
      children.push_back(new ExpressionTreeNode(*other.children.at(i)));
    }
  }
  ExpressionTreeNode::ExpressionTreeNode(ExpressionTreeNode&& other)
  {
    data = other.data;
    children = other.children;
    other.children = std::vector<ExpressionTreeNode*>();
  }
  ExpressionTreeNode& ExpressionTreeNode::operator=(ExpressionTreeNode other)
  {
    swap(*this,other);
    return *this;
  }
  ExpressionTreeNode::~ExpressionTreeNode()
  {
    for(int index = 0; index < children.size(); index++)
    {
      delete children.at(index);
    }
  }
  void print(ExpressionTreeNode* tree)
  {
    recursive_print(tree, 0);
  }
  void recursive_print(ExpressionTreeNode* tree, int currentIndent)
  {
    if(isOp(tree->data.type))
    {
      std::string indent(currentIndent, ' ');
      std::cout<<indent<<tree->data.type<<"\n";
      for(int index = 0; index < tree->children.size(); index++)
      {
        recursive_print(tree->children.at(index),currentIndent+2);
      }
    }
    else
    {
      std::string indent(currentIndent, ' ');
      std::cout<<indent<<tree->data<<"\n";
    }
  }
}

#include"basic_ops.hpp"
#include"arith_chain_formatting.hpp"
#include"utility_functions.hpp"

#endif