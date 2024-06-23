#ifndef ARITH_CHAIN_FORMATTING_HPP
#define ARITH_CHAIN_FORMATTING_HPP


#include"../../fd_decs.hpp"
#include<vector>
#include<memory>

// IMNPORTANT RULE:
// Nothing here can allocate objects onto the heap, but rather only move the pointers around. Do not even think about using the keyword 'new' ANYWHERE here. Or else!

namespace mymath
{
  void formatArithmeticChains(mymath::ExpressionTreeNode*& tree)
  {
    if(isAddNode(tree))
    {
      formatAddChain(tree);
    }
    else if(isMulNode(tree) || isDivNode(tree))
    {
      formatMulDivChain(tree);
    }
    else if(isPowNode)
    {
      formatPowNodes(tree);
    }
    else
    {
      for(int i = 0; i < tree->children.size(); i++)
      {
        formatArithmeticChains(tree->children.at(i));
      }
    }
  }

  void formatAddChain(mymath::ExpressionTreeNode*& tree) //given that:   tree->data.type == OP_ADD
  {
    ExpressionTreeNode* new_tree = new ExpressionTreeNode(Token(nullptr, DT_SUM_CHAIN));

    getAddNodes(tree,new_tree->children);

    tree = new_tree;
  }
  void getAddNodes(mymath::ExpressionTreeNode*& currentNode,std::vector<mymath::ExpressionTreeNode*>& currentAddNodes)
  {
    if(!isAddNode(currentNode))
    {
      formatArithmeticChains(currentNode);
      currentAddNodes.push_back(currentNode);
      currentNode->children = {};
    }
    else
    {
      for(int index = 0; index < currentNode->children.size(); index++)
      {
        getAddNodes(currentNode->children.at(index),currentAddNodes);
      }
      currentNode->children  = {};
      delete currentNode;
    }
  }

  void formatMulDivChain(mymath::ExpressionTreeNode*& tree, bool aboveIsNumerator)
  {
    ExpressionTreeNode* new_tree;
    ExpressionTreeNode* new_numeratorTree = new ExpressionTreeNode(Token(nullptr, DT_MUL_CHAIN));
    ExpressionTreeNode* new_denominatorTree = new ExpressionTreeNode(Token(nullptr, DT_MUL_CHAIN));

    getMulDivNodes(tree,new_numeratorTree->children,new_denominatorTree->children,aboveIsNumerator);

    if(new_denominatorTree->children.size() == 0)
    {
      delete new_denominatorTree;
      new_tree = new_numeratorTree;
    }
    else
    {
      new_tree = new ExpressionTreeNode(Token(nullptr, OP_DIVIDE));
      new_tree->children = {new_numeratorTree, new_denominatorTree};
    }
    tree = new_tree;
  }

  void getMulDivNodes(mymath::ExpressionTreeNode*& currentNode,std::vector<mymath::ExpressionTreeNode*>& currentNumeratorNodes, std::vector<mymath::ExpressionTreeNode*>& currentDenominatorNodes, bool aboveIsNumerator)
  {
    if(!isMulNode(currentNode) && !isDivNode(currentNode) && aboveIsNumerator){currentNumeratorNodes.push_back(currentNode);}
    else if(!isMulNode(currentNode) && !isDivNode(currentNode) && !aboveIsNumerator){currentDenominatorNodes.push_back(currentNode);}
    else if(currentNode->data.type == mymath::TokenType::OP_MULTIPLY)
    {
      for(int index = 0; index < currentNode->children.size(); index++)
      {
        getMulDivNodes(currentNode->children.at(index), currentNumeratorNodes, currentDenominatorNodes, aboveIsNumerator);
      }

      currentNode->children.clear();
      delete currentNode;
    }
    else if(currentNode->data.type == mymath::TokenType::OP_DIVIDE)
    {
      getMulDivNodes(currentNode->children.at(0),currentNumeratorNodes,currentDenominatorNodes,aboveIsNumerator);
      getMulDivNodes(currentNode->children.at(1),currentNumeratorNodes,currentDenominatorNodes,!aboveIsNumerator);

      currentNode->children.clear();
      delete currentNode;
    }
  }

  void formatPowNodes(ExpressionTreeNode*& tree)
  {
    ExpressionTreeNode* new_tree = new ExpressionTreeNode(Token(nullptr, OP_POWER));
    ExpressionTreeNode* new_exponent = new ExpressionTreeNode(Token(nullptr, DT_MUL_CHAIN));
    ExpressionTreeNode* new_base;
    getPowNodes(tree, new_exponent->children, new_base);
    new_tree->children.push_back(new_base);
    new_tree->children.push_back(new_exponent);
  }
  void getPowNodes(ExpressionTreeNode*& currentNode, std::vector<ExpressionTreeNode*>& currentPowNodes, ExpressionTreeNode*& base)
  {
    if(isPowNode(currentNode->children.at(0)))
    {
      formatArithmeticChains(currentNode->children.at(1));
      currentPowNodes.push_back(currentNode->children.at(1));
      
      currentNode->children.clear();
      delete currentNode;
    }
    else
    {
      base = currentNode->children.at(0);
      currentPowNodes.push_back(currentNode->children.at(1));
    }
  }
  void expandMulChainToPow(ExpressionTreeNode*& tree)
  {
    if(isPowNode(tree) && isMulChainNode(tree->children.at(0)))
    {
      expandMulChainToPow(tree->children.at(1));
      for(int index = 0; index < tree->children.at(0)->children.size(); index++)
      {
        ExpressionTreeNode* new_pow_node = new ExpressionTreeNode(Token(nullptr, OP_POWER));
        expandMulChainToPow(tree->children.at(0)->children.at(index));
        new_pow_node->children.push_back(tree->children.at(0)->children.at(index));
        new_pow_node->children.push_back(new ExpressionTreeNode(*(tree->children.at(1))));
        tree->children.at(0)->children.at(index) = new_pow_node;
      }
      ExpressionTreeNode* new_tree = tree->children.at(0);
      tree->children.at(0) = nullptr;
      delete tree;

      tree = new_tree;
    }
    else
    {
      for(int index = 0; index < tree->children.size(); index++)
      {
        expandMulChainToPow(tree->children.at(index));
      }
    }
  }
}

#endif