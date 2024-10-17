#ifndef EXPR_SIMPLIFY_HPP
#define EXPR_SIMPLIFY_HPP

#include<unordered_map>
#include<map>

#include"../../fd_decs.hpp"

// add chains must be previously validated
// already must be formatted properly (arith chain formatting)
namespace mymath
{
  void collectLikeTerms(ExpressionTreeNodePtr& tree)
  {
    ExpressionTreeNodePtr new_tree;
    mr::linear_map<ExpressionTreeNodePtr, std::vector<ExpressionTreeNodePtr>> likeTermMap;            // likeTerm, likeTermFactors
    if(isAddChain(tree))
    {
      for(int addIndex = 0; addIndex < tree->children.size(); addIndex++)
      {
        if(isMulChain(tree->children[addIndex]))
        {
          // the like part of like terms can only be a single variable^something, or a single function^something
          ;
        }
      }
    }
  }

  template<TokenType Ignore>
  void trySimplifyConstants(ExpressionTreeNodePtr& tree)
  {
    if constexpr(Ignore != DT_SUM_CHAIN)
    {
      if(isAddChain(tree))
      {
        trySimplifyConstantsInAddNode(tree);
      }
    }
    else if constexpr(Ignore != DT_MUL_CHAIN)
    {
      if(isMulChain(tree))
      {
        trySimplifyConstantsInMulNode(tree);
      }
    }
    else if constexpr(Ignore != OP_DIVIDE)
    {
      if(isDiv(tree))
      {
        trySimplifyConstantsInDivNode(tree);
      }
    }
    else if constexpr(Ignore != OP_POWER)
    {
      if(isPow(tree))
      {
        trySimplifyConstantsInPowNode(tree);
      }
    }
  }
  
  void trySimplifyConstantsInAddNode(ExpressionTreeNodePtr& addNode)
  {
    Token currentSum;
    for(int index = 0; index < addNode->children.size(); index++)
    {
      if(isData(addNode->children[index]->data))
      {
        currentSum += addNode->children[index]->data;
        delete addNode->children[index];
        addNode->children.erase(addNode->children.begin() + index);
        if(isError(currentSum))
        {
          delete addNode;
          addNode = new ExpressionTreeNode(std::move(currentSum));
          return;
        }
        else
        {
          trySimplifyConstants<DT_SUM_CHAIN>(addNode->children[index]);
        }
      }
    }
    if(!currentSum.is_type<std::nullptr_t>())
    {
      addNode->children.push_back(new ExpressionTreeNode(std::move(currentSum)));
    }
  }

  template<bool shouldAllowMatricesAndVectors>
  void trySimplifyConstantsInMulNode(ExpressionTreeNodePtr& mulNode) // real is at end of children
  {
    Token currentProduct;
    for(int index = 0; index < mulNode->children.size(); index++)
    {
      if constexpr(!shouldAllowMatricesAndVectors)
      {
        if(evaluatesToMatrix(mulNode->children[index]))
        {
          *mulNode = ExpressionTreeNode(Token(InfoLog<2, int>(ERROR_INVALID_DIV_OPERANDS, {DT_UNSPECIFIED, DT_MAT})));
        }
      }
      if(isData(mulNode->children[index]->data))
      {
        currentProduct *= mulNode->children[index]->data;
        delete mulNode->children[index];
        mulNode->children.erase(mulNode->children.begin() + index);
        if(isError(mulNode->children[index]->data))
        {
          delete mulNode;
          mulNode = new ExpressionTreeNode(std::move(currentProduct));
          return;
        }
      }
      else
      {
        trySimplifyConstants<DT_MUL_CHAIN>(mulNode->children[index]);
      }
    }
    if(!currentProduct.is_type<std::nullptr_t>())
    {
      mulNode->children.push_back(new ExpressionTreeNode(std::move(currentProduct)));
    }
    return;
  }

  void trySimplifyConstantsInDivNode(ExpressionTreeNodePtr& divNode)
  {
    ExpressionTreeNodePtr numeratorFactor;
    ExpressionTreeNodePtr denominatorFactor;
    if(isMulChain(divNode->children[0]))
    {
      trySimplifyConstantsInMulNode(divNode->children[0]);
    }
    else
    {
      trySimplifyConstants<DT_MUL_CHAIN>(divNode->children[0]);
      return;
    }
    if(isMulChain(divNode->children[1]))
    {
      trySimplifyConstantsInMulNode<false>(divNode->children[1]);
    }
    else
    {
      trySimplifyConstants<DT_MUL_CHAIN>(divNode->children[0]);
      return;
    }

    if((isReal(divNode->children[0]->data) || isComplex(divNode->children[0]->data)) && (isReal(divNode->children[1]->data) || isComplex(divNode->children[1]->data)))
		{
			Token dataQuotient = numeratorFactor->data / denominatorFactor->data;
			if(isError(dataQuotient))
			{
				return;
				//  compatible type but size error
			}
      else
      {
        delete divNode->children[1];
        divNode->children.erase(divNode->children.begin());
        divNode->children[0]->data = dataQuotient;
      }
		}
		else
		{
			return;
		}
  }

  void trySimplifyConstantsInPowNode(ExpressionTreeNodePtr& powNode)
  {
    if(isData(powNode->children[0]->data) && isData(powNode->children[1]->data))
    {
      *powNode = ExpressionTreeNode(Token(pow(powNode->children[0]->data, powNode->children[1]->data)));
    }
  }
}

#endif