#ifndef EXPR_HPP
#define EXPR_HPP

#include"../../fd_decs.hpp"

#include<vector>


namespace mymath
{
  ExpressionTreeNode::ExpressionTreeNode() : data(Token()), children({}) {}
  ExpressionTreeNode::ExpressionTreeNode(const Token& _data) : data(_data), children({})  {}
  ExpressionTreeNode::ExpressionTreeNode(Token&& _data) : data(_data), children({})  {}
  ExpressionTreeNode::ExpressionTreeNode(ExpressionTreeNode& other)
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
    other.children.clear();
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
  bool operator==(const ExpressionTreeNode& a, const ExpressionTreeNode& b)
  {
    if(a.children.size() != 0 && b.children.size() != 0)
    {
      if(a.children.size() != b.children.size()){return false;}
      else if(!(a.data == b.data)){return false;}

      for(int index = 0; index < a.children.size(); index++)
      {
        if(!(a.children.at(index) == b.children.at(index))){return false;}
      }
    }
    return true;
  }
  bool operator==(ExpressionTreeNodePtr a, ExpressionTreeNodePtr b)
  {
    return (*a == *b);
  }
  void print(ExpressionTreeNodePtr tree)
  {
    recursive_print(tree, 0);
  }
  void recursive_print(ExpressionTreeNodePtr tree, int currentIndent)
  {
    if(isOp(tree->data))
    {
      std::string indent(currentIndent, ' ');
      std::cout<<indent<<std::get<TokenType>(tree->data.dataVariant)<<"\n";
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