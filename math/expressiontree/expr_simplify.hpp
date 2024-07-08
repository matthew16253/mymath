#ifndef EXPR_SIMPLIFY_HPP
#define EXPR_SIMPLIFY_HPP

#include<unordered_map>
#include<map>

#include"../../fd_decs.hpp"

// add chains must be previously validated
// already must be formatted properly (arith chain formatting)
namespace mymath
{
	void simplifyConstantNumericalValues(ExpressionTreeNodePtr& tree)
	{
		if(isOp(tree->data))
		{
			// see if possible to simplify

			Token currentResult;
			Token previousResult;
			for(int index = 0; index < tree->children.size(); index++)
			{
				if(isData(tree->children.at(index)->data))
				{
					applyTokenOperation(currentResult, tree->data.get<TokenType>(), tree->children.at(index)->data);
					if(!isError(currentResult))
					{
							previousResult = currentResult;
					}
					else     // i assume this is an error due to precense of a variable (others should be checked for already)
					{
							currentResult = previousResult;
					}
				}
				else if(isOp(tree->children.at(index)->data))
				{
					simplifyConstantNumericalValues(tree->children.at(index));
				}
			}
		}
	}


	bool arithmeticIsValid(ExpressionTreeNodePtr tree)
	{
		bool isValid = true;
		recursiveArithmeticValidityCheck(tree, isValid);
		return isValid;
	}


	void recursiveArithmeticValidityCheck(ExpressionTreeNodePtr tree, bool& isValid)
	{
		if(isOp(tree->data))
		{
			Token currentResult;
			Token previousResult;
			for(int index = 0; index < tree->children.size(); index++)
			{
				if(isData(tree->children.at(index)->data))
				{
					applyTokenOperation(currentResult, tree->data.get<TokenType>(), tree->children.at(index)->data);
					if(!isError(currentResult))
					{
							previousResult = currentResult;
					}
					else
					{
						if(isVar(currentResult.get<InfoLog<2, TokenType>*>()->ops[0]) || isVar(currentResult.get<InfoLog<2, TokenType>*>()->ops[1]))      // CHANGE THE ERROR MESSAGES TO TAKE AN OPERATION AS WELL!!!!!!!!!!!!!!!!
						{
							currentResult = previousResult;
						}
						else
						{
							isValid = false;
							return;
						}
					}
				}
				else if(isOp(tree->children.at(index)->data))
				{
					recursiveArithmeticValidityCheck(tree->children.at(index), isValid);
					if(!isValid){return;}
				}
			}
		}
		else
		{
			return;
		}
	}


	// All numeric constants should be simplified beforehand
  void simpleCollectLikeTerms(ExpressionTreeNodePtr& tree)
  {
    if(isAddChainNode(tree))
    {
      std::map<std::vector<ExpressionTreeNodePtr>, ExpressionTreeNodePtr> likeVariableMap;
      for(int sumIndex = 0; sumIndex < tree->children.size(); sumIndex++)
      {
        if(isMulChainNode(tree->children.at(sumIndex)))
        {
          std::vector<ExpressionTreeNodePtr> varList;
          ExpressionTreeNodePtr partsToCollect;
          bool shouldStopSearching = false;
					if(isReal(tree->children.at(sumIndex)->children.at(0)->data))
					{
						partsToCollect = tree->children.at(sumIndex)->children.at(0);
						varList = std::vector<ExpressionTreeNodePtr>(tree->children.at(sumIndex)->children.begin(),tree->children.at(sumIndex)->children.end());
						if(static_cast<ExpressionTreeNode*>(likeVariableMap[varList]) == nullptr){likeVariableMap[varList] = new ExpressionTreeNode(Token(1));}
						else if(isReal(likeVariableMap[varList]->data)){addRealNodeToRealNode(likeVariableMap[varList], partsToCollect);}
						else if(isRealFraction(likeVariableMap[varList])){addRealNodeToRealFraction(likeVariableMap[varList], partsToCollect);}
					}
					else if(isRealFraction(tree->children.at(sumIndex)->children.at(0)))
					{
						partsToCollect = tree->children.at(sumIndex)->children.at(0);
						varList = std::vector<ExpressionTreeNodePtr>(tree->children.at(sumIndex)->children.begin(),tree->children.at(sumIndex)->children.end());
						if(static_cast<ExpressionTreeNode*>(likeVariableMap[varList]) == nullptr){likeVariableMap[varList] = new ExpressionTreeNode(Token(1));}
						else if(isReal(likeVariableMap[varList]->data)){addRealFractionToRealNode(likeVariableMap[varList], partsToCollect);}
						else if(isRealFraction(likeVariableMap[varList])){addRealFractionToRealFraction(likeVariableMap[varList], partsToCollect);}
					}
					else
					{
						shouldStopSearching = true;
						break;
					}
          if(shouldStopSearching)
          {
            varList = tree->children.at(sumIndex)->children;
            partsToCollect = new ExpressionTreeNode(Token(1));
          }
          tree->children.at(sumIndex)->children.clear();
          delete tree->children.at(sumIndex);
        }
        else if(isVar(tree->children.at(sumIndex)->data))
        {
          likeVariableMap[{(tree->children.at(sumIndex))}] = (new ExpressionTreeNode(Token(1)));
        }
      }


      tree->children.clear();
      for(auto& [likeTerm, collectedPart] : likeVariableMap)
      {
        if(isReal(collectedPart->data)  &&  collectedPart->data == 1)
        {
          ExpressionTreeNodePtr new_term = new ExpressionTreeNode(Token(DT_MUL_CHAIN));
					new_term->children = likeTerm;
          tree->children.push_back(new_term);
        }
        else
        {
          ExpressionTreeNodePtr new_term = new ExpressionTreeNode(Token(DT_MUL_CHAIN));
          new_term->children = {collectedPart};
          new_term->children.insert(new_term->children.end(), likeTerm.begin(), likeTerm.end());
          tree->children.push_back(new_term);
        }
      }
    }
  }
}

#endif