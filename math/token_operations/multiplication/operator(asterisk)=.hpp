#ifndef TOKEN_OPERATOR_MULTIPLY_EQUALS_HPP
#define TOKEN_OPERATOR_MULTIPLY_EQUALS_HPP

#include"../../../fd_decs.hpp"

namespace mymath
{
  void Token::operator*=(const Token& a)
  {
    switch(type)
    {
      case DT_REAL:
      {
        switch(a.type)
        {
          case DT_REAL:
          {
            *real_ptr *= *a.real_ptr;
            break;
          }
          case DT_COMPLEX:
          {
            Token this_copy(new std::complex<Token>(*this, Token(new long double(0), DT_REAL)),DT_COMPLEX);
            *this_copy.complex_ptr *= *a.complex_ptr;
            break;;
          }
          case DT_VECTOR:
          {
            Token a_copy(a);
            *a_copy.vec_ptr *= *this;
            swap(*this, a_copy);
            break;
          }
          case DT_MATRIX:
          {
            Token a_copy(a);
            *a_copy.mat_ptr *= *this;
            swap(*this,a_copy);
            break;
          }
          case DT_ALGEBRAIC_EXPR:
          {
            Token a_copy(a);
            applyBinaryOperation(a_copy.expr_ptr, OP_MULTIPLY, *this);
            swap(*this,a_copy);
            break;
          }
        }
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
          case DT_VECTOR:
          {
            Token a_copy(a);
            *a_copy.vec_ptr *= *this;
            swap(*this, a_copy);
            break;
          }
          case DT_MATRIX:
          {
            Token a_copy(a);
            *a_copy.mat_ptr *= *this;
            swap(*this, a_copy);
            break;
          }
          case DT_ALGEBRAIC_EXPR:
          {
            Token a_copy(a);
            applyBinaryOperation(a_copy.expr_ptr,OP_MULTIPLY,*this);
            swap(*this, a_copy);
            break;
          }
        }
      }
      case DT_VECTOR:
      {
        switch(a.type)
        {
          case DT_REAL:
          {
            *vec_ptr *= a;
            break;
          }
          case DT_COMPLEX:
          {
            *vec_ptr *= a;
            break;
          }
          case DT_MATRIX:
          {
            if(1 != a.mat_ptr->height)
            {
              int arr[] = {1,a.mat_ptr->height};
              *this = Token(new InfoLog<2,int>(arr), ERROR_INVALID_2_OPERANDS);
              break;
            }
            else
            {
              *vec_ptr *= *a.mat_ptr;
              break;
            }
          }
          case DT_ALGEBRAIC_EXPR:
          {
            Token a_copy(a);
            applyBinaryOperation(a_copy.expr_ptr, OP_MULTIPLY, *this);
            swap(*this,a_copy);
          }
        }
      }
      case DT_MATRIX:
      switch(a.type)
      {
        case DT_REAL:
        {
          *mat_ptr *= a;
        }
        case DT_COMPLEX:
        {
          *mat_ptr *= a;
        }
        case DT_VECTOR:
        {
          if(mat_ptr->width != a.vec_ptr->height)
          {
            int arr[] = {mat_ptr->width, a.vec_ptr->height};
            *this = Token(new InfoLog<2,int>(arr), ERROR_INVALID_2_OPERANDS);
          }
          else
          {
            *mat_ptr *= *a.vec_ptr;
          }
        }
        case DT_MATRIX:
        {
          if(mat_ptr->width != a.mat_ptr->height)
          {
            int arr[] = {mat_ptr->width, a.mat_ptr->height};
            *this = Token(new InfoLog<2,int>(arr), ERROR_INVALID_2_OPERANDS);
          }
        }
        case DT_ALGEBRAIC_EXPR:
        {
          Token a_copy(a);
          applyBinaryOperation(a_copy.expr_ptr, OP_MULTIPLY, *this);
          swap(*this,a_copy);
        }
      }
      case DT_ALGEBRAIC_EXPR:
      {
        switch(a.type)
        {
          case DT_REAL:
          {
            applyBinaryOperation(expr_ptr, OP_MULTIPLY, a);
          }
          case DT_COMPLEX:
          {
            applyBinaryOperation(expr_ptr, OP_MULTIPLY, a);
          }
          case DT_VECTOR:
          {
            applyBinaryOperation(expr_ptr, OP_MULTIPLY, a);
          }
          case DT_MATRIX:
          {
            applyBinaryOperation(expr_ptr, OP_MULTIPLY, a);
          }
          case DT_ALGEBRAIC_EXPR:
          {
            ExpressionTreeNode* local_expr_ptr = a.expr_ptr;
            applyBinaryOperation(expr_ptr, OP_MULTIPLY, local_expr_ptr);
          }
        }
      }
    }
  }
}

#endif