#ifndef RECURSIVE_DESCENT_PARSER_HPP
#define RECURSIVE_DESCENT_PARSER_HPP

#include"../fd_decs.hpp"


bool isNumericOrDot(char c)
{
  return ('0' <= c && c <='9') || c == '.';
}


namespace myparse
{
  Token Parser::generateTokens(std::vector<Token>& target)
  {
    int currentCursor = 0;
    int startCursor;
    bool continueTokenSearch = true;
    while(continueTokenSearch)
    {
      bool numberExpected = isNumericOrDot(code.at(currentCursor));
      if(numberExpected)
      {
        bool continueCurrentTokenSearch = true;
        startCursor = currentCursor;
        while(continueCurrentTokenSearch)
        {
          currentCursor++;
          if(!isNumericOrDot(code[currentCursor]) || (currentCursor == code.size()))
          {
            if(code.at(currentCursor) == 'i' && !(std::isalpha(code[currentCursor+1]) || code.size() != (currentCursor)))
            {
              target.push_back(
                               Token(std::complex<Token>(Token(),
                                                         Token(dec_float(code.substr(startCursor, currentCursor - startCursor)))))
              );
              continueCurrentTokenSearch = false;
            }
            else
            {
              target.push_back(
                Token(dec_float(code.substr(startCursor, currentCursor - startCursor)))
              );
              continueCurrentTokenSearch = false;
            }
          }
        }
        if(currentCursor == code.size())
        {
          continueTokenSearch = false;
        }
      }
    }
  }



  Token& Parser::currentToken()
  {
    return tokens.at(cursor);
  }
  Token& Parser::previousToken()
  {
    return tokens.at(cursor - 1);
  }
  Token& Parser::beforePreviousToken()
  {
    return tokens.at(cursor - 2);
  }
  void Parser::advance()
  {
    cursor++;
  }
  void Parser::stepBack()
  {
    cursor--;
  }
  template<TokenType... types>
  bool Parser::match()
  {
    if(... || check(types))
    {
      advance();
      return true;
    }
    return false;
  }
  bool Parser::matchIdentifier()
  {
    if(checkIdentifier())
    {
      advance();
      return true;
    }
    return false;
  }
  bool Parser::matchData()
  {
    if(checkData())
    {
      advance();
      return true;
    }
    return false;
  }
  bool Parser::check(TokenType type)
  {
    if(isAtEnd() || !currentToken().is_type<TokenType>()){return false;}
    else
    {
      return currentToken().get<TokenType>() == type;
    }
  }
  bool Parser::checkIdentifier()
  {
    return currentToken().is_type<Variable>();
  }
  bool Parser::checkData()
  {
    return isData_notExpr(currentToken());
  }
  bool Parser::isAtEnd()
  {
    return cursor == tokens.size();
  }


  enum IdentifierType
  {
    ERROR, VARIABLE, FUNCTION
  };
  struct TypeErasedIdentifier
  {
    public:
      IdentifierType type;
      ExpressionTreeNodePtr data;
      operator bool(){return type!=ERROR;}
  };

  ExpressionTreeNodePtr isExistingConstant(const Variable& identifier)
  {
    try
    {
      ExpressionTreeNodePtr variableFound = constantMap.map_at(identifier);
      return variableFound;
    }
    catch(const std::out_of_range& e)
    {
      return nullptr;
    }
  }
  ExpressionTreeNodePtr isExistingFunction(const Variable& identifier)
  {
    try
    {
      ExpressionTreeNodePtr variableFound = functionMap.map_at(identifier);
      return variableFound;
    }
    catch(const std::out_of_range& e)
    {
      return nullptr;
    }
  }

  TypeErasedIdentifier isExistingFunctionOrVariableName(const Variable& identifier)
  {
    ExpressionTreeNodePtr variableFound, functionFound = nullptr;
    ExpressionTreeNodePtr variableFound = isExistingConstant(identifier);
	  ExpressionTreeNodePtr functionFound = isExistingFunction(identifier);

    if(!variableFound && functionFound)
    {
      return {FUNCTION, functionFound};
    }
    else if(variableFound && !functionFound)
    {
      return {VARIABLE, variableFound};
    }
    else if(!variableFound && !functionFound)
    {
      return {ERROR, nullptr};
    }
    else
    {
      throw std::runtime_error("variable and function found with the same name!");
    }
  }


    // GRAMMAR:

    // expression -> assignment | term
    // assignment -> (IDENTIFER "=")* term | functionDeclaration "=" term
    // term -> factor (("+" | "-") factor)*
    // factor -> unary ( ("/" | "*") unary)*
    // unary -> ("+" | "-")* unary | power
    // power -> primary ("^" primary)*
    // primary -> NUMBER | IDENTIFIER | "(" term ")" | CONSTANT | functionDeclaration

    // functionDeclaration -> FUNCTION_NAME "(" (IDENTIFIER?) ("," IDENTIFIER)* ")"




    // GRAMMAR:     UPDATED VERSION


