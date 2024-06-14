#ifndef FD_DECS_HPP
#define FD_DECS_HPP

#include<vector>
#include<algorithm>
#include<memory>



namespace mymath
{
  struct CopyTag{};
  struct MoveTag{};



  class ExpressionTreeNode;
  class Token;
  
  template<typename T>
  class vecn;
  
  template<typename T>
  class matn;
  
  enum TokenType
  {
  
    OP_ADD,OP_SUBTRACT,OP_MULTIPLY,OP_DIVIDE,OP_NEGATE,OP_POWER,OP_ROOT,
    OP_SIN,OP_COS,OP_TAN,OP_ASIN,OP_ACOS,OP_ATAN,OP_SINH,OP_COSH,OP_TANH,
    OP_LOG,OP_LOGAB,OP_NATURAL_LOG,OP_EXP,OP_SQRT,OP_CUBEROOT,OP_ABS,

    DT_UNINIT,DT_REAL,DT_COMPLEX,DT_VECTOR,DT_MATRIX,DT_ALGEBRAIC_EXPR,



    ERROR_INVALID_1_OPERANDS,ERROR_INVALID_2_OPERANDS,ERROR_INVALID_3_OPERANDS,

    ERROR_DIV_BY_ZERO,ERROR_INVALID_EXPRESSION,ERROR_INVALID_MAT_DIMS,ERROR_INVALID_VEC_DIMS
  };

  template<typename T>
  class matn
  {
    public:
      int width, height;
      T* data;
      matn(int _width, int _height);
      matn(int _width, int _height, T* newdata);
      matn(const matn<T>& other);
      matn(matn<T>&& other);
      ~matn();
      matn<T>& operator=(const matn<T>& other);
      matn<T>& operator=(matn<T>&& other);
      T& at(int index) const;
      T& at(int x, int y) const;
      T* row_at(int y) const;
      //void operator+=(matn<T> other);
      //void operator+=(vecn<T> other);
  };

  template<typename T>
  class vecn
  {
    public:
      int height;
      T* data;
      vecn(int _height);
      vecn(int _height, T* newdata);
      vecn(const vecn<T>& other);
      vecn(vecn<T>&& other);
      ~vecn();
      vecn<T>& operator=(const vecn<T>& other);
      vecn<T>& operator=(vecn<T>&& other);
      T& at(int index) const;
      //void operator+=(vecn<T> other);

  };

  template<int size, typename T>
  class InfoLog
  {
    public:
      T* ops;
      InfoLog(T* _ops)
      {
        std::copy(_ops, _ops + size, ops);
      }
  };

  class MatDimension
  {
    public:
      int width, height;
      MatDimension(int _width, int _height) : width(_width), height(_height)  {}
  };


  class Token
  {
    public:
      void* dataptr;
      TokenType type;
      Token();
      Token(void* _dataptr,TokenType _type);

      ~Token();
      Token(const Token& other);
      Token(Token&& other);
      Token& operator=(const Token& other);
      Token& operator=(Token&& other);
      void empty();
  };

  
  class ExpressionTreeNode
  {
    public:
      std::vector<ExpressionTreeNode*> children;
      Token data;
      ExpressionTreeNode();
      ExpressionTreeNode(const Token& _data);
      ExpressionTreeNode(Token&& _data);
      ExpressionTreeNode(const ExpressionTreeNode& other);
      ExpressionTreeNode(ExpressionTreeNode&& other);
      ExpressionTreeNode& operator=(const ExpressionTreeNode& other);
      ExpressionTreeNode& operator=(ExpressionTreeNode&& other);
  };
}

void applyBinaryOperation(mymath::ExpressionTreeNode*& tree, mymath::TokenType op, const mymath::Token& other);
void applyBinaryOperation(const mymath::Token& other, mymath::TokenType op, mymath::ExpressionTreeNode*& tree);
void applyBinaryOperation(mymath::ExpressionTreeNode*& tree, mymath::TokenType op, mymath::ExpressionTreeNode*& other);
void applyUnaryOperation(mymath::ExpressionTreeNode*& tree, mymath::TokenType op);

void formatArithmeticChains(mymath::ExpressionTreeNode*& tree);

void formatAddSubChain(mymath::ExpressionTreeNode*& tree);
void getAddSubNodes(mymath::ExpressionTreeNode*& currentNode,std::vector<mymath::ExpressionTreeNode*>& currentAddNodes, std::vector<mymath::ExpressionTreeNode*>& currentSubNodes, mymath::TokenType aboveNodeType = mymath::TokenType::DT_UNINIT);

void formatMulDivChain(mymath::ExpressionTreeNode*& tree, bool aboveIsNumerator = true);
void getMulDivNodes(mymath::ExpressionTreeNode*& currentNode,std::vector<mymath::ExpressionTreeNode*>& currentNumeratorNodes, std::vector<mymath::ExpressionTreeNode*>& currentDenominatorNodes, bool aboveIsNumerator = true);



mymath::Token operator+(mymath::Token& a, mymath::Token& b);


template<typename T>
mymath::vecn<T> operator+(const mymath::vecn<T>& a, const mymath::vecn<T>& b);
template<typename T>
mymath::vecn<T> operator-(const mymath::vecn<T>& a, const mymath::vecn<T>& b);


template<typename T>
mymath::matn<T> operator+(const mymath::matn<T>& a, const mymath::matn<T>& b);
template<typename T>
mymath::matn<T> operator-(const mymath::matn<T>& a, const mymath::matn<T>& b);
template<typename T>
mymath::matn<T> operator*(const mymath::matn<T>& a, const mymath::matn<T>& b);
template<typename T>
mymath::matn<T> gaussianInverse(const mymath::matn<T>& a);

template<typename T>
mymath::vecn<T> operator*(const mymath::matn<T>& a, const mymath::vecn<T>& b);
template<typename T>
mymath::matn<T> operator+(const mymath::matn<T>& a, const mymath::vecn<T>& b);



#include"math/matvec/vec.hpp"
#include"math/matvec/mat.hpp"
#include"math/token.hpp"
#include"math/matvec/matvec_operators.hpp"
#include"math/expressiontree/expressiontree.hpp"

#endif