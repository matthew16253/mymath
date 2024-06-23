#ifndef TOKEN_OPERATOR_SUBTRACT_EQUALS_HPP
#define TOKEN_OPERATOR_SUBTRACT_EQUALS_HPP

#include"../../../fd_decs.hpp"

namespace mymath
{
void mymath::Token::operator-=(const mymath::Token& b) // DO NOT USE THE INPUTTED TOKENS
{
  using namespace mymath;

  if(b.type == DT_UNINIT)
  {
    return;
  }

  switch(type)
  {
    case DT_REAL:
    {
      switch(b.type)
      {
        case DT_REAL:
        {
          *real_ptr -= *b.real_ptr;
          break;
        }
        case DT_COMPLEX:
        {
          Token this_copy(new std::complex<Token>(*this,Token(new long double(), DT_REAL)), DT_COMPLEX);
          *this_copy.complex_ptr -= *b.complex_ptr;
          swap(*this,this_copy);
          break;
        }
        case DT_ALGEBRAIC_EXPR:
        {
          // ExpressionTreeNode* new_tree = static_cast<ExpressionTreeNode*>(b.dataptr);
          // applyBinaryOperation(new_tree,OP_ADD,*this);
          // new_token = Token(new_tree,DT_ALGEBRAIC_EXPR);
          Token b_copy = b;
          applyBinaryOperation(*this,OP_SUBTRACT,b_copy.expr_ptr);
          swap(*this,b_copy);
          break;
        }
        default:
        {
          TokenType arr[] = {type,b.type};
          *this = Token(new InfoLog<2,TokenType>(arr),ERROR_INVALID_2_OPERANDS);
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
          *complex_ptr -= b;
          break;
        }
        case DT_COMPLEX:
        {
          *complex_ptr -= *b.complex_ptr;
          break;
        }
        case DT_ALGEBRAIC_EXPR:
        {
          Token b_copy(b);
          applyBinaryOperation(*this,OP_SUBTRACT,b_copy.expr_ptr);
          swap(*this,b_copy);
          break;
        }
        default:
        {
          TokenType arr[] = {type, b.type};
          *this = Token(new InfoLog<2,TokenType>(arr), ERROR_INVALID_2_OPERANDS);
          break;
        }
      }
      break;
    }
    case DT_VECTOR:
    {
      switch(b.type)
      {
        case DT_VECTOR:
        {
          if(vec_ptr->height != b.vec_ptr->height)
          {
            int arr[] = {vec_ptr->height,b.vec_ptr->height};
            *this = Token(new InfoLog<2,int>(arr), ERROR_INVALID_VEC_DIMS);
            break;
          }
          else
          {
            *vec_ptr -= *b.vec_ptr;
            break;
          }
          break;
        }
        case DT_MATRIX:
        {
          if(vec_ptr->height != b.mat_ptr->width  &&  b.mat_ptr->height != 1)
          {
            MatDimension arr[] = {MatDimension(vec_ptr->height,1) ,
              MatDimension(b.mat_ptr->width,b.mat_ptr->height)};
            *this = Token(new InfoLog<2,MatDimension>(arr)  ,  ERROR_INVALID_MAT_DIMS);
            break;
          }
          else
          {
            *vec_ptr -= *mat_ptr;
            break;
          }
        }
        case DT_ALGEBRAIC_EXPR:
        {
          Token b_copy(b);
          applyBinaryOperation(*this,OP_SUBTRACT,b_copy.expr_ptr);
          swap(*this,b_copy);
          break;
        }
        default:
        {
          TokenType arr[] = {type,b.type};
          *this = Token(new InfoLog<2,TokenType>(arr), ERROR_INVALID_2_OPERANDS);
          break;
        }
      }
      break;
    }
    case DT_MATRIX:
    {
      switch(b.type)
      {
        case DT_VECTOR:
        {
          if(mat_ptr->width != b.vec_ptr->height  &&  mat_ptr->height != 1)
          {
            MatDimension arr[] = {MatDimension(mat_ptr->width,mat_ptr->height) ,
              MatDimension(b.vec_ptr->height,1)};
            *this = Token(new InfoLog<2,MatDimension>(arr)  ,  ERROR_INVALID_MAT_DIMS);
            break;
          }
          else
          {
            *mat_ptr -= *b.vec_ptr;
            break;
          }
        }
        case DT_MATRIX:
        {
          if(mat_ptr->width != b.mat_ptr->width  &&  mat_ptr->height != b.mat_ptr->height)
          {
            MatDimension arr[] = {MatDimension(mat_ptr->width,mat_ptr->height) ,
              MatDimension(b.mat_ptr->width,b.mat_ptr->height)};
            *this = Token(new InfoLog<2,MatDimension>(arr)  ,  ERROR_INVALID_MAT_DIMS);
            break;
          }
          else
          {
            *mat_ptr -= *b.mat_ptr;
            break;
          }
        }
        case DT_ALGEBRAIC_EXPR:
        {
          Token b_copy(b);
          applyBinaryOperation(b_copy.expr_ptr,OP_SUBTRACT,*this);
          swap(*this,b_copy);
          break;
        }
        default:
        {
          TokenType arr[] = {type,b.type};
          *this = Token(new InfoLog<2,TokenType>(arr), ERROR_INVALID_2_OPERANDS);
          break;
        }
      }
      break;
    }
    case DT_ALGEBRAIC_EXPR:
    {
      switch(b.type)
      {
        case DT_REAL:
        {
          applyBinaryOperation(expr_ptr, OP_SUBTRACT, b);
          break;
        }
        case DT_COMPLEX:
        {
          applyBinaryOperation(expr_ptr, OP_SUBTRACT, b);
          break;
        }
        case DT_VECTOR:
        {
          applyBinaryOperation(expr_ptr, OP_SUBTRACT, b);
          break;
        }
        case DT_MATRIX:
        {
          applyBinaryOperation(expr_ptr, OP_SUBTRACT, b);
          break;
        }
        case DT_ALGEBRAIC_EXPR:
        {
          Token b_copy(b);
          applyBinaryOperation(expr_ptr, OP_SUBTRACT, b_copy.expr_ptr);
          break;
        }
        default:
        {
          TokenType arr[] = {type,b.type};
          *this = Token(new InfoLog<2,TokenType>(arr), ERROR_INVALID_2_OPERANDS);
          break;
        }
      }
      break;
    }
    case DT_UNINIT:
    {
      *this = -b;
      break;
    }
    default:
    {
      TokenType arr[] = {type,b.type};
      *this = Token(new InfoLog<2,TokenType>(arr), ERROR_INVALID_2_OPERANDS);
      break;
    }
  }
}
}
#endif