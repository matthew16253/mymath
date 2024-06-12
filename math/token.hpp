#ifndef TOKEN_HPP
#define TOKEN_HPP

#include<complex>

#include<fd_decs.hpp>

namespace mymath
{
  Token::Token() : dataptr(nullptr), type(DT_UNINIT)  {}
  Token::Token(std::shared_ptr<void> _dataptr,TokenType _type) : dataptr(_dataptr), type(_type)  {}
  Token::~Token()
  {
    // switch(type)
    // {
    //   case DT_REAL:  {delete static_cast<long double*>(dataptr);break;}
    //   case DT_COMPLEX:  {delete static_cast<std::complex<Token>*>(dataptr);break;}
    //   case DT_VECTOR:  {delete static_cast<mymath::vecn<Token>*>(dataptr);break;}
    //   case DT_MATRIX:  {delete static_cast<mymath::matn<Token>*>(dataptr);break;}
    //   case DT_ALGEBRAIC_EXPR:  {delete static_cast<ExpressionTreeNode*>(dataptr);break;}
    //   default:  {}
    // }
  }
  Token::Token(const Token& other, CopyTag)  :  type(other.type)
  {
    switch(other.type)
    {
      case DT_REAL:
        {dataptr = std::static_pointer_cast<void>(std::make_shared<long double>(std::static_pointer_cast<long double>(other.dataptr)));  break;}
      case DT_COMPLEX:
        {dataptr = std::static_pointer_cast<void>(std::make_shared<std::complex<Token>>(std::static_pointer_cast<std::complex<Token>>(other.dataptr)));  break;}
      case DT_VECTOR:
        {dataptr = std::static_pointer_cast<void>(std::make_shared<long double>(std::static_pointer_cast<long double>(other.dataptr)));  break;}
      case DT_MATRIX:
        {dataptr = std::static_pointer_cast<void>(std::make_shared<long double>(std::static_pointer_cast<long double>(other.dataptr)));  break;}
      case DT_ALGEBRAIC_EXPR:
        {dataptr = std::static_pointer_cast<void>(std::make_shared<long double>(std::static_pointer_cast<long double>(other.dataptr)));  break;}
      default:  {}
    }
  }
  Token::Token(const Token& other, MoveTag) : dataptr(other.dataptr), type(other.type)
  {
    ;
  }
  Token::Token(Token&& other) : dataptr(other.dataptr), type(other.type)
  {
    ;
  }
  
  Token& Token::operator=(const Token& other)
  {
    dataptr = other.dataptr;
    type = other.type;
    return *this;
  }
  
