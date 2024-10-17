#ifndef TOKEN_OPERATOR_PLUS_EQUALS_HPP
#define TOKEN_OPERATOR_PLUS_EQUALS_HPP

#include"../../../fd_decs.hpp"

namespace mymath
{
  template<typename A, typename B>
  concept OperatorPlusAssignOverloadExists_c = requires()
  {
    std::declval<A>() += (std::declval<B>());
  };

  template<typename A, typename B>
  struct OperatorPlusAssignOverloadExists
  {
    static constexpr bool value = OperatorPlusAssignOverloadExists_c<A,B>;
  };
  template<typename A, typename B>
  constexpr bool OperatorPlusAssignOverloadExists_v = OperatorPlusAssignOverloadExists<A,B>::value;

  template<typename A, typename B>
  void tokenOperatorPlusAssign(Token& a, const Token& b)
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
    else if constexpr(isInfoLog_v<noqual_B> || std::is_null_pointer_v<noqual_A>)
    {
      a = b;
    }
    else if constexpr(std::is_same_v<noqual_A, ExpressionTreeNodePtr> && std::is_same_v<noqual_B, ExpressionTreeNodePtr>)
    {
      const_cast<ExpressionTreeNodePtr&>(a.get<ExpressionTreeNodePtr>()) += b.get<ExpressionTreeNodePtr>();
    }
    else if constexpr(std::is_same_v<noqual_A, ExpressionTreeNodePtr>)
    {
      const_cast<ExpressionTreeNodePtr&>(a.get<ExpressionTreeNodePtr>()) += b;
    }
    else if constexpr(std::is_same_v<noqual_B, ExpressionTreeNodePtr>)
    {
      ExpressionTreeNodePtr new_a = new ExpressionTreeNode(*const_cast<ExpressionTreeNodePtr&>(b.get<ExpressionTreeNodePtr>()));
      new_a += a;
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
    else if constexpr(std::is_same_v<noqual_B, dec_float*> && std::is_same_v<noqual_B, std::complex<Token>*>)
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
        a = InfoLog<2, MatDimension>(ERROR_INVALID_ADD_OPERANDS, {MatDimension(a.get<matn<Token>*>()->width, a.get<matn<Token>*>()->height),
                                                                  MatDimension(b.get<matn<Token>*>()->width, b.get<matn<Token>*>()->height)});
      }
      else
      {
        (*a.get<matn<Token>*>()) += (*b.get<matn<Token>*>());
      }
    }
    else if constexpr(std::is_same_v<noqual_A, vecn<Token>*> && std::is_same_v<noqual_B, vecn<Token>*>)
    {
      if(a.get<vecn<Token>*>()->height != b.get<vecn<Token>*>()->height)
      {
        MatDimension dimensionArray[2] = {MatDimension(0, a.get<vecn<Token>*>()->height),
                                          MatDimension(0, b.get<vecn<Token>*>()->height)};
        a = InfoLog<2, MatDimension>(ERROR_INVALID_ADD_OPERANDS,{MatDimension(1, a.get<vecn<Token>*>()->height),
                                                                 MatDimension(1, b.get<vecn<Token>*>()->height)});
      }
      else
      {
        (*a.get<vecn<Token>*>()) += (*b.get<vecn<Token>*>());
      }
    }
    else if constexpr(std::is_same_v<noqual_A, dec_float*> && std::is_same_v<noqual_B, dec_float*>)
    {
      *(a.get<dec_float*>()) += *(b.get<dec_float*>());
      if(isOverflow(*a.get<dec_float*>()))
      {
        a = Token(ERROR_ADD_OVERFLOW);
      }
      else if(isUnderflow(*a.get<dec_float*>()))
      {
        a = Token(ERROR_ADD_UNDERFLOW);
      }
      else
      {
        //
      }
    }
    // else if constexpr(OperatorPlusAssignOverloadExists_v<std::remove_pointer_t<noqual_A>, std::remove_pointer_t<noqual_B>>)
    // {
    //   *(a.get<noqual_A>()) += *(b.get<noqual_B>());
    // }
    else
    {
      a = Token(InfoLog<2, int>(ERROR_INVALID_ADD_OPERANDS, {getTokenType<noqual_A>(), getTokenType<noqual_B>()}));
    }
  }

  struct tokenVariantOperatorPlusAssignVisitor
  {
    tokenVariantOperatorPlusAssignVisitor() {}
    template<typename A, typename B>
    void operator()(Token& a, const Token& b)
    {
      tokenOperatorPlusAssign<A,B>(a,b);
    }
  };

  void Token::operator+=(const Token& other)
  {
    return std::visit([&](auto& arg1, auto& arg2){return tokenVariantOperatorPlusAssignVisitor().operator()<std::remove_reference_t<decltype(arg1)>, std::remove_reference_t<decltype(arg2)>>(*this,other);}, dataVariant, other.dataVariant);
  }
}

#endif