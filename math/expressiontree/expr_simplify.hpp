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
		if(isOp(tree->data.type))
		{
			// see if possible to simplify

			Token currentResult;
			Token previousResult;
			for(int index = 0; index < tree->children.size(); index++)
			{
				if(isData(tree->children.at(index)->data.type))
				{
					applyTokenOperation(currentResult, tree->data.type, tree->children.at(index)->data);
					if(!isError(currentResult.type))
					{
							previousResult = currentResult;
					}
					else     // i assume this is an error due to precense of a variable (others should be checked for already)
					{
							currentResult = previousResult;
					}
				}
				else if(isOp(tree->children.at(index)->data.type))
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
		if(isOp(tree->data.type))
		{
			Token currentResult;
			Token previousResult;
			for(int index = 0; index < tree->children.size(); index++)
			{
				if(isData(tree->children.at(index)->data.type))
				{
					applyTokenOperation(currentResult, tree->data.type, tree->children.at(index)->data);
					if(!isError(currentResult.type))
					{
							previousResult = currentResult;
					}
					else
					{
						if(isVar(currentResult.operands2_error_ptr->operands[0]) || isVar(currentResult.operands2_error_ptr->operands[1]))      // CHANGE THE ERROR MESSAGES TO TAKE AN OPERATION AS WELL!!!!!!!!!!!!!!!!
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
				else if(isOp(tree->children.at(index)->data.type))
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
			std::map<std::vector<ExpressionTreeNodePtr>, std::vector<ExpressionTreeNodePtr>> likeVariableMap;
			for(int sumIndex = 0; sumIndex < tree->children.size(); sumIndex++)
			{
				if(isMulChainNode(tree->children.at(sumIndex)))
				{
					std::vector<ExpressionTreeNodePtr> varList;
					ExpressionTreeNodePtr partsToCollect;

          bool shouldStopSearching = false;

					for(int mulIndex = 0; mulIndex < tree->children.at(sumIndex)->children.size(); mulIndex++)
					{
						if(isVar(tree->children.at(sumIndex)->children.at(mulIndex)->data.type))
						{
							varList.push_back(tree->children.at(sumIndex)->children.at(mulIndex));
						}
						else if(tree->children.at(sumIndex)->children.at(mulIndex)->data.type == DT_REAL)
						{
						  partsToCollect.addRealNodeToRealNode(tree->children.at(sumIndex)->children.at(mulIndex));
						}
						else if(isRealFraction(tree->children.at(sumIndex)->children.at(mulIndex)))
						{
							partsToCollect.addRealFractionToRealNode(tree->children.at(sumIndex)->children.at(mulIndex));
						}
						else
						{
							shouldStopSearching = true;
							break;
						}
					}
					if(shouldStopSearching)
					{
						varList = {new ExpressionTreeNode(Token(1))};
						partsToCollect = tree->children.at(sumIndex)->children;
					}

					likeVariableMap[varList].insert(likeVariableMap[varList].end(), partsToCollect.begin(), partsToCollect.end());

					tree->children.at(sumIndex)->children.clear();
					delete tree->children.at(sumIndex);
				}

				else if(isVar(tree->children.at(sumIndex)->data.type))
				{
					likeVariableMap[{(tree->children.at(sumIndex))}].push_back(new ExpressionTreeNode(Token(1)));
				}
			}

			tree->children.clear();

			
			for(auto& [likeTerm, collectedPart] : likeVariableMap)
			{
				if(collectedPart.size() == 1)
				{
					ExpressionTreeNodePtr new_term = new ExpressionTreeNode(Token(nullptr, DT_MUL_CHAIN));
					if(collectedPart.at(0)->data != 1)
					{
						new_term = likeTerm;
					}
					else
					{
						new_term = 
					}


					tree->children.push_back(new_term);
				}
			}
			// Add the nodes in the map to tree :)
		}
	}
}

#endif