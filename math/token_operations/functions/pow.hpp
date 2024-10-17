#ifndef TOKEN_FUNCTION_POW_HPP
#define TOKEN_FUNCTION_POW_HPP

#include"../../../fd_decs.hpp"

namespace mymath
{
  template<typename A, typename B>
  concept FunctionPowOverloadExists_c = requires()
  {
    pow(std::declval<A>(), std::declval<B>());
  };

  template<typename A, typename B>
  struct FunctionPowOverloadExists
  {
    static constexpr bool value = FunctionPowOverloadExists_c<A, B>;
  };

  template<typename A, typename B>
  constexpr bool FunctionPowOverloadExists_v =  FunctionPowOverloadExists<A, B>::value;

  template<typename A, typename B>
  Token tokenFunctionPow(const Token& a, const Token& b)
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
    else if constexpr(isInfoLog_v<noqual_B>)
    {
      result = b;
    }
    else if constexpr(std::is_null_pointer_v<noqual_A>)
    {
      // leave uninitialised as nullptr_t
    }
    else if constexpr(std::is_same_v<noqual_A, ExpressionTreeNodePtr> && std::is_same_v<noqual_B, ExpressionTreeNodePtr>)
    {
      result = pow(a.get<ExpressionTreeNodePtr>, b.get<ExpressionTreeNodePtr>);
    }
    else if constexpr(std::is_same_v<noqual_A, ExpressionTreeNodePtr>)
    {
      result = pow(a.get<ExpressionTreeNodePtr>(), b);
    }
    else if constexpr(std::is_same_v<noqual_B, ExpressionTreeNodePtr>)
    {
      result = pow(a, b.get<ExpressionTreeNodePtr>());
    }
    else if constexpr(std::is_same_v<noqual_A, std::complex<Token>*> && std::is_same_v<noqual_B, std::complex<Token>*>)
    {
      if(a.get<std::complex<Token>*>()->real().is_type<dec_float*>() && a.get<std::complex<Token>*>()->imag().is_type<dec_float*>())
      {
        std::complex<dec_float&> base = std::complex<dec_float&>(*a.get<std::complex<Token>*>()->real().get<dec_float*>(), 
                                                                     *a.get<std::complex<Token>*>()->imag().get<dec_float*>());
        std::complex<dec_float&> exponent = std::complex<dec_float&>(*b.get<std::complex<Token>*>()->real().get<dec_float*>(), 
                                                                         *b.get<std::complex<Token>*>()->imag().get<dec_float*>());
        std::complex<dec_float> complex_result = std::pow(base, exponent);
        result = Token(std::complex<Token>(Token(complex_result.real()),
                                           Token(complex_result.imag())));
      }
      else
      {
        result = Token(InfoLog<2, int>(ERROR_INVALID_POW_OPERANDS, {DT_NOT_REAL_COMPLEX, DT_NOT_REAL_COMPLEX}));
      }
    }
    else if constexpr(std::is_same_v<noqual_A, dec_float*> && std::is_same_v<noqual_B, dec_float*>)
    {
      dec_float tempResult(pow(*(a.get<dec_float*>()), *(b.get<dec_float*>())));
      if(tempResult.backend().isinf())
      {
        result = Token(ERROR_INF);
      }
      else
      {
        result = std::move(tempResult);
      }
    }
    else if constexpr(std::is_same_v<noqual_A, std::complex<Token>*> && std::is_same_v<noqual_B, dec_float*>)
    {
      if(b.get<std::complex<Token>*>()->real().is_type<dec_float*>() && b.get<std::complex<Token>*>()->imag().is_type<dec_float*>())
      {
        std::complex<dec_float> base = std::complex<dec_float>(*a.get<std::complex<Token>*>()->real().get<dec_float*>(), 
                                                                 *a.get<std::complex<Token>*>()->imag().get<dec_float*>());
        result = Token(std::pow(base, *b.get<dec_float*>()));
      }
      else
      {
        result = Token(InfoLog<2, int>(ERROR_INVALID_POW_OPERANDS, {DT_NOT_REAL_COMPLEX, DT_REAL}));
      }
    }
    else if constexpr(std::is_same_v<noqual_A, dec_float*> && std::is_same_v<noqual_B, std::complex<Token>*>)
    {
      if(b.get<std::complex<Token>*>()->real().is_type<dec_float*>() && b.get<std::complex<Token>*>()->imag().is_type<dec_float*>())
      {
        std::complex<dec_float> exponent = std::complex<dec_float>(*b.get<std::complex<Token>*>()->real().get<dec_float*>(), 
                                                                  *b.get<std::complex<Token>*>()->imag().get<dec_float*>());
        result = Token(std::pow(*a.get<dec_float*>(), exponent));
      }
      else
      {
        result = Token(InfoLog<2, int>(ERROR_INVALID_POW_OPERANDS, {DT_REAL, DT_NOT_REAL_COMPLEX}));
      }
    }
    else
    {
      result = Token(InfoLog<2, int>(ERROR_INVALID_POW_OPERANDS, {getTokenType<noqual_A>(), getTokenType<noqual_B>()}));
    }
    return std::move(result);
  }

  struct tokenVariantFunctionPowVisitor
  {
    tokenVariantFunctionPowVisitor() {}
    template<typename A, typename B>
    Token operator()(const Token& a, const Token& b)
    {
      return tokenFunctionPow<A,B>(a,b);
    }
  };

  Token pow(const Token& a, const Token& b)
  {
    return std::visit([&](auto& arg1, auto& arg2){return tokenVariantFunctionPowVisitor().operator()<std::remove_reference_t<decltype(arg1)>, std::remove_reference_t<decltype(arg2)>>(a,b);}, a.dataVariant, b.dataVariant);
  }
}

#endif