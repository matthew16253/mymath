#ifndef OPERATOR_FD_SLASH_EQUALS_HPP
#define OPERATOR_FD_SLASH_EQUALS_HPP

#include"../../../fd_decs.hpp"

namespace mymath
{
  template<typename A, typename B>
  concept OperatorDivideAssignOverloadExists_c = requires()
  {
    std::declval<A>() /= std::declval<B>();
  };

  template<typename A, typename B>
  constexpr bool OperatorDivideAssignOverloadExists_v = OperatorDivideAssignOverloadExists_c<A,B>;

  template<typename A, typename B>
  void tokenOperatorDivideAssign(Token& a, const Token& b)
  {
    using noqual_A = std::remove_cv_t<std::remove_reference_t<A>>;
    using noqual_B = std::remove_cv_t<std::remove_reference_t<B>>;
    if constexpr(std::is_same_v<noqual_A, TokenType> || std::is_same_v<noqual_B, TokenType>)
    {
      throw std::invalid_argument("no tokentypes for operator+ sowwy :(");
    }
    else if constexpr(isInfoLog_v<noqual_A> || std::is_null_pointer_v<noqual_B>)
    {
      //a = a;
    }
    else if constexpr(isInfoLog_v<noqual_B> || std::is_null_pointer_v<noqual_A>)
    {
      a = b;
    }
    else if constexpr(std::is_same_v<noqual_A, ExpressionTreeNodePtr> && std::is_same_v<noqual_B, ExpressionTreeNodePtr>)
    {
      const_cast<ExpressionTreeNodePtr&>(a.get<ExpressionTreeNodePtr>()) /= b.get<ExpressionTreeNodePtr>();
    }
    else if constexpr(std::is_same_v<noqual_A, ExpressionTreeNodePtr>)
    {
      const_cast<ExpressionTreeNodePtr&>(a.get<ExpressionTreeNodePtr>()) /= b;
    }
    else if constexpr(std::is_same_v<noqual_B, ExpressionTreeNodePtr>)
    {
      ExpressionTreeNodePtr new_a = new ExpressionTreeNode(*const_cast<ExpressionTreeNodePtr&>(b.get<ExpressionTreeNodePtr>()));
      new_a /= a.get<ExpressionTreeNodePtr>();
      a = new_a;
    }
    else if constexpr(std::is_same_v<noqual_A, std::complex<Token>*> && std::is_same_v<noqual_B, std::complex<Token>*>)
    {
      (*(a.get<std::complex<Token>*>())) /= (*(b.get<std::complex<Token>*>()));
    }
    else if constexpr(std::is_same_v<noqual_A, std::complex<Token>*> && std::is_same_v<noqual_B, dec_float*>)
    {
      (*(a.get<std::complex<Token>*>())) /= b;
    }
    else if constexpr(std::is_same_v<noqual_A, dec_float*> && std::is_same_v<noqual_B, std::complex<Token>*>)
    {
      std::complex<Token> new_a = *b.get<std::complex<Token>*>();
      new_a /= (*(b.get<std::complex<Token>*>()));
      a = std::move(new_a);
    }
    else if constexpr(std::is_same_v<noqual_A, dec_float*> && std::is_same_v<noqual_B, dec_float*>)
    {
      *(a.get<dec_float*>()) /= *(b.get<dec_float*>());
      if(isOverflow(*a.get<dec_float*>()))
      {
        a = Token(ERROR_DIV_OVERFLOW);
      }
      else if(isUnderflow(*a.get<dec_float*>()))
      {
        a = Token(ERROR_DIV_UNDERFLOW);
      }
      else if(isDomainError(*a.get<dec_float*>()))
      {
        a = Token(ERROR_DIV_BY_ZERO);
      }
      else
      {
        //
      }
    }
    // else if constexpr(OperatorDivideAssignOverloadExists_v<std::remove_pointer_t<noqual_A>, std::remove_pointer_t<noqual_B>>)
    // {
    //   *(a.get<noqual_A>()) /= *(b.get<noqual_B>());
    // }
    else
    {
      a = Token(InfoLog<2, int>(ERROR_INVALID_DIV_OPERANDS, {getTokenType<noqual_A>(), getTokenType<noqual_B>()}));
    }
  }

  struct tokenVariantOperatorDivideAssignVisitor
  {
    tokenVariantOperatorDivideAssignVisitor() {}
    template<typename A, typename B>
    void operator()(Token& a, const Token& b)
    {
      tokenOperatorDivideAssign<A,B>(a,b);
    }
  };

  void Token::operator/=(const Token& other)
  {
    std::visit([&](auto& arg1, auto& arg2){return tokenVariantOperatorDivideAssignVisitor().operator()<std::remove_reference_t<decltype(arg1)>, std::remove_reference_t<decltype(arg2)>>(*this,other);}, this->dataVariant, other.dataVariant);
  }
}

#endif