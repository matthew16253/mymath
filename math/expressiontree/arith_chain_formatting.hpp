#ifndef ARITH_CHAIN_FORMATTING_HPP
#define ARITH_CHAIN_FORMATTING_HPP


#include<fd_decs.hpp>
#include<vector>
#include<memory>

// IMNPORTANT RULE:
// Nothing here can allocate objects onto the heap, but rather only move the pointers around. Do not even think about using the keyword 'new' ANYWHERE here. Or else!




void formatArithmeticChains(std::shared_ptr<mymath::ExpressionTreeNode>& tree)
{
  if(tree->data.type==mymath::TokenType::OP_ADD || tree->data.type==mymath::TokenType::OP_SUBTRACT)
  {
    formatAddSubChain(tree);
  }
  else if(tree->data.type==mymath::TokenType::OP_DIVIDE || tree->data.type==mymath::TokenType::OP_MULTIPLY)
  {
    formatMulDivChain(tree);
  }
  else
  {
    for(int i = 0; i < tree->children.size(); i++)
    {
      formatArithmeticChains(tree->children.at(i));
    }
  }
}

void formatAddSubChain(std::shared_ptr<mymath::ExpressionTreeNode>& tree) //given that:   tree->data.type == OP_ADD || OP_SUBTRACT
{
  std::vector<std::shared_ptr<mymath::ExpressionTreeNode>> addNodes, subNodes;
  getAddSubNodes(tree,addNodes,subNodes);
  std::shared_ptr<mymath::ExpressionTreeNode> new_tree;
  if(addNodes.size() > 1){formatArithmeticChains(addNodes.front());new_tree = addNodes.front();}
  else{formatArithmeticChains(subNodes.front());new_tree = subNodes.front();}
  // add all items in list to + chain, as well as -chain
  for(int i = ((addNodes.size() > 1)? 1 : 0); i < addNodes.size(); i++)
  {
    formatArithmeticChains(addNodes.at(i));
    applyBinaryOperation(new_tree,mymath::TokenType::OP_ADD,addNodes.at(i));
  }
  for(int i = ((subNodes.size() > 1)? 0 : 1); i < subNodes.size(); i++)
  {
    formatArithmeticChains(subNodes.at(i));
    applyBinaryOperation(new_tree,mymath::TokenType::OP_SUBTRACT,subNodes.at(i));
  }
  tree = new_tree;
}
void getAddSubNodes(std::shared_ptr<mymath::ExpressionTreeNode>& currentNode,std::vector<std::shared_ptr<mymath::ExpressionTreeNode>>& currentAddNodes, std::vector<std::shared_ptr<mymath::ExpressionTreeNode>>& currentSubNodes, mymath::TokenType aboveNodeType = mymath::TokenType::DT_UNINIT)
{
  if(currentNode->data.type != mymath::TokenType::OP_ADD && currentNode->data.type != mymath::TokenType::OP_SUBTRACT && aboveNodeType == mymath::TokenType::OP_ADD){currentAddNodes.push_back(currentNode);}
  else if(currentNode->data.type != mymath::TokenType::OP_ADD && currentNode->data.type != mymath::TokenType::OP_SUBTRACT && aboveNodeType == mymath::TokenType::OP_SUBTRACT){currentSubNodes.push_back(currentNode);}
  else
  {
    getAddSubNodes(currentNode->children.at(0),currentAddNodes,currentSubNodes, currentNode->data.type);
    getAddSubNodes(currentNode->children.at(1),currentAddNodes,currentSubNodes, currentNode->data.type);
  }
}

void formatMulDivChain(std::shared_ptr<mymath::ExpressionTreeNode>& tree, bool aboveIsNumerator = true)
{
  std::vector<std::shared_ptr<mymath::ExpressionTreeNode>> numeratorNodes, denominatorNodes;
  getMulDivNodes(tree,numeratorNodes,denominatorNodes,aboveIsNumerator);

  std::shared_ptr<mymath::ExpressionTreeNode> new_numeratorTree, new_denominatorTree, new_tree;

  formatArithmeticChains(numeratorNodes.at(0));
  new_numeratorTree =  numeratorNodes.at(0);
  for(int i = 1; i < numeratorNodes.size(); i++)
  {
    formatArithmeticChains(numeratorNodes.at(i));
    applyBinaryOperation(new_numeratorTree,mymath::TokenType::OP_MULTIPLY,numeratorNodes.at(i));
  }

  new_tree = new_numeratorTree;

  if(denominatorNodes.size() > 0)
  {  
    formatArithmeticChains(denominatorNodes.at(0));
    new_denominatorTree = denominatorNodes.at(0);
    for(int i = 1; i < numeratorNodes.size(); i++)
    {
      formatArithmeticChains(numeratorNodes.at(i));
      applyBinaryOperation(new_numeratorTree,mymath::TokenType::OP_MULTIPLY,numeratorNodes.at(i));
    }
    applyBinaryOperation(new_tree,mymath::TokenType::OP_DIVIDE,new_denominatorTree);
  }

  tree = new_tree;
}

void getMulDivNodes(std::shared_ptr<mymath::ExpressionTreeNode>& currentNode,std::vector<std::shared_ptr<mymath::ExpressionTreeNode>>& currentNumeratorNodes, std::vector<std::shared_ptr<mymath::ExpressionTreeNode>>& currentDenominatorNodes, bool aboveIsNumerator = true)
{
  if(currentNode->data.type != mymath::TokenType::OP_MULTIPLY && currentNode->data.type != mymath::TokenType::OP_DIVIDE && aboveIsNumerator){currentNumeratorNodes.push_back(currentNode);}
  else if(currentNode->data.type != mymath::TokenType::OP_MULTIPLY && currentNode->data.type != mymath::TokenType::OP_DIVIDE && !aboveIsNumerator){currentDenominatorNodes.push_back(currentNode);}
  else if(currentNode->data.type == mymath::TokenType::OP_MULTIPLY)
  {
    getMulDivNodes(currentNode->children.at(0),currentNumeratorNodes,currentDenominatorNodes,aboveIsNumerator);
    getMulDivNodes(currentNode->children.at(1),currentNumeratorNodes,currentDenominatorNodes,aboveIsNumerator);
  }
  else if(currentNode->data.type == mymath::TokenType::OP_DIVIDE);
  {
    getMulDivNodes(currentNode->children.at(0),currentNumeratorNodes,currentDenominatorNodes,aboveIsNumerator);
    getMulDivNodes(currentNode->children.at(1),currentNumeratorNodes,currentDenominatorNodes,!aboveIsNumerator);
  }
}


#endif