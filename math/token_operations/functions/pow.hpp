#ifndef TOKEN_POW_HPP
#define TOKEN_POW_HPP

#include<cmath>

#include"../../../fd_decs.hpp"

namespace mymath
{
  Token pow(const Token& a, const Token& b)
  {
    if(isError(a.type)){return a;}
    if(isError(b.type)){return b;}

    Token result;

    switch (a.type)
    {
      case DT_REAL:
      {
        switch(b.type)
        {
          case DT_REAL:
          {
            result = Token(new long double(std::pow(*a.real_ptr,*b.real_ptr)), DT_REAL);
            break;
          }
          case DT_COMPLEX:
          {
            result = Token(new std::complex<Token>(std::pow(a, *b.complex_ptr)), DT_COMPLEX);
            break;
          }
          default:
          {
            TokenType arr[] = {a.type, b.type};
            return Token(new InfoLog<2,TokenType>(arr), ERROR_INVALID_2_OPERANDS);
            break;
          }
        }
        break;
      }
      case DT_COMPLEX:
      {
        switch(b.type)
        {
          case DT_REAL:
          {
            result = Token(new std::complex<Token>(std::pow(*a.complex_ptr, b)), DT_COMPLEX);
            break;
          }
          case DT_COMPLEX:
          {
            result = Token(new std::complex<Token>(std::pow(*a.complex_ptr, *b.complex_ptr)), DT_COMPLEX);
            break;
          }
          default:
          {
            TokenType arr[] = {a.type, b.type};
            result = Token(new InfoLog<2,TokenType>(arr), ERROR_INVALID_2_OPERANDS);
            break;
          }
        }
        break;
      }
      case DT_MATRIX:
      {
        switch(b.type)
        {
          case DT_REAL:
          {
            if(!isInt(*b.real_ptr))
            {
              TokenType arr[2] = {a.type, b.type};
              result = Token(new InfoLog<2, TokenType>(arr), ERROR_INVALID_2_OPERANDS);
            }
            else
            {
              result = Token(new matn<Token>(pow(*a.mat_ptr, static_cast<int>(*b.real_ptr))), DT_MATRIX);
            }
            break;
          }
          default:
          {
            TokenType arr[] = {a.type, b.type};
            result = Token(new InfoLog<2, TokenType>(arr), ERROR_INVALID_2_OPERANDS);
          }
        }
      }
      default:
      {
        TokenType arr[] = {a.type, b.type};
        result = Token(new InfoLog<2, TokenType>(arr), ERROR_INVALID_2_OPERANDS);
      }
    }
    return result;
  }
}

#endif