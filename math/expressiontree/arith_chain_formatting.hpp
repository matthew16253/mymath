#ifndef ARITH_CHAIN_FORMATTING_HPP
#define ARITH_CHAIN_FORMATTING_HPP


#include"../../fd_decs.hpp"
#include<vector>
#include<memory>

// IMNPORTANT RULE:
// Nothing here can allocate objects onto the heap, but rather only move the pointers around. Do not even think about using the keyword 'new' ANYWHERE here. Or else!

namespace mymath
{
  void formatArithmeticChains(ExpressionTreeNodePtr& tree)
  {
    if(isAddNode(tree) || isAddChainNode(tree))
    {
      formatAddChain(tree);
    }
    else if(isMulNode(tree) || isMulChainNode(tree) || isDivNode(tree))
    {
      formatMulDivChain(tree);
    }
    else if(isPowNode(tree))
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

  void formatAddChain(ExpressionTreeNodePtr& tree) //given that:   tree->data.type == OP_ADD
  {
    ExpressionTreeNodePtr new_tree = new ExpressionTreeNode(Token(nullptr, DT_SUM_CHAIN));

    getAddNodes(tree,new_tree->children);

    tree = new_tree;
  }
  void getAddNodes(ExpressionTreeNodePtr& currentNode,std::vector<ExpressionTreeNodePtr>& currentAddNodes)
  {
    if(!(isAddNode(currentNode) || isAddChainNode(currentNode)))
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

  void formatMulDivChain(ExpressionTreeNodePtr& tree, bool aboveIsNumerator)
  {
    ExpressionTreeNodePtr new_tree;
    ExpressionTreeNodePtr new_numeratorTree = new ExpressionTreeNode(Token(nullptr, DT_MUL_CHAIN));
    ExpressionTreeNodePtr new_denominatorTree = new ExpressionTreeNode(Token(nullptr, DT_MUL_CHAIN));

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

  void getMulDivNodes(ExpressionTreeNodePtr& currentNode,std::vector<ExpressionTreeNodePtr>& currentNumeratorNodes, std::vector<ExpressionTreeNodePtr>& currentDenominatorNodes, bool aboveIsNumerator)
  {
    if(!isMulNode(currentNode) && !isMulChainNode(currentNode) && !isDivNode(currentNode) && aboveIsNumerator){currentNumeratorNodes.push_back(currentNode);}
    else if(!isMulNode(currentNode) && !isMulChainNode(currentNode)  && !isDivNode(currentNode) && !aboveIsNumerator){currentDenominatorNodes.push_back(currentNode);}
    else if(currentNode->data.type == mymath::TokenType::OP_MULTIPLY)
    {
      for(int index = 0; index < currentNode->children.size(); index++)
      {
        getMulDivNodes(currentNode->children.at(index), currentNumeratorNodes, currentDenominatorNodes, aboveIsNumerator);
      }

      currentNode->children.clear();
      delete currentNode;
    }
    else if(isDivNode(currentNode))
    {
      getMulDivNodes(currentNode->children.at(0),currentNumeratorNodes,currentDenominatorNodes,aboveIsNumerator);
      getMulDivNodes(currentNode->children.at(1),currentNumeratorNodes,currentDenominatorNodes,!aboveIsNumerator);

      currentNode->children.clear();
      delete currentNode;
    }
  }

  void formatPowNodes(ExpressionTreeNodePtr& tree)
  {
    ExpressionTreeNodePtr new_tree = new ExpressionTreeNode(Token(nullptr, OP_POWER));
    ExpressionTreeNodePtr new_exponent = new ExpressionTreeNode(Token(nullptr, DT_MUL_CHAIN));
    ExpressionTreeNodePtr new_base;
    getPowNodes(tree, new_exponent->children, new_base);
    new_tree->children.push_back(new_base);
    new_tree->children.push_back(new_exponent);
  }
  void getPowNodes(ExpressionTreeNodePtr& currentNode, std::vector<ExpressionTreeNodePtr>& currentPowNodes, ExpressionTreeNodePtr& base)
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
      formatArithmeticChains(currentNode->children.at(0));
      formatArithmeticChains(currentNode->children.at(1));
      base = currentNode->children.at(0);
      currentPowNodes.push_back(currentNode->children.at(1));
    }
  }
  void expandMulChainToPow(ExpressionTreeNodePtr& tree)
  {
    if(isPowNode(tree) && isMulChainNode(tree->children.at(0)))
    {
      expandMulChainToPow(tree->children.at(1));
      for(int index = 0; index < tree->children.at(0)->children.size(); index++)
      {
        ExpressionTreeNodePtr new_pow_node = new ExpressionTreeNode(Token(nullptr, OP_POWER));
        expandMulChainToPow(tree->children.at(0)->children.at(index));
        new_pow_node->children.push_back(tree->children.at(0)->children.at(index));
        new_pow_node->children.push_back(new ExpressionTreeNode(*(tree->children.at(1))));
        tree->children.at(0)->children.at(index) = new_pow_node;
      }
      ExpressionTreeNodePtr new_tree = tree->children.at(0);
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