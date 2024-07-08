#ifndef FD_DECS_HPP
#define FD_DECS_HPP

#include<vector>
#include<algorithm>
#include<memory>
#include<complex>
#include<cmath>
#include<variant>
#include<boost/multiprecision/cpp_dec_float.hpp>
#include<type_traits>

template<typename command, typename ...Args>
consteval bool functionExists()
{
  ;
}

namespace mymath
{

  template<typename T>
  bool operator!=(T a, T b)
  {
    return !(a==b);
  }

  bool isInt(long double a)
  {
    long double intPart;
    std::modf(a, &intPart);
    return (a == intPart);
  }

  class ExpressionTreeNode;
  struct ExpressionTreeNodePtr;
  class Token;
  
  template<typename T>
  class vecn;
  
  template<typename T>
  class matn;
  
  enum TokenType
  {
    OPERATORS_BEGIN,
    OP_ADD,OP_SUBTRACT,OP_MULTIPLY,OP_DIVIDE,OP_NEGATE,OP_POWER,OP_ROOT,
    OP_SIN,OP_COS,OP_TAN,OP_ASIN,OP_ACOS,OP_ATAN,OP_SINH,OP_COSH,OP_TANH,
    OP_LOG,OP_LOGAB,OP_NATURAL_LOG,OP_EXP,OP_SQRT,OP_CUBEROOT,OP_ABS,DT_SUM_CHAIN, DT_MUL_CHAIN,
    OPERATORS_END,

    VAR_BEGIN,
    VAR_L_A,VAR_L_B,VAR_L_C,VAR_L_D,VAR_L_E,VAR_L_F,VAR_L_G,VAR_L_H,VAR_L_I,
    VAR_L_J,VAR_L_K,VAR_L_L,VAR_L_M,VAR_L_N,VAR_L_O,VAR_L_P,VAR_L_Q,VAR_L_R,
    VAR_L_S,VAR_L_T,VAR_L_U,VAR_L_V,VAR_L_W,VAR_L_X,VAR_L_Y,VAR_L_Z,

    VAR_U_A,VAR_U_B,VAR_U_C,VAR_U_D,VAR_U_E,VAR_U_F,VAR_U_G,VAR_U_H,VAR_U_I,
    VAR_U_J,VAR_U_K,VAR_U_L,VAR_U_M,VAR_U_N,VAR_U_O,VAR_U_P,VAR_U_Q,VAR_U_R,
    VAR_U_S,VAR_U_T,VAR_U_U,VAR_U_V,VAR_U_W,VAR_U_X,VAR_U_Y,VAR_U_Z,
    VAR_END,


    ERROR_BEGIN,
    ERROR_INVALID_1_OPERANDS,ERROR_INVALID_2_OPERANDS,ERROR_INVALID_3_OPERANDS,

    ERROR_DIV_BY_ZERO,ERROR_INVALID_EXPRESSION,ERROR_INVALID_MAT_DIMS,ERROR_INVALID_VEC_DIMS,
    ERROR_END
  };

  bool isVar(TokenType type){return (type > VAR_BEGIN && type < VAR_END);}
  bool isOp(TokenType type){return (type > OPERATORS_BEGIN && type < OPERATORS_END);}
  bool isError(TokenType type){return (type > ERROR_BEGIN && type < ERROR_END);}

  bool isVar(Token& token)
  {
    if(std::holds_alternative<TokenType>(token.dataVariant)){return isVar(std::get<TokenType>(token.dataVariant));}
    return false;
  }
  bool isOp(Token& token)
  {
    if(std::holds_alternative<TokenType>(token.dataVariant)){return isOp(std::get<TokenType>(token.dataVariant));}
    return false;
  }
  bool isError(Token& token)
  {
    if(std::holds_alternative<TokenType>(token.dataVariant)){return isError(std::get<TokenType>(token.dataVariant));}
    return false;
  }


