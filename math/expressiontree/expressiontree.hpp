#ifndef EXPR_HPP
#define EXPR_HPP

#include<fd_decs.hpp>

#include<vector>


namespace mymath
{
  ExpressionTreeNode::ExpressionTreeNode() : data(Token(nullptr, mymath::TokenType::DT_UNINIT))  {}
  ExpressionTreeNode::ExpressionTreeNode(Token _data, MoveTag) : data(_data, MoveTag{})  {}
  ExpressionTreeNode::ExpressionTreeNode(const ExpressionTreeNode& other, CopyTag)
      {
        data = other.data;
        for(int i = 0; i < other.children.size(); i++)
        {
          children.push_back(std::make_shared<ExpressionTreeNode>(*other.children.at(i), CopyTag{}));
        }
      }
  ExpressionTreeNode::ExpressionTreeNode(ExpressionTreeNode&& other)
      {
        data = other.data;
        children = other.children;
  
        other.children = std::vector<std::shared_ptr<ExpressionTreeNode>>();
      }
  ExpressionTreeNode& ExpressionTreeNode::operator=(const ExpressionTreeNode& other)
      {
        data = other.data;
        children = other.children;
        // data = other.data;
        // children = std::vector<ExpressionTreeNode*>();
        // for(int i = 0; i < other.children.size(); i++)
        // {
        //   children.push_back(new ExpressionTreeNode(*other.children.at(i)));
        // }
        // return *this;
      }
  
}

#include<math/expressiontree/basic_ops.hpp>
#include<math/expressiontree/arith_chain_formatting.hpp>

#endif