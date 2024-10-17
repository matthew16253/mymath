#ifndef TOKEN_OPERATOR_MULTIPLY_HPP
#define TOKEN_OPERATOR_MULTIPLY_HPP

#include"../../../fd_decs.hpp"

namespace mymath
{
  template<typename A, typename B>
  concept OperatorMultiplyOverloadExists_c = requires()
  {
    std::declval<A>() * std::declval<B>();
  };

  template<typename A, typename B>
  constexpr bool OperatorMultiplyOverloadExists_v = OperatorMultiplyOverloadExists_c<A,B>;

  template<typename A, typename B>
  Token tokenOperatorMultiply(const Token& a, const Token& b)
  {
    using noqual_A = std::remove_cv_t<std::remove_reference_t<A>>;
    using noqual_B = std::remove_cv_t<std::remove_reference_t<B>>;
    Token result;
    if constexpr(std::is_same_v<noqual_A, TokenType> || std::is_same_v<noqual_B, TokenType>)
    {
      throw std::invalid_argument("no tokentypes for operator* sowwy :(");
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
      result = a.get<ExpressionTreeNodePtr>() * b.get<ExpressionTreeNodePtr>();
    }
    else if constexpr(std::is_same_v<noqual_A, ExpressionTreeNodePtr>)
    {
      result = a.get<ExpressionTreeNodePtr>() * b;
    }
    else if constexpr(std::is_same_v<noqual_B, ExpressionTreeNodePtr>)
    {
      result = a * b.get<ExpressionTreeNodePtr>();
    }
    else if constexpr(std::is_same_v<noqual_A, std::complex<Token>*> && std::is_same_v<noqual_B, std::complex<Token>*>)
    {
      result = (*(a.get<std::complex<Token>*>())) * (*(b.get<std::complex<Token>*>()));
    }
    else if constexpr(std::is_same_v<noqual_A, std::complex<Token>*>)
    {
      result = (*(a.get<std::complex<Token>*>())) * b;
    }
    else if constexpr(std::is_same_v<noqual_B, std::complex<Token>*>)
    {
      result = a * (*(b.get<std::complex<Token>*>()));
    }
    else if constexpr(std::is_same_v<noqual_A, matn<Token>*> && std::is_same_v<noqual_B, matn<Token>*>)
    {
      if(a.get<matn<Token>*>()->width != b.get<matn<Token>*>()->height)
      {
        MatDimension dimensionArray[2] = {MatDimension(a.get<matn<Token>*>()->width, a.get<matn<Token>*>()->height),
                                          MatDimension(b.get<matn<Token>*>()->width, b.get<matn<Token>*>()->height)};
        result = InfoLog<2, MatDimension>(dimensionArray);
      }
      else
      {
        result = (*a.get<matn<Token>*>()) * (*b.get<matn<Token>*>());
      }
    }
    else if constexpr(std::is_same_v<noqual_A, vecn<Token>*> && std::is_same_v<noqual_B, vecn<Token>*>)
    {
      if(a.get<vecn<Token>*>()->height != 3  &&  b.get<vecn<Token>*>()->height != 3)
      {
        MatDimension dimensionArray[2] = {MatDimension(1, a.get<vecn<Token>*>()->height),
                                          MatDimension(1, b.get<vecn<Token>*>()->height)};
        result = InfoLog<2, MatDimension>(dimensionArray);
      }
      else
      {
        result = cross((*a.get<vecn<Token>*>()), (*b.get<vecn<Token>*>()));
      }
    }
    else if constexpr(std::is_same_v<noqual_A, dec_float*> && std::is_same_v<noqual_B, dec_float*>)
    {
      dec_float tempResult(*(a.get<dec_float*>()) + *(b.get<dec_float*>()));
      if(tempResult.backend().isinf())
      {
        result = Token(ERROR_INF);
      }
      else
      {
        result = std::move(tempResult);
      }
    }
    // else if constexpr(OperatorMultiplyOverloadExists_v<std::remove_pointer_t<noqual_A>, std::remove_pointer_t<noqual_B>>)
    // {
    //   result = *(a.get<noqual_A>()) + *(b.get<noqual_B>());
    // }
    else
    {
      result = Token(InfoLog<2, int>(ERROR_INVALID_OPERATOR_MUL_OPERANDS, {getTokenType<noqual_A>(), getTokenType<noqual_B>()}));
    }

    return std::move(result);
  }

  struct tokenVariantOperatorMultiplyVisitor
  {
    tokenVariantOperatorMultiplyVisitor() {}
    template<typename A, typename B>
    Token operator()(const Token& a, const Token& b)
    {
      return tokenOperatorMultiply<A,B>(a,b);
    }
  };

  Token operator*(const Token& a, const Token& b)
  {
    return std::visit([&](auto& arg1, auto& arg2){return tokenVariantOperatorMultiplyVisitor().operator()<std::remove_reference_t<decltype(arg1)>, std::remove_reference_t<decltype(arg2)>>(a,b);}, a.dataVariant, b.dataVariant);
  }
}

#endif