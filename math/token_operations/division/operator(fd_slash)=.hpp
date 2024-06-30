#ifndef OPERATOR_FD_SLASH_EQUALS_HPP
#define OPERATOR_FD_SLASH_EQUALS_HPP

#include"../../../fd_decs.hpp"

namespace mymath
{
  void Token::operator/=(const Token& a)
  {
    if(isError(type) || isError(a.type)){return;}
    switch(type)
    {
      case DT_REAL:
      {
        switch(a.type)
        {
          case DT_REAL:
          {
            *real_ptr /= *a.real_ptr;
            break;
          }
          case DT_COMPLEX:
          {
            Token this_copy(new std::complex<Token>(*this, Token(new long double(0), DT_REAL)),DT_COMPLEX);
            *this_copy.complex_ptr /= *a.complex_ptr;
            break;;
          }
          case DT_ALGEBRAIC_EXPR:
          {
            Token a_copy(a);
            ExpressionTreeNodePtr a_copy_expr_ptr = a_copy.expr_ptr;
            applyBinaryOperation(a_copy_expr_ptr, OP_DIVIDE, *this);
            a_copy.expr_ptr = expr_ptr;
            swap(*this,a_copy);
            break;
          }
        }
        break;
      }
      case DT_COMPLEX:
      {
        switch(a.type)
        {
          case DT_REAL:
          {
            *complex_ptr *= a;
            break;
          }
          case DT_COMPLEX:
          {
            *complex_ptr *= *a.complex_ptr;
            break;
          }
          case DT_ALGEBRAIC_EXPR:
          {
            Token a_copy(a);
            ExpressionTreeNodePtr a_copy_expr_ptr = a_copy.expr_ptr;
            applyBinaryOperation(a_copy_expr_ptr, OP_DIVIDE, *this);
            a_copy.expr_ptr = expr_ptr;
            swap(*this, a_copy);
            break;
          }
        }
        break;
      }
      case DT_VECTOR:
      {
        switch(a.type)
        {
          case DT_REAL:
          {
            *vec_ptr /= a;
            break;
          }
          case DT_COMPLEX:
          {
            *vec_ptr /= a;
            break;
          }
          case DT_ALGEBRAIC_EXPR:
          {
            Token a_copy(a);
            ExpressionTreeNodePtr a_copy_expr_ptr = a_copy.expr_ptr;
            applyBinaryOperation(a_copy_expr_ptr, OP_DIVIDE, *this);          
            a_copy.expr_ptr = expr_ptr;  
            swap(*this,a_copy);
            break;
          }
        }
        break;
      }
      case DT_MATRIX:
      switch(a.type)
      {
        case DT_REAL:
        {
          *mat_ptr /= a;
          break;
        }
        case DT_COMPLEX:
        {
          *mat_ptr /= a;
          break;
        }
        case DT_ALGEBRAIC_EXPR:
        {
          Token a_copy(a);
          ExpressionTreeNodePtr a_copy_expr_ptr = a_copy.expr_ptr;
          applyBinaryOperation(a_copy_expr_ptr, OP_DIVIDE, *this);          
          a_copy.expr_ptr = expr_ptr;            
          swap(*this,a_copy);
          break;
        }
        break;
      }
      case DT_ALGEBRAIC_EXPR:
      {
        switch(a.type)
        {
          ExpressionTreeNodePtr _expr_ptr = expr_ptr;
          case DT_REAL:
          {
            applyBinaryOperation(_expr_ptr, OP_DIVIDE, a);
            expr_ptr = _expr_ptr;
            break;
          }
          case DT_COMPLEX:
          {
            applyBinaryOperation(_expr_ptr, OP_DIVIDE, a);
            expr_ptr = _expr_ptr;
            break;
          }
          case DT_VECTOR:
          {
            applyBinaryOperation(_expr_ptr, OP_DIVIDE, a);
            expr_ptr = _expr_ptr;
            break;
          }
          case DT_MATRIX:
          {
            applyBinaryOperation(_expr_ptr, OP_DIVIDE, a);
            expr_ptr = _expr_ptr;
            break;
          }
          case DT_ALGEBRAIC_EXPR:
          {
            ExpressionTreeNodePtr a_expr_ptr = a.expr_ptr;
            applyBinaryOperation(_expr_ptr, OP_DIVIDE, a_expr_ptr);
            expr_ptr = _expr_ptr;
            break;
          }
        }
        break;
      }
      case DT_UNINIT:
      {
        *this = Token(nullptr, DT_UNINIT);
        break;
      }
    }
  }
}

#endif