    // expression -> assignment | term | deletion
    // assignment -> (newConstant "=")* term | functionDeclaration "=" term
    // term -> factor (("+" | "-") factor)*
    // factor -> unary ( ("/" | "*") unary)*
    // unary -> ("+" | "-") unary | power
    // power -> primary ("^" primary)*
    // primary -> NUMBER/DATA | bracketedTerm | variable | existingConstant | functionCall

    // functionDeclaration -> !isConstant(IDENTIFIER) "(" (variable?) ("," variable)* ")"
    // functionCall -> IDENTIFIER "(" (term?) ("," term)* ")"
    // variable -> !isFunctionOrConstant(IDENTIFIER)
    // existingConstant -> isConstant(IDENTIFIER)
    // newConstant -> !isFunction(IDENTIFIER)
    // bracketedTerm -> "(" term ")"
    // deletion -> "delete" existingConstant

  std::tuple<ExpressionTreeNodePtr, bool> Parser::parse()
  {
    std::tuple<ExpressionTreeNodePtr, bool> tree_tuple = expression();
    if(std::get<bool>(tree_tuple))
    {
      return tree_tuple;
    }
    delete std::get<ExpressionTreeNodePtr>(tree_tuple);
    return {nullptr, false};
  }



// expression -> assignment | term
  std::tuple<ExpressionTreeNodePtr, bool> Parser::expression()
  {
    std::tuple<ExpressionTreeNodePtr, bool> tree_tuple = assignment();
    if(std::get<bool>(tree_tuple))
    {
      return tree_tuple;
    }
    delete std::get<ExpressionTreeNodePtr>(tree_tuple);
    tree_tuple = term();

    if(std::get<bool>(tree_tuple))
    {
      return tree_tuple;
    }
    delete std::get<ExpressionTreeNodePtr>(tree_tuple);
    return {nullptr, false};
  }

// assignment -> (IDENTIFER "=")* term | functionDeclaration "=" term
  std::tuple<ExpressionTreeNodePtr, bool> Parser::assignment()
  {
    std::tuple<ExpressionTreeNodePtr, bool> tree_tuple;
    if(matchIdentifier())
    {
      if(match<OP_ASSIGN>())
      {
        std::get<ExpressionTreeNodePtr>(tree_tuple) = new ExpressionTreeNode(Token(OP_ASSIGN));
        std::get<ExpressionTreeNodePtr>(tree_tuple)->children.push_back(new ExpressionTreeNode(beforePreviousToken()));
        std::tuple<ExpressionTreeNodePtr, bool> assignExpression = term();
        if(std::get<bool>(assignExpression))
        {
          std::get<ExpressionTreeNodePtr>(tree_tuple)->children.push_back(std::get<ExpressionTreeNodePtr>(assignExpression));
          return tree_tuple;
        }
        delete std::get<ExpressionTreeNodePtr>(tree_tuple);
        return {nullptr, false};
      }
      else
      {
        stepBack();
        return {nullptr, false};
      }
    }
  }

// term -> factor (("+" | "-") factor)*
  std::tuple<ExpressionTreeNodePtr,  bool> Parser::term()
  {
    std::tuple<ExpressionTreeNodePtr,  bool> tree_tuple = factor();
    if(std::get<bool>(tree_tuple))
    {
      bool shouldContinue = true;
      while(shouldContinue)
      {
        if(match<OP_ADD>())
        {
          std::tuple<ExpressionTreeNodePtr,  bool> nextFactorTree_tuple = factor();
          if(!std::get<bool>(nextFactorTree_tuple))
          {
            delete std::get<ExpressionTreeNodePtr>(tree_tuple);
            delete std::get<ExpressionTreeNodePtr>(nextFactorTree_tuple);
            return {nullptr, false};
          }
          applyBinaryOperation(std::get<ExpressionTreeNodePtr>(tree_tuple), OP_ADD, std::get<ExpressionTreeNodePtr>(nextFactorTree_tuple));
        }
        else if(match<OP_SUBTRACT>())
        {
          std::tuple<ExpressionTreeNodePtr,  bool> nextFactorTree_tuple = factor();
          if(!std::get<bool>(nextFactorTree_tuple))
          {
            delete std::get<ExpressionTreeNodePtr>(tree_tuple);
            delete std::get<ExpressionTreeNodePtr>(nextFactorTree_tuple);
            return {nullptr, false};
          }
          applyBinaryOperation(std::get<ExpressionTreeNodePtr>(tree_tuple), OP_ADD, -std::get<ExpressionTreeNodePtr>(nextFactorTree_tuple));
        }
        else
        {
          shouldContinue = false;
        }
      }
    }
    else
    {
        delete std::get<ExpressionTreeNodePtr>(tree_tuple);
        return {nullptr, false};
    }
    return tree_tuple;
  }

// factor -> unary ( ("/" | "*") unary)*
  std::tuple<ExpressionTreeNodePtr,  bool> Parser::factor()
  {
    std::tuple<ExpressionTreeNodePtr,  bool> tree_tuple = unary();
    if(std::get<bool>(tree_tuple))
    {
      bool shouldContinue = true;
      while(shouldContinue)
      {
        if(match<OP_MULTIPLY>())
        {
          std::tuple<ExpressionTreeNodePtr,  bool> nextUnaryTree_tuple = unary();
          if(!std::get<bool>(nextUnaryTree_tuple))
          {
            delete std::get<ExpressionTreeNodePtr>(tree_tuple);
            delete std::get<ExpressionTreeNodePtr>(nextUnaryTree_tuple);
            return {nullptr, false};
          }
          applyBinaryOperation(std::get<ExpressionTreeNodePtr>(tree_tuple), OP_MULTIPLY, std::get<ExpressionTreeNodePtr>(nextUnaryTree_tuple));
        }
        else if(match<OP_DIVIDE>())
        {
          std::tuple<ExpressionTreeNodePtr,  bool> nextUnaryTree_tuple = unary();
          if(!std::get<bool>(nextUnaryTree_tuple))
          {
            delete std::get<ExpressionTreeNodePtr>(tree_tuple);
            delete std::get<ExpressionTreeNodePtr>(nextUnaryTree_tuple);
            return {nullptr, false};
          }
          applyBinaryOperation(std::get<ExpressionTreeNodePtr>(tree_tuple), OP_DIVIDE, std::get<ExpressionTreeNodePtr>(nextUnaryTree_tuple));
        }
        else
        {
          shouldContinue = false;
        }
      }
    }
    else
    {
      delete std::get<ExpressionTreeNodePtr>(tree_tuple);
      return {nullptr, false};
    }
    return tree_tuple;
  }


// unary -> ("+" | "-") unary | power
  std::tuple<ExpressionTreeNodePtr,  bool> Parser::unary()
  {
    std::tuple<ExpressionTreeNodePtr,  bool> tree_tuple;
    if(match<OP_NEGATE>())
    {
      std::tuple<ExpressionTreeNodePtr,  bool> tree_tuple = unary();
      if(!std::get<bool>(tree_tuple))
      {
        delete std::get<ExpressionTreeNodePtr>(tree_tuple);
        return {nullptr, false};
      }
      applyUnaryOperation(std::get<ExpressionTreeNodePtr>(tree_tuple), OP_NEGATE);
      return tree_tuple;
    }
    else if(match<OP_UNARY_PLUS>())
    {
      std::tuple<ExpressionTreeNodePtr,  bool> tree_tuple = unary();
      if(!std::get<bool>(tree_tuple))
      {
        delete std::get<ExpressionTreeNodePtr>(tree_tuple);
        return {nullptr, false};
      }
      applyUnaryOperation(std::get<ExpressionTreeNodePtr>(tree_tuple), OP_UNARY_PLUS);
      return tree_tuple;
    }
    tree_tuple = power();
    if(!std::get<bool>(tree_tuple))
    {
      delete std::get<ExpressionTreeNodePtr>(tree_tuple);
      return {nullptr, false};
    }
    return tree_tuple;
  }

// power -> primary ("^" primary)*
  std::tuple<ExpressionTreeNodePtr, bool> Parser::power()
  {
    std::tuple<ExpressionTreeNodePtr, bool> tree_tuple = primary();
    if(!std::get<bool>(tree_tuple))
    {
      delete std::get<ExpressionTreeNodePtr>(tree_tuple);
      return {nullptr, false};
    }

    bool shouldContinue = true;
    while(match<OP_POWER>())
    {
      std::tuple<ExpressionTreeNodePtr,  bool> exponentTree_tuple = primary();
      if(!std::get<bool>(exponentTree_tuple))
      {
        delete std::get<ExpressionTreeNodePtr>(exponentTree_tuple);
        return {nullptr, false};
      }
      applyBinaryOperation(std::get<ExpressionTreeNodePtr>(tree_tuple), OP_POWER, std::get<ExpressionTreeNodePtr>(exponentTree_tuple));
      return tree_tuple;
    }
  }

// functionDeclaration -> !isConstant(IDENTIFIER) "(" (variable?) ("," variable)* ")"
  std::tuple<ExpressionTreeNodePtr, bool> Parser::functionDeclaration()
  {
    using namespace mr;
    if(!matchIdentifier())
    {
      return {new ExpressionTreeNode(Token(Variable(""))), false};
    }
    if(!isExistingConstant(previousToken().get<Variable>()))
    {
      return {new ExpressionTreeNode(Token(Variable(""))), false};
    }
    if(!match<LEFT_PARENTHESIS>())
    {
      return {new ExpressionTreeNode(Token(Variable(""))), false};
    }
    Variable variableFound = variable();
    if(variableFound.data.empty())
    {
      ExpressionTreeNodePtr newFunction()
      return {new ExpressionTreeNode(Token(previousToken().get<Variable>())), true};
    }
    while(match<COMMA>())
    {
      variableFound = variable();
      if(variableFound.data.empty())
      {
        return 
      }
    }
  }
}


#endif