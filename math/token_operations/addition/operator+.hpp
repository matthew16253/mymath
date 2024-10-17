#ifndef TOKEN_OPERATOR_PLUS_HPP
#define TOKEN_OPERATOR_PLUS_HPP

#include"../../../fd_decs.hpp"

#include<type_traits>
#include<concepts>

namespace mymath
{
  template<typename A, typename B>
  concept OperatorPlusOverloadExists_c = requires()
  {
    std::declval<A>() + std::declval<B>();
  };

  template<typename A, typename B>
  struct OperatorPlusOverloadExists
  {
    static constexpr bool value = OperatorPlusOverloadExists_c<A,B>;
  };
  template<typename A, typename B>
  constexpr bool OperatorPlusOverloadExists_v = OperatorPlusOverloadExists<A, B>::value;

  template<typename A, typename B>
  Token tokenOperatorPlus(const Token& a, const Token& b)
  {
    using noqual_A = std::remove_cv_t<std::remove_reference_t<A>>;
    using noqual_B = std::remove_cv_t<std::remove_reference_t<B>>;
    Token result;
    if constexpr(std::is_same_v<noqual_A, TokenType> || std::is_same_v<noqual_B, TokenType>)
    {
      throw std::invalid_argument("no tokentypes for operator+ sowwy :(");
    }
    else if constexpr(isInfoLog_v<noqual_A> || std::is_null_pointer_v<noqual_B>)
    {
      result = a;
    }
    else if constexpr(isInfoLog_v<noqual_B> || std::is_null_pointer_v<noqual_A>)
    {
      result = b;
    }
    else if constexpr(std::is_same_v<noqual_A, ExpressionTreeNodePtr> && std::is_same_v<noqual_B, ExpressionTreeNodePtr>)
    {
      result = a.get<ExpressionTreeNodePtr>() + b.get<ExpressionTreeNodePtr>();
    }
    else if constexpr(std::is_same_v<noqual_A, ExpressionTreeNodePtr>)
    {
      result = a.get<ExpressionTreeNodePtr>() + b;
    }
    else if constexpr(std::is_same_v<noqual_B, ExpressionTreeNodePtr>)
    {
      result = a + b.get<ExpressionTreeNodePtr>();
    }
    else if constexpr(std::is_same_v<noqual_A, std::complex<Token>*> && std::is_same_v<noqual_B, std::complex<Token>*>)
    {
      result = (*(a.get<std::complex<Token>*>())) + (*(b.get<std::complex<Token>*>()));
    }
    else if constexpr(std::is_same_v<noqual_A, std::complex<Token>*> && std::is_same_v<noqual_B, dec_float*>)
    {
      result = (*(a.get<std::complex<Token>*>())) + b;
    }
    else if constexpr(std::is_same_v<noqual_A, dec_float*> && std::is_same_v<noqual_B, std::complex<Token>*>)
    {
      result = a + (*(b.get<std::complex<Token>*>()));
    }
    else if constexpr(std::is_same_v<noqual_A, matn<Token>*> && std::is_same_v<noqual_B, matn<Token>*>)
    {
      if(a.get<matn<Token>*>()->width != b.get<matn<Token>*>()->width
          ||  a.get<matn<Token>*>()->height != b.get<matn<Token>*>()->height)
      {
        result = InfoLog<2, MatDimension>(ERROR_INVALID_ADD_OPERANDS, {MatDimension(a.get<matn<Token>*>()->width, a.get<matn<Token>*>()->height),
                                                                       MatDimension(b.get<matn<Token>*>()->width, b.get<matn<Token>*>()->height)});
      }
      else
      {
        result = (*a.get<matn<Token>*>()) + (*b.get<matn<Token>*>());
      }
    }
    else if constexpr(std::is_same_v<noqual_A, vecn<Token>*> && std::is_same_v<noqual_B, vecn<Token>*>)
    {
      if(a.get<vecn<Token>*>()->height != b.get<vecn<Token>*>()->height)
      {
        MatDimension dimensionArray[2] = {MatDimension(1, a.get<vecn<Token>*>()->height),
                                          MatDimension(1, b.get<vecn<Token>*>()->height)};
        result = InfoLog<2, MatDimension>(ERROR_INVALID_ADD_OPERANDS, dimensionArray);
      }
      else
      {
        result = (*a.get<vecn<Token>*>()) + (*b.get<vecn<Token>*>());
      }
    }
    else if constexpr(std::is_same_v<noqual_A, dec_float*> && std::is_same_v<noqual_B, dec_float*>)
    {
      dec_float tempResult(*(a.get<dec_float*>()) + *(b.get<dec_float*>()));
      if(isOverflow(tempResult))
      {
        result = Token(ERROR_ADD_OVERFLOW);
      }
      else if(isUnderflow(tempResult))
      {
        result = Token(ERROR_ADD_UNDERFLOW);
      }
      else
      {
        result = std::move(tempResult);
      }
    }
    // else if constexpr(OperatorPlusOverloadExists_v<std::remove_pointer_t<noqual_A>, std::remove_pointer_t<noqual_B>>)
    // {
    //   throw std::runtime_error("sorry, testing if this is possible");
    //   result = *(a.get<noqual_A>()) + *(b.get<noqual_B>());
    // }
    else
    {
      result = Token(InfoLog<2, int>(ERROR_INVALID_ADD_OPERANDS, {getTokenType<noqual_A>(), getTokenType<noqual_B>()}));
    }

    return std::move(result);
  }

  struct tokenVariantOperatorPlusVisitor
  {
    tokenVariantOperatorPlusVisitor() {}
    template<typename A, typename B>
    Token operator()(const Token& a, const Token& b)
    {
      return tokenOperatorPlus<A,B>(a,b);
    }
  };

  Token operator+(const Token& a, const Token& b)
  {
    return std::visit([&](auto& arg1, auto& arg2){return tokenVariantOperatorPlusVisitor().operator()<std::remove_reference_t<decltype(arg1)>, std::remove_reference_t<decltype(arg2)>>(a,b);}, a.dataVariant, b.dataVariant);
  }
}

#endif