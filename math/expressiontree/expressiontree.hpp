#ifndef EXPR_HPP
#define EXPR_HPP

#include<fd_decs.hpp>

#include<vector>


namespace mymath
{
  ExpressionTreeNode::ExpressionTreeNode() : data(Token(nullptr, mymath::TokenType::DT_UNINIT))  {}
  ExpressionTreeNode::ExpressionTreeNode(const Token& _data) : data(_data)  {}
  ExpressionTreeNode::ExpressionTreeNode(Token&& _data) : data(_data)  {_data.dataptr = nullptr; data.type = DT_UNINIT;}
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
  ExpressionTreeNode& ExpressionTreeNode::operator=(const ExpressionTreeNode& other)
      {
        data = other.data;
        children = std::vector<ExpressionTreeNode*>();
        for(int i = 0; i < other.children.size(); i++)
        {
          children.at(i) = new ExpressionTreeNode(*other.children.at(i));
        }
        return *this;
      }
  ExpressionTreeNode& ExpressionTreeNode::operator=(ExpressionTreeNode&& other)
      {
        data = other.data;
        children = other.children;
      }
}

#include<math/expressiontree/basic_ops.hpp>
#include<math/expressiontree/arith_chain_formatting.hpp>

#endif