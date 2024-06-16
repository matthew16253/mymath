#ifndef TOKEN_HPP
#define TOKEN_HPP

#include<complex>
#include<type_traits>

#include"../fd_decs.hpp"

namespace mymath
{
  Token::Token() : dataptr(nullptr), type(DT_UNINIT)  {}
  Token::Token(void* _dataptr,TokenType _type) : dataptr(_dataptr), type(_type)  {}
  Token::~Token()
  {
    switch(type)
    {
      case DT_REAL:  {delete static_cast<long double*>(dataptr);break;}
      case DT_COMPLEX:  {delete static_cast<std::complex<Token>*>(dataptr);break;}
      case DT_VECTOR:  {delete static_cast<mymath::vecn<Token>*>(dataptr);break;}
      case DT_MATRIX:  {delete static_cast<mymath::matn<Token>*>(dataptr);break;}
      case DT_ALGEBRAIC_EXPR:  {delete static_cast<ExpressionTreeNode*>(dataptr);break;}
      default:  {}
    }
  }

  Token::Token(const Token& other)  :  type(other.type)
  {
    switch(other.type)
    {
      case DT_REAL:
        {dataptr = static_cast<void*>(new long double(*static_cast<long double*>(other.dataptr)));  break;}
      case DT_COMPLEX:
        {dataptr = static_cast<void*>(new std::complex<Token>(*static_cast<std::complex<Token>*>(other.dataptr)));  break;}
      case DT_VECTOR:
        {dataptr = static_cast<void*>(new mymath::vecn(*static_cast<mymath::vecn<Token>*>(other.dataptr)));  break;}
      case DT_MATRIX:
        {dataptr = static_cast<void*>(new mymath::matn<Token>(*static_cast<mymath::matn<Token>*>(other.dataptr)));  break;}
      case DT_ALGEBRAIC_EXPR:
        {dataptr = static_cast<void*>(new mymath::ExpressionTreeNode(*static_cast<mymath::ExpressionTreeNode*>(other.dataptr)));  break;}
      default:  {}
    }
  }
  Token::Token(Token&& other) : dataptr(other.dataptr), type(other.type)
  {
    other.empty();
  }
  
  Token& Token::operator=(Token other)
  {
    swap(*this,other);
    return *this;
  }
  
  void Token::empty()
  {
    dataptr = nullptr;
    type = DT_UNINIT;
  }
}

#include"token_operations/addition/operator+.hpp"
#include"token_operations/addition/operator+=.hpp"
#include"token_operations/subtraction/operator-.hpp"
#include"token_operations/subtraction/operator-=.hpp"

#include"complex/template_specializations.hpp"

#endif