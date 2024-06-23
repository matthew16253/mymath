#ifndef TOKEN_OPERATOR_PLUS_EQUALS_HPP
#define TOKEN_OPERATOR_PLUS_EQUALS_HPP

#include"../../../fd_decs.hpp"

namespace mymath
{
  void mymath::Token::operator+=(const mymath::Token& b) // DO NOT USE THE INPUTTED TOKENS
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
        long double* real_a_ptr = static_cast<long double*>(dataptr);
        switch(b.type)
        {
          case DT_REAL:
          {
            *real_a_ptr += *static_cast<long double*>(b.dataptr);
            std::cout<<*static_cast<long double*>(dataptr)<<"\n";
            break;
          }
          case DT_COMPLEX:
          {
            Token b_copy(b);
            *static_cast<std::complex<Token>*>(b_copy.dataptr) += *this;
            swap(*this,b_copy);
            break;
          }
          case DT_ALGEBRAIC_EXPR:
          {
            // ExpressionTreeNode* new_tree = static_cast<ExpressionTreeNode*>(b.dataptr);
            // applyBinaryOperation(new_tree,OP_ADD,*this);
            // new_token = Token(new_tree,DT_ALGEBRAIC_EXPR);
            Token b_copy = b;
            ExpressionTreeNode* b_copy_expr_ptr = static_cast<ExpressionTreeNode*>(b_copy.dataptr);
            applyBinaryOperation(b_copy_expr_ptr,OP_ADD,*this);
            b_copy.dataptr = b_copy_expr_ptr;
            swap(*this,b_copy);
            break;
          }
          default:
          {
            TokenType arr[] = {type,b.type};
            *this = Token(static_cast<void*>(new InfoLog<2,TokenType>(arr)),ERROR_INVALID_2_OPERANDS);
            break;
          }
        }
        break;
      }
      case DT_COMPLEX:
      {
        std::complex<Token>* complex_a_ptr = static_cast<std::complex<Token>*>(dataptr);
        switch(b.type)
        {
          case DT_REAL:
          {
            *complex_a_ptr += b;
            break;
          }
          case DT_COMPLEX:
          {
            *complex_a_ptr += *complex_a_ptr;
            break;
          }
          case DT_ALGEBRAIC_EXPR:
          {
            Token b_copy(b);
            ExpressionTreeNode* b_copy_expr_ptr = static_cast<ExpressionTreeNode*>(b_copy.dataptr);
            applyBinaryOperation(b_copy_expr_ptr,OP_ADD,*this);
            b_copy.dataptr = b_copy_expr_ptr;
            swap(*this,b_copy);
            break;
          }
          default:
          {
            TokenType arr[] = {type, b.type};
            *this = Token(static_cast<void*>(new InfoLog<2,TokenType>(arr)), ERROR_INVALID_2_OPERANDS);
            break;
          }
        }
        break;
      }
      case DT_VECTOR:
      {
        vecn<Token>* vector_a_ptr = static_cast<vecn<Token>*>(dataptr);
        switch(b.type)
        {
          case DT_VECTOR:
          {
            vecn<Token>* vector_b_ptr = static_cast<vecn<Token>*>(b.dataptr);
            if(vector_a_ptr->height != vector_b_ptr->height)
            {
              int arr[] = {vector_a_ptr->height,vector_b_ptr->height};
              *this = Token(new InfoLog<2,int>(arr), ERROR_INVALID_VEC_DIMS);
              break;
            }
            else
            {
              *vector_a_ptr += *vector_b_ptr;
              break;
            }
            break;
          }
          case DT_MATRIX:
          {
            matn<Token>* matrix_b_ptr = static_cast<matn<Token>*>(b.dataptr);
            if(vector_a_ptr->height != matrix_b_ptr->width  &&  matrix_b_ptr->height != 1)
            {
              MatDimension arr[] = {MatDimension(vector_a_ptr->height,1) ,
                MatDimension(matrix_b_ptr->width,matrix_b_ptr->height)};
              *this = Token(new InfoLog<2,MatDimension>(arr)  ,  ERROR_INVALID_MAT_DIMS);
              break;
            }
            else
            {
              *vector_a_ptr += *matrix_b_ptr;
              break;
            }
          }
          case DT_ALGEBRAIC_EXPR:
          {
            Token b_copy(b);
            ExpressionTreeNode* b_copy_expr_ptr = static_cast<ExpressionTreeNode*>(b_copy.dataptr);
            applyBinaryOperation(b_copy_expr_ptr,OP_ADD,*this);
            b_copy.dataptr = b_copy_expr_ptr;
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
        matn<Token>* matrix_a_ptr = static_cast<matn<Token>*>(dataptr);
        switch(b.type)
        {
          case DT_VECTOR:
          {
            vecn<Token>* vector_b_ptr = static_cast<vecn<Token>*>(b.dataptr);
            if(matrix_a_ptr->width != vector_b_ptr->height  &&  matrix_a_ptr->height != 1)
            {
              MatDimension arr[] = {MatDimension(matrix_a_ptr->width,matrix_a_ptr->height) ,
                MatDimension(vector_b_ptr->height,1)};
              *this = Token(new InfoLog<2,MatDimension>(arr)  ,  ERROR_INVALID_MAT_DIMS);
              break;
            }
            else
            {
              *matrix_a_ptr += *vector_b_ptr;
              break;
            }
          }
          case DT_MATRIX:
          {
            matn<Token>* matrix_b_ptr = static_cast<matn<Token>*>(dataptr);
            if(matrix_a_ptr->width != matrix_b_ptr->width  &&  matrix_a_ptr->height != matrix_b_ptr->height)
            {
              MatDimension arr[] = {MatDimension(matrix_a_ptr->width,matrix_a_ptr->height) ,
                MatDimension(matrix_b_ptr->width,matrix_b_ptr->height)};
              *this = Token(new InfoLog<2,MatDimension>(arr)  ,  ERROR_INVALID_MAT_DIMS);
              break;
            }
            else
            {
              *matrix_a_ptr += *matrix_b_ptr;
              break;
            }
          }
          case DT_ALGEBRAIC_EXPR:
          {
            Token b_copy(b);
            ExpressionTreeNode* b_copy_expr_ptr = static_cast<ExpressionTreeNode*>(b_copy.dataptr);
            applyBinaryOperation(b_copy_expr_ptr,OP_ADD,*this);
            b_copy.dataptr = b_copy_expr_ptr;
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
        ExpressionTreeNode* new_tree = static_cast<ExpressionTreeNode*>(dataptr);
        switch(b.type)
        {
          case DT_REAL:
          {
            applyBinaryOperation(new_tree, OP_ADD, b);
            dataptr = new_tree;
            break;
          }
          case DT_COMPLEX:
          {
            applyBinaryOperation(new_tree, OP_ADD, b);
            dataptr = new_tree;
            break;
          }
          case DT_VECTOR:
          {
            applyBinaryOperation(new_tree, OP_ADD, b);
            dataptr = new_tree;
            break;
          }
          case DT_MATRIX:
          {
            applyBinaryOperation(new_tree, OP_ADD, b);
            dataptr = new_tree;
            break;
          }
          case DT_ALGEBRAIC_EXPR:
          {
            ExpressionTreeNode* tree_b_ptr = static_cast<ExpressionTreeNode*>(b.dataptr);
            applyBinaryOperation(new_tree, OP_ADD, tree_b_ptr);
            dataptr = new_tree;
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
        *this = b;
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