  bool isReal(Token& token){return (std::holds_alternative<long double*>(token.dataVariant));}
  bool isComplex(Token& token){return std::holds_alternative<std::complex<Token>*>(token.dataVariant);}
  bool isVector(Token& token){return std::holds_alternative<vecn<Token>*>(token.dataVariant);}
  bool isMatrix(Token& token){return std::holds_alternative<matn<Token>*>(token.dataVariant);}
  bool isExpr(Token& token){return std::holds_alternative<ExpressionTreeNodePtr>(token.dataVariant);}
  bool isData(Token& token){return isReal(token) || isComplex(token) || isVector(token) || isMatrix(token) || isExpr(token);}
  std::ostream& operator<<(std::ostream& os, TokenType type)
  {
    switch (type)
    {
      case OP_ADD:{os<<"+";break;}
      case OP_MULTIPLY:{os<<"*";break;}
      case OP_DIVIDE:{os<<"/";break;}
      case OP_POWER:{os<<"^";break;}
      case DT_SUM_CHAIN:{os<<"+(chain)";break;}
      case DT_MUL_CHAIN:{os<<"*(chain)";break;}
    }
    return os;
  }

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
      matn<T>& operator=(matn<T> other);
      T& at(int index) const;
      T& at(int x, int y) const;
      T* row_at(int y) const;
      void operator+=(const matn<T>& other);
      void operator+=(const vecn<T>& other);
      void operator-=(const matn<T>& other);
      void operator-=(const vecn<T>& other);
      void operator*=(const matn<T>& other);
      void operator*=(const vecn<T>& other);
      void operator*=(const T& other);
      void operator/=(const T& other);
  };

  template<typename T>
  matn<T> identity_matn(int width);

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
      vecn<T>& operator=(vecn<T> other);
      T& at(int index) const;
      void operator+=(const vecn<T>& other);
      void operator+=(const matn<T>& other);
      void operator-=(const vecn<T>& other);
      void operator-=(const matn<T>& other);
      void operator*=(const matn<T>& other);
      void operator*=(const vecn<T>& other);
      void operator*=(const T& other);
      void operator/=(const T& other);

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


  bool isAdd(Token& tree);
  bool isMul(Token& tree);
  bool isDiv(Token& tree);
  bool isPow(Token& tree);
  bool isMulChain(Token& tree);
  bool isAddChain(Token& tree);

  ExpressionTreeNodePtr getPolyTermCoefficient(ExpressionTreeNodePtr tree);
  ExpressionTreeNodePtr getPolyTermPower(ExpressionTreeNodePtr tree);

  bool arithmeticIsValid(ExpressionTreeNodePtr tree);
  void simplifyConstantNumericalValues(ExpressionTreeNodePtr& tree);
  void recursiveArithmeticValidityCheck(ExpressionTreeNodePtr tree, bool& isValid);
  void simpleCollectLikeTerms(ExpressionTreeNodePtr& tree);


  class Token
  {
    public:
      std::variant<std::nullptr_t, long double*, std::complex<Token>*, matn<Token>*, vecn<Token>*, ExpressionTreeNodePtr, InfoLog<2, TokenType>*, TokenType> dataVariant;
      Token();

      ~Token();
      Token(const Token& other);
      Token(Token&& other);
      template<typename T>
      Token(T other);

      template<typename T>
      Token operator=(T other);


      Token& operator=(Token other);

      void operator+=(const Token& b);
      void operator-=(const Token& b);
      void operator*=(const Token& a);
      void operator/=(const Token& a);

      template<typename T>
      T get();
  };

  void applyTokenOperation(Token& token, TokenType op, const Token& other);
  Token getTokenOperationResult(const Token& a, TokenType op, const Token& b);

  
  class ExpressionTreeNode
  {
    public:
      std::vector<ExpressionTreeNodePtr> children;
      Token data;
      ExpressionTreeNode();
      ExpressionTreeNode(const Token& _data);
      ExpressionTreeNode(Token&& _data);
      ExpressionTreeNode(ExpressionTreeNode& other);
      ExpressionTreeNode(ExpressionTreeNode&& other);
      ExpressionTreeNode& operator=(ExpressionTreeNode other);
      ~ExpressionTreeNode();

  };


  struct ExpressionTreeNodePtr
  {
    public:
      ExpressionTreeNode* data;
      ExpressionTreeNodePtr(ExpressionTreeNode* tree) : data(tree)  {}
      ExpressionTreeNodePtr() : data(nullptr)  {}
      operator ExpressionTreeNode*(){return data;}
      ExpressionTreeNode* operator->(){return data;}
      ExpressionTreeNode& operator*(){return *data;}
      void operator delete(void* ptr){delete static_cast<ExpressionTreeNode*>(ptr);}
  };

  bool operator==(const ExpressionTreeNode& a, const ExpressionTreeNode& b);
  bool operator==(ExpressionTreeNodePtr a, ExpressionTreeNodePtr b);
  bool operator==(const Token& a, const Token& b);

  void simplifyRealFraction(ExpressionTreeNodePtr tree);

  void applyBinaryOperation(ExpressionTreeNodePtr& tree, mymath::TokenType op, const mymath::Token& other);
  void applyBinaryOperation(const mymath::Token& other, mymath::TokenType op, ExpressionTreeNodePtr& tree);
  void applyBinaryOperation(ExpressionTreeNodePtr& tree, mymath::TokenType op, ExpressionTreeNodePtr& other);
  void applyUnaryOperation(ExpressionTreeNodePtr& tree, mymath::TokenType op);


  void formatArithmeticChains(ExpressionTreeNodePtr& tree);

  void formatAddChain(ExpressionTreeNodePtr& tree);
  void getAddNodes(ExpressionTreeNodePtr& currentNode,std::vector<ExpressionTreeNodePtr>& currentAddNodes);

  void formatMulDivChain(ExpressionTreeNodePtr& tree, bool aboveIsNumerator = true);
  void getMulDivNodes(ExpressionTreeNodePtr& currentNode,std::vector<ExpressionTreeNodePtr>& currentNumeratorNodes, std::vector<ExpressionTreeNodePtr>& currentDenominatorNodes, bool aboveIsNumerator = true);

  void formatPowNodes(ExpressionTreeNodePtr& tree);
  void getPowNodes(ExpressionTreeNodePtr& currentNode, std::vector<ExpressionTreeNodePtr>& currentPowNodes, ExpressionTreeNodePtr& base);



  mymath::Token operator+(const mymath::Token& a, const mymath::Token& b);
  mymath::Token operator-(const mymath::Token& a, const mymath::Token& b);
  mymath::Token operator-(const mymath::Token& a);
  mymath::Token operator*(const mymath::Token& a, const mymath::Token& b);
  Token operator/(const Token& a, const Token& b);



  template<typename T>
  mymath::vecn<T> operator+(const mymath::vecn<T>& a, const mymath::vecn<T>& b);
  template<typename T>
  mymath::vecn<T> operator-(const mymath::vecn<T>& a, const mymath::vecn<T>& b);
  template<typename T>
  mymath::vecn<T> operator-(const mymath::vecn<T>& a);

  template<typename T>
  mymath::matn<T> operator+(const mymath::matn<T>& a, const mymath::matn<T>& b);
  template<typename T>
  mymath::matn<T> operator-(const mymath::matn<T>& a, const mymath::matn<T>& b);
  template<typename T>
  mymath::matn<T> operator-(const mymath::matn<T>& a);

  template<typename T>
  mymath::matn<T> operator*(const mymath::matn<T>& a, const mymath::matn<T>& b);
  template<typename T>
  mymath::matn<T> gaussianInverse(const mymath::matn<T>& a);

  template<typename T>
  mymath::vecn<T> operator*(const mymath::matn<T>& a, const mymath::vecn<T>& b);
  template<typename T>
  mymath::matn<T> operator+(const mymath::matn<T>& a, const mymath::vecn<T>& b);

  template<typename T>
  matn<T> pow(const matn<T>& base, int exponent);


  template<typename T>
  void swap(mymath::matn<T>& a, mymath::matn<T>& b)
  {
    std::swap(a.width,b.width);
    std::swap(a.height,b.height);
    std::swap(a.data,b.data);
  }
  template<typename T>
  void swap(mymath::vecn<T>& a, mymath::vecn<T>& b)
  {
    std::swap(a.height,b.height);
    std::swap(a.data,b.data);
  }
  void swap(mymath::Token& a, mymath::Token& b)
  {
    std::swap(a.dataVariant, b.dataVariant);
  }
  void swap(mymath::ExpressionTreeNode& a, mymath::ExpressionTreeNode& b)
  {
    swap(a.data,b.data);
    std::swap(a.children,b.children);
  }



  std::ostream& operator<<(std::ostream& os, const Token& token);
  void print(ExpressionTreeNodePtr tree);
  void recursive_print(ExpressionTreeNodePtr tree, int currentIndent);

  typedef std::variant<std::nullptr_t, long double*, std::complex<Token>*,
   matn<Token>*, vecn<Token>*, ExpressionTreeNodePtr, InfoLog<2, TokenType>*, TokenType>
     tokenVariant;

}

// template<>
// inline _GLIBCXX20_CONSTEXPR std::complex<mymath::Token>
// std::operator-(const std::complex<mymath::Token>& __x);


#include"math/matvec/vec.hpp"
#include"math/matvec/mat.hpp"
#include"math/token.hpp"
#include"math/matvec/matvec_operators.hpp"
#include"math/expressiontree/expressiontree.hpp"

#endif