  Token& Token::operator=(Token&& other)
  {
    dataptr = other.dataptr;
    type = other.type;
    return *this;
  }
}
  mymath::Token operator+(mymath::Token& a, mymath::Token& b) // DO NOT USE THE INPUTTED TOKENS
  {
    switch(a.type)
    {
      case mymath::TokenType::DT_REAL:
      {
        std::shared_ptr<long double> real_a_ptr = std::static_pointer_cast<long double>(a.dataptr);
        switch(b.type)
        {
          case mymath::TokenType::DT_REAL:
          {
            *real_a_ptr += *std::static_pointer_cast<long double>(b.dataptr);
            return mymath::Token(a.dataptr  ,  mymath::TokenType::DT_REAL);
            break;
          }
          case mymath::TokenType::DT_COMPLEX:
          {
            *std::static_pointer_cast<std::complex<mymath::Token>>(b.dataptr) += a;
            return mymath::Token(b.dataptr  ,  mymath::TokenType::DT_COMPLEX);
            break;
          }
          case mymath::TokenType::DT_ALGEBRAIC_EXPR:
          {
            //return mymath::Token(new mymath::ExpressionTreeNode(a + *std::static_pointer_cast<mymath::ExpressionTreeNode*>(b.dataptr))  ,  mymath::TokenType::DT_ALGEBRAIC_EXPR);
            // KEY EXMAPLE ON HOW TO USE THE EXPR TREE
            std::shared_ptr<mymath::ExpressionTreeNode> new_tree = std::static_pointer_cast<mymath::ExpressionTreeNode>(b.dataptr);
            applyBinaryOperation(new_tree,mymath::TokenType::OP_ADD,a);
            return mymath::Token(new_tree,mymath::TokenType::DT_ALGEBRAIC_EXPR);
            break;
          }
          default:
          {
            mymath::TokenType arr[] = {a.type,b.type};
            return mymath::Token(std::static_pointer_cast<void>(std::make_shared<mymath::InfoLog<2,mymath::TokenType>>(arr)),mymath::TokenType::ERROR_INVALID_2_OPERANDS);
          }
        }
        break;
      }
      case mymath::TokenType::DT_COMPLEX:
      {
        std::shared_ptr<std::complex<mymath::Token>> complex_a_ptr = std::static_pointer_cast<std::complex<mymath::Token>>(a.dataptr);
        switch(b.type)
        {
          case mymath::TokenType::DT_REAL:
          {
            *complex_a_ptr += b;
            return mymath::Token(a.dataptr  ,  mymath::TokenType::DT_COMPLEX);
            break;
          }
          case mymath::TokenType::DT_COMPLEX:
          {
            *complex_a_ptr += *std::static_pointer_cast<std::complex<mymath::Token>>(b.dataptr);
            return mymath::Token(a.dataptr  ,  mymath::TokenType::DT_COMPLEX);
            break;
          }
          case mymath::TokenType::DT_ALGEBRAIC_EXPR:
          {
            std::shared_ptr<mymath::ExpressionTreeNode> new_tree = std::static_pointer_cast<mymath::ExpressionTreeNode>(b.dataptr);
            applyBinaryOperation(new_tree,mymath::TokenType::OP_ADD,a);
            return mymath::Token(new_tree,mymath::TokenType::DT_ALGEBRAIC_EXPR);
            break;
          }
          default:
          {
            mymath::TokenType arr[] = {a.type, b.type};
            return mymath::Token(std::static_pointer_cast<void>(std::make_shared<mymath::InfoLog<2,mymath::TokenType>>(arr)), mymath::TokenType::ERROR_INVALID_2_OPERANDS);
          }
        }
        break;
      }
      case mymath::TokenType::DT_VECTOR:
      {
        std::shared_ptr<mymath::vecn<mymath::Token>> vector_a_ptr = std::static_pointer_cast<mymath::vecn<mymath::Token>>(a.dataptr);
        switch(b.type)
        {
          case mymath::TokenType::DT_VECTOR:
          {
            std::shared_ptr<mymath::vecn<mymath::Token>> vector_b_ptr = std::static_pointer_cast<mymath::vecn<mymath::Token>>(b.dataptr);
            if(vector_a_ptr->height != vector_b_ptr->height)
            {
              int arr[] = {vector_a_ptr->height,vector_b_ptr->height};
              return mymath::Token(std::make_shared<mymath::InfoLog<2,int>>(arr), mymath::TokenType::ERROR_INVALID_VEC_DIMS);
            }
            else
            {
              *vector_a_ptr += *vector_b_ptr;
              return mymath::Token(a.dataptr  ,  mymath::TokenType::DT_VECTOR);
            }
            break;
          }
          case mymath::TokenType::DT_MATRIX:
          {
            std::shared_ptr<mymath::matn<mymath::Token>> matrix_b_ptr = std::static_pointer_cast<mymath::matn<mymath::Token>>(b.dataptr);
            if(vector_a_ptr->height != matrix_b_ptr->width  &&  matrix_b_ptr->height != 1)
            {
              mymath::MatDimension arr[] = {mymath::MatDimension(vector_a_ptr->height,1) ,
                mymath::MatDimension(matrix_b_ptr->width,matrix_b_ptr->height)};
              return mymath::Token(std::make_shared<mymath::InfoLog<2,mymath::MatDimension>>(arr)  ,  mymath::TokenType::ERROR_INVALID_MAT_DIMS);
            }
            else
            {
              *matrix_b_ptr += *vector_a_ptr;
              return mymath::Token(b.dataptr  ,  mymath::TokenType::DT_MATRIX);
            }
            break;
          }
          case mymath::TokenType::DT_ALGEBRAIC_EXPR:
          {
            std::shared_ptr<mymath::ExpressionTreeNode> new_tree = std::static_pointer_cast<mymath::ExpressionTreeNode>(b.dataptr);
            applyBinaryOperation(new_tree,mymath::TokenType::OP_ADD,a);
            return mymath::Token(new_tree, mymath::TokenType::DT_ALGEBRAIC_EXPR);
            break;
          }
          default:
          {
            mymath::TokenType arr[] = {a.type,b.type};
            return mymath::Token(std::make_shared<mymath::InfoLog<2,mymath::TokenType>>(arr), mymath::TokenType::ERROR_INVALID_2_OPERANDS);
          }
        }
        break;
      }
      case mymath::TokenType::DT_MATRIX:
      {
        std::shared_ptr<mymath::matn<mymath::Token>> matrix_a_ptr = std::static_pointer_cast<mymath::matn<mymath::Token>>(a.dataptr);
        switch(b.type)
        {
          case mymath::TokenType::DT_VECTOR:
          {
            std::shared_ptr<mymath::vecn<mymath::Token>> vector_b_ptr = std::static_pointer_cast<mymath::vecn<mymath::Token>>(b.dataptr);
            if(matrix_a_ptr->width != vector_b_ptr->height  &&  matrix_a_ptr->height != 1)
            {
              mymath::MatDimension arr[] = {mymath::MatDimension(matrix_a_ptr->width,matrix_a_ptr->height) ,
                mymath::MatDimension(vector_b_ptr->height,1)};
              return mymath::Token(std::make_shared<mymath::InfoLog<2,mymath::MatDimension>>(arr)  ,  mymath::TokenType::ERROR_INVALID_MAT_DIMS);
            }
            else
            {
              *matrix_a_ptr += *vector_b_ptr;
              return mymath::Token(a.dataptr  ,  mymath::TokenType::DT_MATRIX);
            }
            break;
          }
          case mymath::TokenType::DT_MATRIX:
          {
            std::shared_ptr<mymath::matn<mymath::Token>> matrix_b_ptr = std::static_pointer_cast<mymath::matn<mymath::Token>>(a.dataptr);
            if(matrix_a_ptr->width != matrix_b_ptr->width  &&  matrix_a_ptr->height != matrix_b_ptr->height)
            {
              mymath::MatDimension arr[] = {mymath::MatDimension(matrix_a_ptr->width,matrix_a_ptr->height) ,
                mymath::MatDimension(matrix_b_ptr->width,matrix_b_ptr->height)};
              return mymath::Token(std::make_shared<mymath::InfoLog<2,mymath::MatDimension>>(arr)  ,  mymath::TokenType::ERROR_INVALID_MAT_DIMS);
            }
            else
            {
              *matrix_a_ptr += *matrix_b_ptr;
              return mymath::Token(a.dataptr  ,  mymath::TokenType::DT_MATRIX);
            }
            break;
          }
          case mymath::TokenType::DT_ALGEBRAIC_EXPR:
          {
            std::shared_ptr<mymath::ExpressionTreeNode> new_tree = std::static_pointer_cast<mymath::ExpressionTreeNode>(b.dataptr);
            applyBinaryOperation(new_tree,mymath::TokenType::OP_ADD,a);
            return mymath::Token(new_tree, mymath::TokenType::DT_ALGEBRAIC_EXPR);
            break;
          }
          default:
          {
            mymath::TokenType arr[] = {a.type,b.type};
            return mymath::Token(std::make_shared<mymath::InfoLog<2,mymath::TokenType>>(arr), mymath::TokenType::ERROR_INVALID_2_OPERANDS);
          }
        }
        break;
      }
      case mymath::TokenType::DT_ALGEBRAIC_EXPR:
      {
        std::shared_ptr<mymath::ExpressionTreeNode> new_tree = std::static_pointer_cast<mymath::ExpressionTreeNode>(a.dataptr);
        switch(b.type)
        {
          case mymath::TokenType::DT_REAL:
          {
            applyBinaryOperation(new_tree, mymath::TokenType::OP_ADD, b);
            return mymath::Token(new_tree  ,  mymath::TokenType::DT_ALGEBRAIC_EXPR);
            break;
          }
          case mymath::TokenType::DT_COMPLEX:
          {
            applyBinaryOperation(new_tree, mymath::TokenType::OP_ADD, b);
            return mymath::Token(new_tree  ,  mymath::TokenType::DT_ALGEBRAIC_EXPR);
            break;
          }
          case mymath::TokenType::DT_VECTOR:
          {
            applyBinaryOperation(new_tree, mymath::TokenType::OP_ADD, b);
            return mymath::Token(new_tree  ,  mymath::TokenType::DT_ALGEBRAIC_EXPR);
            break;
          }
          case mymath::TokenType::DT_MATRIX:
          {
            applyBinaryOperation(new_tree, mymath::TokenType::OP_ADD, b);
            return mymath::Token(new_tree  ,  mymath::TokenType::DT_ALGEBRAIC_EXPR);
            break;
          }
          case mymath::TokenType::DT_ALGEBRAIC_EXPR:
          {
            std::shared_ptr<mymath::ExpressionTreeNode> tree_b_ptr = std::static_pointer_cast<mymath::ExpressionTreeNode>(b.dataptr);
            applyBinaryOperation(new_tree, mymath::TokenType::OP_ADD, tree_b_ptr);
            return mymath::Token(new_tree  ,  mymath::TokenType::DT_ALGEBRAIC_EXPR);
            break;
          }
          default:
          {
            mymath::TokenType arr[] = {a.type,b.type};
            return mymath::Token(std::make_shared<mymath::InfoLog<2,mymath::TokenType>>(arr), mymath::TokenType::ERROR_INVALID_2_OPERANDS);
          }
        }
        break;
      }
      default:
      {
        mymath::TokenType arr[] = {a.type,b.type};
        return mymath::Token(std::make_shared<mymath::InfoLog<2,mymath::TokenType>>(arr), mymath::TokenType::ERROR_INVALID_2_OPERANDS);
      }
    }
  
}

#endif