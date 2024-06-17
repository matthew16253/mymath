#include<iostream>
#include"fd_decs.hpp"

int main()
{
  using namespace mymath;


  ExpressionTreeNode* node = new ExpressionTreeNode(Token(nullptr, OP_DIVIDE));
  node->children.push_back(new ExpressionTreeNode(Token(nullptr, OP_DIVIDE)));
  node->children.push_back(new ExpressionTreeNode(Token(nullptr, OP_MULTIPLY)));
  node->children.at(0)->children.push_back(new ExpressionTreeNode(Token(new long double(1), DT_REAL)));
  node->children.at(0)->children.push_back(new ExpressionTreeNode(Token(new long double(2), DT_REAL)));
  node->children.at(1)->children.push_back(new ExpressionTreeNode(Token(new long double(3), DT_REAL)));
  node->children.at(1)->children.push_back(new ExpressionTreeNode(Token(new long double(4), DT_REAL)));

  std::cout<<node->children.size()<<"\n";
  
  formatArithmeticChains(node);

  std::cout<<node->children.size()<<"\n";

  return 0;
}