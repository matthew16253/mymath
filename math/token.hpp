#ifndef TOKEN_HPP
#define TOKEN_HPP

#include<complex>
#include<type_traits>

#include"../fd_decs.hpp"

namespace mymath
{
  struct tokenVariantDeleter
  {
    template<typename T>
    void operator()(T data)
    {
      if(std::is_pointer(T))
      {
        delete data;
      }
    }
  };

  struct tokenVariantCopyConstructor
  {
    tokenVariant& to_be_assigned_to;
    tokenVariantCopyConstructor(tokenVariant& _to_be_assigned_to) : to_be_assigned_to(_to_be_assigned_to)  {}
    template<typename T>
    void operator()(T data)
    {
      if(std::is_pointer(T))
      {
        to_be_assigned_to = new typename std::remove_pointer(T)(*data);
      }
      else
      {
        to_be_assigned_to = data;
      }
    }
  };

  struct tokenVariantPrinter
  {
    template<typename T>
    void operator()(T data)
    {
      std::cout << data;
    }
  };

  struct tokenVariantMoveConstructor
  {
    tokenVariant& to_be_assigned_to;
    tokenVariantMoveConstructor(tokenVariant& _to_be_assigned_to) : to_be_assigned_to(_to_be_assigned_to)  {}
    template<typename T>
    void operator()(T data)
    {
      to_be_assigned_to = data;
    }
  };

  struct tokenVariantComparor
  {
    template<typename A, typename B>
    bool operator()(A a, B b)
    {
      if(std::is_same(A, B))
      {
        return (a == b);
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
  Token::Token(T other){dataVariant = other;}

  template<typename T>
  Token Token::operator=(T other){dataVariant = other;}

  Token::Token(Token&& other)
  {
    std::visit(tokenVariantMoveConstructor(dataVariant), other.dataVariant);
  }
  
  Token& Token::operator=(Token other)
  {
    swap(*this,other);
    return *this;
  }



  std::ostream& operator<<(std::ostream& os, const Token& token)
  {
    std::visit(tokenVariantPrinter{}, token.dataVariant);
    return os;
  }

  bool operator==(const Token& a, const Token& b)
  {
    return std::visit(tokenVariantComparor{}, a.dataVariant, b.dataVariant);
  }

  template<typename T>
  T Token::get()
  {
    return std::get<T>(*this);
  }

}

#include"token_operations/addition/operator+.hpp"
#include"token_operations/addition/operator+=.hpp"
#include"token_operations/subtraction/operator-.hpp"
#include"token_operations/subtraction/operator-=.hpp"

#include"complex/template_specializations.hpp"

#endif