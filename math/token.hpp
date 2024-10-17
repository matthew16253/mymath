#ifndef TOKEN_HPP
#define TOKEN_HPP

#include<complex>
#include<type_traits>
#include<concepts>

#include"../fd_decs.hpp"

namespace mymath
{
  struct tokenVariantDeleter
  {
    template<typename T>
    requires is_pointer<T> or std::same_as<T, ExpressionTreeNodePtr>
    void operator()(T& data)
    {
      delete data;
    }
    template<typename T>
    void operator()(const T& data) {}
  };

  struct tokenVariantCopyConstructor
  {
    tokenVariant& to_be_assigned_to;
    tokenVariantCopyConstructor(tokenVariant& _to_be_assigned_to) : to_be_assigned_to(_to_be_assigned_to)  {}
    template<typename T>
    requires is_pointer<T>
    void operator()(T data)
    {
      to_be_assigned_to = new typename std::remove_pointer<T>(*data);
    }
    template<typename T>
    void operator()(T data)
    {
      to_be_assigned_to = data;
    }
  };

  struct tokenVariantPrinter
  {
    std::ostream& os;
    tokenVariantPrinter(std::ostream& _os) : os(_os)  {}
    template<typename T>
    void operator()(T data)
    {
      using noqual_T = std::remove_reference_t<std::remove_cv_t<T>>;
      if constexpr(isInfoLog_v<noqual_T> || std::is_same_v<noqual_T, std::nullptr_t>)
      {
        throw std::invalid_argument("this type is not printable");
      }
      else if constexpr(std::is_pointer_v<noqual_T>)
      {
       os << *data;
      }
      else
      {
        os << data;
      }
    }
  };

  struct tokenVariantMoveConstructor
  {
    tokenVariant& to_be_assigned_to;
    tokenVariantMoveConstructor(tokenVariant& _to_be_assigned_to) : to_be_assigned_to(_to_be_assigned_to)  {}
    template<typename T>
    void operator()(const T& data)
    {
      to_be_assigned_to = data;
    }
  };

  struct tokenVariantComparor
  {
    template<typename A, typename B>
    bool operator()(A a, B b)
    {
      if constexpr(std::is_same_v<A, B>)
      {
        if constexpr(isInfoLog_v<A>)
        {
          return false;
        }
        else if constexpr(std::is_same_v<A, ExpressionTreeNodePtr> || std::is_pointer_v<A>)
        {
          return (*a == *b);
        }
        else
        {
          return (a == b);
        }
      }
      else
      {
        return false;
      }
    }
  };



  Token::Token() : dataVariant(std::nullptr_t())  {}

  Token::~Token()
  {
    std::visit(tokenVariantDeleter{}, dataVariant);
  }

  Token::Token(const Token& other)
  {
    std::visit(tokenVariantCopyConstructor(dataVariant), other.dataVariant);
  }

  template<typename T>
  requires (!std::is_same_v<std::remove_reference_t<std::remove_cv_t<T>>, Token>)
  Token::Token(const T& other)
  {
    this->~Token();
    if constexpr(std::is_same_v<T, TokenType> || std::is_same_v<T, ExpressionTreeNodePtr>)
    {
      this->dataVariant = other;
    }
    else if constexpr(std::is_same_v<T, ExpressionTreeNode>)
    {
      this->dataVariant = ExpressionTreeNodePtr(new ExpressionTreeNode(other));
    }
    else if constexpr(std::is_pointer_v<T>)
    {
      this->dataVariant = other;
    }
    else
    {
      this->dataVariant = new T(other);
    }
  }

  Token::Token(Token&& other)
  {
    std::visit(tokenVariantMoveConstructor(dataVariant), other.dataVariant);
  }
  
  Token& Token::operator=(Token other)
  {
    //Token other_copy = other;
    swap(*this, other);
    return *this;
  }

  template<typename T>
  requires (!std::is_same_v<std::remove_reference_t<std::remove_cv_t<T>>, Token>)
  Token& Token::operator=(const T& other)
  {
    *this = Token(other);
    return *this;
  }

  template<typename T>
  requires (!std::is_same_v<std::remove_reference_t<std::remove_cv_t<T>>, Token>)
  Token& Token::operator=(T&& other)
  {
    *this = Token(other);
    return *this;
  }

  std::ostream& operator<<(std::ostream& os, const Token& token)
  {
    std::visit(tokenVariantPrinter(os), token.dataVariant);
    return os;
  }

  bool operator==(const Token& a, const Token& b)
  {
    return std::visit(tokenVariantComparor{}, a.dataVariant, b.dataVariant);
  }

  template<typename T>
  const T& Token::get() const
  {
    return std::get<T>(dataVariant);
  }

  template<typename T>
  bool Token::is_type()
  {
    return std::holds_alternative<T>(dataVariant);
  }




  template<typename T>
  constexpr TokenType getTokenType()
  {
    if constexpr(std::is_same_v<std::remove_pointer_t<T>, dec_float>)
    {
      return DT_REAL;
    }
    else if constexpr(std::is_same_v<std::remove_pointer_t<T>, std::complex<Token>>)
    {
      return DT_COMPLEX;
    }
    else if constexpr(std::is_same_v<std::remove_pointer_t<T>, matn<Token>>)
    {
      return DT_MAT;
    }
    else if constexpr(std::is_same_v<std::remove_pointer_t<T>, vecn<Token>>)
    {
      return DT_VEC;
    }
    else if constexpr(std::is_same_v<T, ExpressionTreeNodePtr> || std::is_same_v<T, ExpressionTreeNode>)
    {
      return DT_EXPR;
    }
    else
    {
      return DT_NOT_DATA;
    }
  }

}

#include"token_operations/addition/operator+.hpp"
#include"token_operations/addition/operator+=.hpp"
#include"token_operations/subtraction/operator-.hpp"
#include"token_operations/subtraction/operator-=.hpp"
#include"token_operations/multiplication/operator(asterisk).hpp"
#include"token_operations/multiplication/operator(asterisk)=.hpp"
#include"token_operations/division/operator(fd_slash).hpp"
#include"token_operations/division/operator(fd_slash)=.hpp"
#include"token_operations/functions/pow.hpp"

#endif