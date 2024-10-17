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
    if(a.data != b.data || a.children.size() != b.children.size()){return false;}
    if(isMulChain(a))
    {
      //ensure matrices are in order
      bool shouldContinue = true;
      while(shouldContinue)
      {
        throw std::invalid_argument("you must do this part whenever you can bro");
      }
    }
    else
    {
      return a==b;
    }
    return false;
  }
  bool operator==(ExpressionTreeNodePtr a, ExpressionTreeNodePtr b)
  {
    return (*a == *b);
  }
  std::ostream& operator<<(std::ostream& os, ExpressionTreeNodePtr tree)
  {
    print(os, tree);
    return os;
  }
  void print(std::ostream& os, ExpressionTreeNodePtr tree)
  {
    recursive_print(os, tree, 0);
  }
  void recursive_print(std::ostream& os, ExpressionTreeNodePtr tree, int currentIndent)
  {
    if(isOp(tree->data))
    {
      std::string indent(currentIndent, ' ');
      os<<indent<<std::get<TokenType>(tree->data.dataVariant)<<"\n";
      for(int index = 0; index < tree->children.size(); index++)
      {
        recursive_print(os,tree->children.at(index),currentIndent+2);
      }
    }
    else
    {
      std::string indent(currentIndent, ' ');
      os<<indent<<tree->data<<"\n";
    }
  }
  std::vector<ExpressionTreeNodePtr> copyExpressionTreeNodePtrChildren(ExpressionTreeNodePtr tree)
  {
    std::vector<ExpressionTreeNodePtr> new_vector;
    for(int index = 0; index < tree->children.size(); index++)
    {
      new_vector.push_back(new ExpressionTreeNode(*(tree->children.at(index))));
    }
    return new_vector;
  }
}

#include"basic_ops.hpp"
#include"arith_chain_formatting.hpp"
#include"utility_functions.hpp"
#include"expr_simplify.hpp"
#include"operations/addition.hpp"
#include"operations/subtraction.hpp"
#include"operations/multiplication.hpp"
#include"operations/division.hpp"

#endif