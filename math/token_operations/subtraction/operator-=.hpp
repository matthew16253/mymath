#ifndef TOKEN_OPERATOR_SUBTRACT_EQUALS_HPP
#define TOKEN_OPERATOR_SUBTRACT_EQUALS_HPP

#include"../../../fd_decs.hpp"

namespace mymath
{
  template<typename A, typename B>
  concept OperatorSubAssignOverloadExists_c = requires()
  {
    std::declval<A>() -= (std::declval<B>());
  };

  template<typename A, typename B>
  struct OperatorSubAssignOverloadExists
  {
    static constexpr bool value = OperatorSubAssignOverloadExists_c<A,B>;
  };
  template<typename A, typename B>
  constexpr bool OperatorSubAssignOverloadExists_v = OperatorSubAssignOverloadExists<A,B>::value;

  template<typename A, typename B>
  void tokenOperatorSubAssign(Token& a, const Token& b)
  {
    using noqual_A = std::remove_cv_t<std::remove_reference_t<A>>;
    using noqual_B = std::remove_cv_t<std::remove_reference_t<B>>;

    if constexpr(std::is_same_v<noqual_A, TokenType> || std::is_same_v<noqual_B, TokenType>)
    {
      throw std::invalid_argument("get these tokentypes outta here kiddo");
    }
    else if constexpr(isInfoLog_v<noqual_A> || std::is_null_pointer_v<noqual_B>)
    {
      return;
    }
    else if constexpr(isInfoLog_v<noqual_B>)
    {
      a = b;
    }
    else if constexpr(std::is_null_pointer_v<noqual_A>)
    {
      a = -b;
    }
    else if constexpr(std::is_same_v<noqual_A, ExpressionTreeNodePtr> && std::is_same_v<noqual_B, ExpressionTreeNodePtr>)
    {
      const_cast<ExpressionTreeNodePtr&>(a.get<ExpressionTreeNodePtr>()) -= b.get<ExpressionTreeNodePtr>();
    }
    else if constexpr(std::is_same_v<noqual_A, ExpressionTreeNodePtr>)
    {
      const_cast<ExpressionTreeNodePtr&>(a.get<ExpressionTreeNodePtr>()) -= b;
    }
    else if constexpr(std::is_same_v<noqual_B, ExpressionTreeNodePtr>)
    {
      ExpressionTreeNodePtr new_a = new ExpressionTreeNode(*(-b.get<ExpressionTreeNodePtr>()));
      new_a += a.get<ExpressionTreeNodePtr>();
      a = new_a;
    }
    else if constexpr(std::is_same_v<noqual_A, std::complex<Token>*> && std::is_same_v<noqual_B, std::complex<Token>*>)
    {
      (*(a.get<std::complex<Token>*>())) += (*(b.get<std::complex<Token>*>()));
    }
    else if constexpr(std::is_same_v<noqual_A, std::complex<Token>*> && std::is_same_v<noqual_B, dec_float*>)
    {
      (*(a.get<std::complex<Token>*>())) += b;
    }
    else if constexpr(std::is_same_v<noqual_A, dec_float*> && std::is_same_v<noqual_B, std::complex<Token>*>)
    {
      std::complex<Token> new_a = (*b.get<std::complex<Token>*>());
      new_a += a;
      a = std::move(new_a);
    }
    else if constexpr(std::is_same_v<noqual_A, matn<Token>*> && std::is_same_v<noqual_B, matn<Token>*>)
    {
      if(a.get<matn<Token>*>()->width != b.get<matn<Token>*>()->width
          ||  a.get<matn<Token>*>()->height != b.get<matn<Token>*>()->height)
      {
        MatDimension dimensionArray[2] = {MatDimension(a.get<matn<Token>*>()->width, a.get<matn<Token>*>()->height),
                                          MatDimension(b.get<matn<Token>*>()->width, b.get<matn<Token>*>()->height)};
        a = InfoLog<2, MatDimension>(dimensionArray);
      }
      else
      {
        (*a.get<matn<Token>*>()) -= (*b.get<matn<Token>*>());
      }
    }
    else if constexpr(std::is_same_v<noqual_A, vecn<Token>*> && std::is_same_v<noqual_B, vecn<Token>*>)
    {
      if(a.get<vecn<Token>*>()->height != b.get<vecn<Token>*>()->height)
      {
        MatDimension dimensionArray[2] = {MatDimension(1, a.get<vecn<Token>*>()->height),
                                          MatDimension(1, b.get<vecn<Token>*>()->height)};
        a = InfoLog<2, MatDimension>(dimensionArray);
      }
      else
      {
        (*a.get<vecn<Token>*>()) -= (*b.get<vecn<Token>*>());
      }
    }
    else if constexpr(std::is_same_v<noqual_A, dec_float*> && std::is_same_v<noqual_B, dec_float*>)
    {
      *(a.get<dec_float*>()) += *(b.get<dec_float*>());
      if(a.get<dec_float*>()->backend().isinf())
      {
        a = Token(ERROR_INF);
      }
      else
      {
        //
      }
    }
    // else if constexpr(OperatorSubAssignOverloadExists_v<std::remove_pointer_t<noqual_A>, std::remove_pointer_t<noqual_B>>)
    // {
    //   *(a.get<noqual_A>()) -= *(b.get<noqual_B>());
    // }
    else
    {
      a = Token(InfoLog<2, int>(ERROR_INVALID_OPERATOR_SUBTRACT_ASSIGN_OPERANDS, {getTokenType<noqual_A>(), getTokenType<noqual_B>()}));
    }
  }

  struct tokenVariantOperatorSubAssignVisitor
  {
    tokenVariantOperatorSubAssignVisitor() {}
    template<typename A, typename B>
    void operator()(Token& a, const Token& b)
    {
      tokenOperatorSubAssign<A,B>(a,b);
    }
  };

  void Token::operator-=(const Token& other)
  {
    return std::visit([&](auto& arg1, auto& arg2){return tokenVariantOperatorSubAssignVisitor().operator()<std::remove_reference_t<decltype(arg1)>, std::remove_reference_t<decltype(arg2)>>(*this,other);}, dataVariant, other.dataVariant);
  }
}

#endif