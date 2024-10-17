#ifndef TOKEN_OPERATOR_SUBTRACT_HPP
#define TOKEN_OPERATOR_SUBTRACT_HPP

#include"../../../fd_decs.hpp"

namespace mymath
{
  template<typename A, typename B>
  concept OperatorSubOverloadExists_c = requires()
  {
    operator-(std::declval<A>(), std::declval<B>());
  };

  template<typename A, typename B>
  struct OperatorSubOverloadExists
  {
    static constexpr bool value = OperatorSubOverloadExists_c<A,B>;
  };
  template<typename A, typename B>
  constexpr bool OperatorSubOverloadExists_v = OperatorSubOverloadExists<A,B>::value;

  template<typename A, typename B>
  Token tokenOperatorSub(const Token& a, const Token& b)
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
    else if constexpr(std::is_null_pointer_v<noqual_A>)
    {
      result = -b;
    }
    else if constexpr(isInfoLog_v<noqual_B>)
    {
      result = b;
    }
    else if constexpr(std::is_same_v<noqual_A, ExpressionTreeNodePtr> && std::is_same_v<noqual_B, ExpressionTreeNodePtr>)
    {
      result = a.get<ExpressionTreeNodePtr>() - b.get<ExpressionTreeNodePtr>();
    }
    else if constexpr(std::is_same_v<noqual_A, ExpressionTreeNodePtr>)
    {
      result = a.get<ExpressionTreeNodePtr>() - b;
    }
    else if constexpr(std::is_same_v<noqual_B, ExpressionTreeNodePtr>)
    {
      result = a - b.get<ExpressionTreeNodePtr>();
    }
    else if constexpr(std::is_same_v<noqual_A, std::complex<Token>*> && std::is_same_v<noqual_B, std::complex<Token>*>)
    {
      result = (*(a.get<std::complex<Token>*>())) - (*(b.get<std::complex<Token>*>()));
    }
    else if constexpr(std::is_same_v<noqual_A, std::complex<Token>*> && std::is_same_v<noqual_B, dec_float*>)
    {
      result = (*(a.get<std::complex<Token>*>())) - b;
    }
    else if constexpr(std::is_same_v<noqual_A, dec_float> && std::is_same_v<noqual_B, std::complex<Token>*>)
    {
      result = a - (*(b.get<std::complex<Token>*>()));
    }
    else if constexpr(std::is_same_v<noqual_A, matn<Token>*> && std::is_same_v<noqual_B, matn<Token>*>)
    {
      if(a.get<matn<Token>*>()->width != b.get<matn<Token>*>()->width
          ||  a.get<matn<Token>*>()->height != b.get<matn<Token>*>()->height)
      {
        MatDimension dimensionArray[2] = {MatDimension(a.get<matn<Token>*>()->width, a.get<matn<Token>*>()->height),
                                          MatDimension(b.get<matn<Token>*>()->width, b.get<matn<Token>*>()->height)};
        result = InfoLog<2, MatDimension>(dimensionArray);
      }
      else
      {
        result = (*a.get<matn<Token>*>()) - (*b.get<matn<Token>*>());
      }
    }
    else if constexpr(std::is_same_v<noqual_A, vecn<Token>*> && std::is_same_v<noqual_B, vecn<Token>*>)
    {
      if(a.get<vecn<Token>*>()->height != b.get<vecn<Token>*>()->height)
      {
        MatDimension dimensionArray[2] = {MatDimension(1, a.get<vecn<Token>*>()->height),
                                          MatDimension(1, b.get<vecn<Token>*>()->height)};
        result = InfoLog<2, MatDimension>(dimensionArray);
      }
      else
      {
        result = (*a.get<vecn<Token>*>()) - (*b.get<vecn<Token>*>());
      }
    }
    else if constexpr(std::is_same_v<noqual_A, dec_float*> && std::is_same_v<noqual_B, dec_float*>)
    {
      dec_float tempResult(*(a.get<dec_float*>()) - *(b.get<dec_float*>()));
      if(tempResult.backend().isinf())
      {
        result = Token(ERROR_INF);
      }
      else
      {
        result = std::move(tempResult);
      }
    }
    // else if constexpr(OperatorSubOverloadExists_v<std::remove_pointer_t<noqual_A>, std::remove_pointer_t<noqual_B>>)
    // {
    //   result = *(a.get<noqual_A>()) - *(b.get<noqual_B>());
    // }
    else
    {
      result = Token(InfoLog<2, int>(ERROR_INVALID_OPERATOR_SUB_OPERANDS, {getTokenType<noqual_A>(), getTokenType<noqual_B>()}));
    }

    return std::move(result);
  }

  struct tokenVariantOperatorSubVisitor
  {
    tokenVariantOperatorSubVisitor() {}
    template<typename A, typename B>
    Token operator()(const Token& a, const Token& b)
    {
      return tokenOperatorSub<A,B>(a,b);
    }
  };

  Token operator-(const Token& a, const Token& b)
  {
    return std::visit([&](auto& arg1, auto& arg2){return tokenVariantOperatorSubVisitor().operator()<std::remove_reference_t<decltype(arg1)>, std::remove_reference_t<decltype(arg2)>>(a,b);}, a.dataVariant, b.dataVariant);
  }



































  template<typename A>
  concept UnaryOperatorSubOverloadExists_c = requires()
  {
    operator-(std::declval<A>());
  };

  template<typename A>
  struct UnaryOperatorSubOverloadExists
  {
    static constexpr bool value = UnaryOperatorSubOverloadExists_c<A>;
  };
  template<typename A>
  constexpr bool UnaryOperatorSubOverloadExists_v = UnaryOperatorSubOverloadExists<A>::value;

  template<typename A>
  Token tokenUnaryOperatorSub(const Token& a)
  {
    using noqual_A = std::remove_cv_t<std::remove_reference_t<A>>;
    Token result;
    if constexpr(std::is_same_v<A, TokenType>)
    {
      throw std::invalid_argument("dont use toketype for negation");
    }
    else if constexpr(isInfoLog_v<noqual_A> || std::is_same_v<A, std::nullptr_t>)
    {
      result = a;
    }
    else if constexpr(std::is_same_v<noqual_A, dec_float*>)
    {
      dec_float tempResult( - *(a.get<dec_float*>()));
      if(tempResult.backend().isinf())
      {
        //set result to error pls
      }
      else
      {
        result = std::move(tempResult);
      }
    }
    else if constexpr(std::is_same_v<noqual_A, ExpressionTreeNodePtr>)
    {
      result = -a.get<noqual_A>();
    }
    // else if constexpr(UnaryOperatorSubOverloadExists_v<std::remove_pointer_t<noqual_A>>)
    // {
    //   result = -a.get<noqual_A>();
    // }
    else
    {
      result = Token(InfoLog<1, int>(ERROR_INVALID_NEGATE_OPERANDS, {getTokenType<noqual_A>()}));
    }

    return std::move(result);
  }

  struct tokenVariantUnaryOperatorSubVisitor
  {
    tokenVariantUnaryOperatorSubVisitor() {}
    template<typename A>
    Token operator()(const Token& a)
    {
      return tokenUnaryOperatorSub<A>(a);
    }
  };

  Token operator-(const Token& a)
  {
    return std::visit([&](auto arg1){return tokenVariantUnaryOperatorSubVisitor().operator()<std::remove_reference_t<decltype(arg1)>>(a);}, a.dataVariant);
  }
}

#endif