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
  
  Token& Token::operator=(const Token& other)
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
    return *this;
  }
  
  Token& Token::operator=(Token&& other)
  {
    dataptr = other.dataptr;
    type = other.type;
    other.empty();
    return *this;
  }
  void Token::empty()
  {
    dataptr = nullptr;
    type = DT_UNINIT;
  }
}
  mymath::Token operator+(mymath::Token&& a, mymath::Token&& b) // DO NOT USE THE INPUTTED TOKENS
  {
    mymath::Token new_token;


    switch(a.type)
    {
      case mymath::TokenType::DT_REAL:
      {
        long double* real_a_ptr = static_cast<long double*>(a.dataptr);
        switch(b.type)
        {
          case mymath::TokenType::DT_REAL:
          {
            *real_a_ptr += *static_cast<long double*>(b.dataptr);
            new_token = mymath::Token(a.dataptr  ,  mymath::TokenType::DT_REAL);
            a.empty();
            b.empty();
            break;
          }
          case mymath::TokenType::DT_COMPLEX:
          {
            *static_cast<std::complex<mymath::Token>*>(b.dataptr) = a + *static_cast<std::complex<mymath::Token>*>(b.dataptr);
            new_token = mymath::Token(b.dataptr  ,  mymath::TokenType::DT_COMPLEX);
            a.empty();
            b.empty();
            break;
          }
          case mymath::TokenType::DT_ALGEBRAIC_EXPR:
          {
            //new_token = mymath::Token(new mymath::ExpressionTreeNode(a + *std::static_pointer_cast<mymath::ExpressionTreeNode*>(b.dataptr))  ,  mymath::TokenType::DT_ALGEBRAIC_EXPR);
            // KEY EXMAPLE ON HOW TO USE THE EXPR TREE
            mymath::ExpressionTreeNode* new_tree = static_cast<mymath::ExpressionTreeNode*>(b.dataptr);
            applyBinaryOperation(new_tree,mymath::TokenType::OP_ADD,a);
            new_token = mymath::Token(new_tree,mymath::TokenType::DT_ALGEBRAIC_EXPR);
            a.empty();
            b.empty();
            break;
          }
          default:
          {
            mymath::TokenType arr[] = {a.type,b.type};
            new_token = mymath::Token(static_cast<void*>(new mymath::InfoLog<2,mymath::TokenType>(arr)),mymath::TokenType::ERROR_INVALID_2_OPERANDS);
            a.empty();
            b.empty();
            break;
          }
        }
        break;
      }
      case mymath::TokenType::DT_COMPLEX:
      {
        std::complex<mymath::Token>* complex_a_ptr = static_cast<std::complex<mymath::Token>*>(a.dataptr);
        switch(b.type)
        {
          case mymath::TokenType::DT_REAL:
          {
            *complex_a_ptr = b + *complex_a_ptr;
            new_token = mymath::Token(a.dataptr  ,  mymath::TokenType::DT_COMPLEX);
            a.empty();
            b.empty();
            break;
          }
          case mymath::TokenType::DT_COMPLEX:
          {
            *complex_a_ptr = *static_cast<std::complex<mymath::Token>*>(b.dataptr) + *complex_a_ptr;
            new_token = mymath::Token(a.dataptr  ,  mymath::TokenType::DT_COMPLEX);
            a.empty();
            b.empty();
            break;
          }
          case mymath::TokenType::DT_ALGEBRAIC_EXPR:
          {
            mymath::ExpressionTreeNode* new_tree = static_cast<mymath::ExpressionTreeNode*>(b.dataptr);
            applyBinaryOperation(new_tree,mymath::TokenType::OP_ADD,a);
            new_token = mymath::Token(new_tree,mymath::TokenType::DT_ALGEBRAIC_EXPR);
            a.empty();
            b.empty();
            break;
          }
          default:
          {
            mymath::TokenType arr[] = {a.type, b.type};
            new_token = mymath::Token(static_cast<void*>(new mymath::InfoLog<2,mymath::TokenType>(arr)), mymath::TokenType::ERROR_INVALID_2_OPERANDS);
            a.empty();
            b.empty();
            break;
          }
        }
        break;
      }
      case mymath::TokenType::DT_VECTOR:
      {
        mymath::vecn<mymath::Token>* vector_a_ptr = static_cast<mymath::vecn<mymath::Token>*>(a.dataptr);
        switch(b.type)
        {
          case mymath::TokenType::DT_VECTOR:
          {
            mymath::vecn<mymath::Token>* vector_b_ptr = static_cast<mymath::vecn<mymath::Token>*>(b.dataptr);
            if(vector_a_ptr->height != vector_b_ptr->height)
            {
              int arr[] = {vector_a_ptr->height,vector_b_ptr->height};
              new_token = mymath::Token(new mymath::InfoLog<2,int>(arr), mymath::TokenType::ERROR_INVALID_VEC_DIMS);
              a.empty();
              b.empty();
              break;
            }
            else
            {
              *vector_a_ptr = *vector_a_ptr + *vector_b_ptr;
              new_token = mymath::Token(a.dataptr  ,  mymath::TokenType::DT_VECTOR);
              a.empty();
              b.empty();
              break;
            }
            break;
          }
          case mymath::TokenType::DT_MATRIX:
          {
            mymath::matn<mymath::Token>* matrix_b_ptr = static_cast<mymath::matn<mymath::Token>*>(b.dataptr);
            if(vector_a_ptr->height != matrix_b_ptr->width  &&  matrix_b_ptr->height != 1)
            {
              mymath::MatDimension arr[] = {mymath::MatDimension(vector_a_ptr->height,1) ,
                mymath::MatDimension(matrix_b_ptr->width,matrix_b_ptr->height)};
              new_token = mymath::Token(new mymath::InfoLog<2,mymath::MatDimension>(arr)  ,  mymath::TokenType::ERROR_INVALID_MAT_DIMS);
              a.empty();
              b.empty();
              break;
            }
            else
            {
              *matrix_b_ptr = *matrix_b_ptr + *vector_a_ptr;
              new_token = mymath::Token(b.dataptr  ,  mymath::TokenType::DT_MATRIX);
              a.empty();
              b.empty();
              break;
            }
          }
          case mymath::TokenType::DT_ALGEBRAIC_EXPR:
          {
            mymath::ExpressionTreeNode* new_tree = static_cast<mymath::ExpressionTreeNode*>(b.dataptr);
            applyBinaryOperation(new_tree,mymath::TokenType::OP_ADD,a);
            new_token = mymath::Token(new_tree, mymath::TokenType::DT_ALGEBRAIC_EXPR);
            a.empty();
            b.empty();
            break;
          }
          default:
          {
            mymath::TokenType arr[] = {a.type,b.type};
            new_token = mymath::Token(new mymath::InfoLog<2,mymath::TokenType>(arr), mymath::TokenType::ERROR_INVALID_2_OPERANDS);
            a.empty();
            b.empty();
            break;
          }
        }
        break;
      }
      case mymath::TokenType::DT_MATRIX:
      {
        mymath::matn<mymath::Token>* matrix_a_ptr = static_cast<mymath::matn<mymath::Token>*>(a.dataptr);
        switch(b.type)
        {
          case mymath::TokenType::DT_VECTOR:
          {
            mymath::vecn<mymath::Token>* vector_b_ptr = static_cast<mymath::vecn<mymath::Token>*>(b.dataptr);
            if(matrix_a_ptr->width != vector_b_ptr->height  &&  matrix_a_ptr->height != 1)
            {
              mymath::MatDimension arr[] = {mymath::MatDimension(matrix_a_ptr->width,matrix_a_ptr->height) ,
                mymath::MatDimension(vector_b_ptr->height,1)};
              new_token = mymath::Token(new mymath::InfoLog<2,mymath::MatDimension>(arr)  ,  mymath::TokenType::ERROR_INVALID_MAT_DIMS);
              a.empty();
              b.empty();
              break;
            }
            else
            {
              *matrix_a_ptr = *matrix_a_ptr + *vector_b_ptr;
              new_token = mymath::Token(a.dataptr  ,  mymath::TokenType::DT_MATRIX);
              a.empty();
              b.empty();
              break;
            }
          }
          case mymath::TokenType::DT_MATRIX:
          {
            mymath::matn<mymath::Token>* matrix_b_ptr = static_cast<mymath::matn<mymath::Token>*>(a.dataptr);
            if(matrix_a_ptr->width != matrix_b_ptr->width  &&  matrix_a_ptr->height != matrix_b_ptr->height)
            {
              mymath::MatDimension arr[] = {mymath::MatDimension(matrix_a_ptr->width,matrix_a_ptr->height) ,
                mymath::MatDimension(matrix_b_ptr->width,matrix_b_ptr->height)};
              new_token = mymath::Token(new mymath::InfoLog<2,mymath::MatDimension>(arr)  ,  mymath::TokenType::ERROR_INVALID_MAT_DIMS);
              a.empty();
              b.empty();
              break;
            }
            else
            {
              *matrix_a_ptr = *matrix_a_ptr + *matrix_b_ptr;
              new_token = mymath::Token(a.dataptr  ,  mymath::TokenType::DT_MATRIX);
              a.empty();
              b.empty();
              break;
            }
          }
          case mymath::TokenType::DT_ALGEBRAIC_EXPR:
          {
            mymath::ExpressionTreeNode* new_tree = static_cast<mymath::ExpressionTreeNode*>(b.dataptr);
            applyBinaryOperation(new_tree,mymath::TokenType::OP_ADD,a);
            new_token = mymath::Token(new_tree, mymath::TokenType::DT_ALGEBRAIC_EXPR);
            a.empty();
            b.empty();
            break;
          }
          default:
          {
            mymath::TokenType arr[] = {a.type,b.type};
            new_token = mymath::Token(new mymath::InfoLog<2,mymath::TokenType>(arr), mymath::TokenType::ERROR_INVALID_2_OPERANDS);
            a.empty();
            b.empty();
            break;
          }
        }
        break;
      }
      case mymath::TokenType::DT_ALGEBRAIC_EXPR:
      {
        mymath::ExpressionTreeNode* new_tree = static_cast<mymath::ExpressionTreeNode*>(a.dataptr);
        switch(b.type)
        {
          case mymath::TokenType::DT_REAL:
          {
            applyBinaryOperation(new_tree, mymath::TokenType::OP_ADD, b);
            new_token = mymath::Token(new_tree  ,  mymath::TokenType::DT_ALGEBRAIC_EXPR);
            a.empty();
            b.empty();
            break;
          }
          case mymath::TokenType::DT_COMPLEX:
          {
            applyBinaryOperation(new_tree, mymath::TokenType::OP_ADD, b);
            new_token = mymath::Token(new_tree  ,  mymath::TokenType::DT_ALGEBRAIC_EXPR);
            a.empty();
            b.empty();
            break;
          }
          case mymath::TokenType::DT_VECTOR:
          {
            applyBinaryOperation(new_tree, mymath::TokenType::OP_ADD, b);
            new_token = mymath::Token(new_tree  ,  mymath::TokenType::DT_ALGEBRAIC_EXPR);
            a.empty();
            b.empty();
            break;
          }
          case mymath::TokenType::DT_MATRIX:
          {
            applyBinaryOperation(new_tree, mymath::TokenType::OP_ADD, b);
            new_token = mymath::Token(new_tree  ,  mymath::TokenType::DT_ALGEBRAIC_EXPR);
            a.empty();
            b.empty();
            break;
          }
          case mymath::TokenType::DT_ALGEBRAIC_EXPR:
          {
            mymath::ExpressionTreeNode* tree_b_ptr = static_cast<mymath::ExpressionTreeNode*>(b.dataptr);
            applyBinaryOperation(new_tree, mymath::TokenType::OP_ADD, tree_b_ptr);
            new_token = mymath::Token(new_tree  ,  mymath::TokenType::DT_ALGEBRAIC_EXPR);
            a.empty();
            b.empty();
            break;
          }
          default:
          {
            mymath::TokenType arr[] = {a.type,b.type};
            new_token = mymath::Token(new mymath::InfoLog<2,mymath::TokenType>(arr), mymath::TokenType::ERROR_INVALID_2_OPERANDS);
            a.empty();
            b.empty();
            break;
          }
        }
        break;
      }
      default:
      {
        mymath::TokenType arr[] = {a.type,b.type};
        new_token = mymath::Token(new mymath::InfoLog<2,mymath::TokenType>(arr), mymath::TokenType::ERROR_INVALID_2_OPERANDS);
        a.empty();
        b.empty();
        break;
      }
    }
  return new_token;
}

#endif