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
#include<linear_map.hpp>


namespace mymath
{
  typedef boost::multiprecision::cpp_dec_float_100 dec_float;
  bool isOverflow(const dec_float& num);
  bool isUnderflow(const dec_float& num);
  bool isNan(const dec_float& num);
  bool isDomainError(const dec_float& num);
  bool isEqual(const dec_float& a, const dec_float& b, const dec_float& epsilon);

  template<typename T>
  concept is_pointer = std::is_pointer_v<T>();

  template<typename T>
  concept is_null_pointer = std::is_null_pointer_v<T>();

  template<typename T>
  bool operator!=(T a, T b)
  {
    return !(a==b);
  }

  class ExpressionTreeNode;
  struct ExpressionTreeNodePtr;
  class Token;
  template<typename T>
  class complex;
  
  template<typename T>
  class vecn;
  
  template<typename T>
  class matn;

  class Variable;
  
  enum TokenType
  {
    NONE,
    
    OPERATORS_BEGIN,
    OP_ADD,OP_SUBTRACT,OP_MULTIPLY,OP_DIVIDE,OP_NEGATE,OP_UNARY_PLUS,OP_POWER,OP_ROOT,
    OP_SIN,OP_COS,OP_TAN,OP_ASIN,OP_ACOS,OP_ATAN,OP_SINH,OP_COSH,OP_TANH,
    OP_LOG,OP_LOGAB,OP_NATURAL_LOG,OP_EXP,OP_SQRT,OP_CUBEROOT,OP_ABS,DT_SUM_CHAIN, DT_MUL_CHAIN,
    OPERATORS_END,

    ERROR_BEGIN,
    ERROR_INVALID_POW_OPERANDS,

    ERROR_DIV_BY_ZERO,ERROR_INVALID_EXPRESSION,ERROR_INVALID_MAT_DIMS,ERROR_INVALID_VEC_DIMS,
    ERROR_END,ERROR_INVALID_DIV_OPERANDS,ERROR_INVALID_MUL_OPERANDS,ERROR_INVALID_ADD_OPERANDS,
    ERROR_INVALID_SUB_OPERANDS,ERROR_INVALID_NEGATE_OPERANDS,

    ERROR_DIV_OVERFLOW,ERROR_MUL_OVERFLOW,ERROR_ADD_OVERFLOW,ERROR_SUB_OVERFLOW,
    ERROR_DIV_UNDERFLOW,ERROR_MUL_UNDERFLOW,ERROR_ADD_UNDERFLOW,ERROR_SUB_UNDERFLOW,
    ERROR_INVALID_DOMAIN,

    DT_UNSPECIFIED,DT_REAL,DT_COMPLEX,DT_VEC,DT_MAT,DT_EXPR,

    DT_NOT_REAL,DT_NOT_REAL_COMPLEX,DT_NOT_DATA,

    OP_ASSIGN,

    IDENTIFIER,LEFT_PARENTHESIS,RIGHT_PARENTHESIS,COMMA,
  };

  template<typename T>
  constexpr TokenType getTokenType();

  class syntaxError : public std::exception
  {
    public:
      std::string what()
      {
        return "syntax error";
      }
  };

  bool isOp(TokenType type){return (type > OPERATORS_BEGIN && type < OPERATORS_END);}
  bool isError(TokenType type){return (type > ERROR_BEGIN && type < ERROR_END);}


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

  class Variable : public std::string
  {
    public:
      using std::string::string;
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
      static constexpr int width = 1;
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
      TokenType error;
      T* ops;
      InfoLog(TokenType _error, T* _ops) : error(_error)
      {
        std::copy(_ops, _ops + size, ops);
      }
      InfoLog(TokenType _error, std::initializer_list<T> _ops) : error(_error)
      {
        std::copy(_ops.begin(), _ops.end(), ops);
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
  bool isDiv(ExpressionTreeNodePtr tree);
  bool isPow(ExpressionTreeNodePtr tree);
  bool isMulChain(ExpressionTreeNodePtr tree);
  bool isAddChain(ExpressionTreeNodePtr tree);
  //bool isCommutativeOperator(ExpressionTreeNodePtr tree);

  ExpressionTreeNodePtr getPolyTermCoefficient(ExpressionTreeNodePtr tree);
  ExpressionTreeNodePtr getPolyTermPower(ExpressionTreeNodePtr tree);

  void collectLikeTerms(ExpressionTreeNodePtr& tree);
  template<TokenType Ignore = NONE>
  void trySimplifyConstants(ExpressionTreeNodePtr& tree);
  void trySimplifyConstantsInPowNode(ExpressionTreeNodePtr& powNode);
  void trySimplifyConstantsInDivNode(ExpressionTreeNodePtr& divNode);
  template<bool shouldAllowMatricesAndVectors = true>
  void trySimplifyConstantsInMulNode(ExpressionTreeNodePtr& mulNode);
  void trySimplifyConstantsInAddNode(ExpressionTreeNodePtr& addNode);

  bool evaluatesToMatrix(ExpressionTreeNodePtr tree);

  bool isRealFraction(ExpressionTreeNodePtr);
  bool arithmeticIsValid(ExpressionTreeNodePtr tree);
  void simplifyConstantNumericalValues(ExpressionTreeNodePtr& tree);
  void simpleCollectLikeTerms(ExpressionTreeNodePtr& tree);

  struct ExpressionTreeNodePtr
  {
    public:
      ExpressionTreeNode* data;
      ExpressionTreeNodePtr(ExpressionTreeNode* tree) : data(tree)  {}
      ExpressionTreeNodePtr() : data(nullptr)  {}
      operator ExpressionTreeNode*(){return data;}
      ExpressionTreeNode* operator->(){return data;}
      ExpressionTreeNode& operator*(){return *data;}
      void operator delete(void* ptr);

      void operator+=(const ExpressionTreeNodePtr other);
      void operator+=(const Token& other);

      void operator-=(const ExpressionTreeNodePtr other);
      void operator-=(const Token& other);

      void operator*=(const ExpressionTreeNodePtr other);
      void operator*=(const Token& other);

      void operator/=(const ExpressionTreeNodePtr other);
      void operator/=(const Token& other);
  };

  bool operator<(const ExpressionTreeNodePtr a, const ExpressionTreeNodePtr b){throw std::runtime_error("do not use binary map search with expr");return true;}
  bool operator>(const ExpressionTreeNodePtr a, const ExpressionTreeNodePtr b){throw std::runtime_error("do not use binary map search with expr");return true;}

  template<typename T>
  class complex
  {
    private:
      T m_real, m_imag;
    public:
      T& real();
      T& imag();
  };


  class Token
  {
    public:
      std::variant<std::nullptr_t, dec_float*, std::complex<Token>*, matn<Token>*, vecn<Token>*, ExpressionTreeNodePtr, InfoLog<2, int>*, InfoLog<2,MatDimension>*, TokenType, Variable> dataVariant;
      Token();

      ~Token();
      Token(const Token& other);
      Token(Token&& other);

      template<typename T>
      requires (!std::is_same_v<std::remove_reference_t<std::remove_cv_t<T>>, Token>)
      explicit Token(const T& other);

      template<typename T>
      requires (!std::is_same_v<std::remove_reference_t<std::remove_cv_t<T>>, Token>)
      Token& operator=(const T& other);

      template<typename T>
      requires (!std::is_same_v<std::remove_reference_t<std::remove_cv_t<T>>, Token>)
      Token& operator=(T&& other);


      Token& operator=(Token other);

      void operator+=(const Token& b);
      void operator-=(const Token& b);
      void operator*=(const Token& a);
      void operator/=(const Token& a);

      template<typename T>
      const T& get() const;
      template<typename T>
      bool is_type();
  };



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

  std::vector<ExpressionTreeNodePtr> copyExpressionTreeNodePtrChildren(ExpressionTreeNodePtr ptr);

  void ExpressionTreeNodePtr::operator delete(void* ptr){delete static_cast<ExpressionTreeNode*>(ptr);}

  bool isVar(const Token& token)
  {
    if(std::holds_alternative<Variable>(token.dataVariant)){return true;}
    else{return false;}
  }
  bool isOp(const Token& token)
  {
    if(std::holds_alternative<TokenType>(token.dataVariant)){return isOp(std::get<TokenType>(token.dataVariant));}
    return false;
  }
  bool isError(const Token& token)
  {
    if(std::holds_alternative<InfoLog<2, int>>(token.dataVariant) || std::holds_alternative<InfoLog<2, MatDimension>>(token.dataVariant)){return true;}
    else if(std::holds_alternative<TokenType>(token.dataVariant)){return isError(std::get<TokenType>(token.dataVariant));}
    return false;
  }


  bool isReal(const Token& token){return (std::holds_alternative<dec_float*>(token.dataVariant));}
  bool isComplex(const Token& token){return std::holds_alternative<std::complex<Token>*>(token.dataVariant);}
  bool isVector(const Token& token){return std::holds_alternative<vecn<Token>*>(token.dataVariant);}
  bool isMatrix(const Token& token){return std::holds_alternative<matn<Token>*>(token.dataVariant);}
  bool isExpr(const Token& token){return std::holds_alternative<ExpressionTreeNodePtr>(token.dataVariant);}
  bool isData(const Token& token){return isReal(token) || isComplex(token) || isVector(token) || isMatrix(token) || isExpr(token);}
  bool isData_notExpr(const Token& token){return isReal(token) || isComplex(token) || isVector(token) || isMatrix(token);}

  void applyTokenOperation(Token& token, TokenType op, const Token& other);
  Token getTokenOperationResult(const Token& a, TokenType op, const Token& b);

  ExpressionTreeNodePtr operator+(const ExpressionTreeNodePtr tree, const Token& token);
  ExpressionTreeNodePtr operator+(const Token& token, const ExpressionTreeNodePtr tree);
  ExpressionTreeNodePtr operator+(const ExpressionTreeNodePtr tree, const ExpressionTreeNodePtr other);

  ExpressionTreeNodePtr operator-(const ExpressionTreeNodePtr tree, const Token& token);
  ExpressionTreeNodePtr operator-(const Token& token, const ExpressionTreeNodePtr tree);
  ExpressionTreeNodePtr operator-(const ExpressionTreeNodePtr tree, const ExpressionTreeNodePtr other);
  ExpressionTreeNodePtr operator-(const ExpressionTreeNodePtr other);

  ExpressionTreeNodePtr operator*(const ExpressionTreeNodePtr tree, const Token& token);
  ExpressionTreeNodePtr operator*(const Token& token, const ExpressionTreeNodePtr tree);
  ExpressionTreeNodePtr operator*(const ExpressionTreeNodePtr tree, const ExpressionTreeNodePtr other);

  ExpressionTreeNodePtr operator/(const ExpressionTreeNodePtr a, const ExpressionTreeNodePtr b);
  ExpressionTreeNodePtr operator/(const ExpressionTreeNodePtr a, const Token& b);
  ExpressionTreeNodePtr operator/(const Token& a, const ExpressionTreeNodePtr b);

  bool operator==(const ExpressionTreeNode& a, const ExpressionTreeNode& b);
  bool operator==(const ExpressionTreeNodePtr a, const ExpressionTreeNodePtr b);
  bool operator==(const Token& a, const Token& b);

  void simplifyRealFraction(ExpressionTreeNodePtr tree);

  void applyBinaryOperation(ExpressionTreeNodePtr& tree, mymath::TokenType op, const mymath::Token& other);
  //void applyBinaryOperation(const mymath::Token& other, mymath::TokenType op, ExpressionTreeNodePtr& tree);
  void applyBinaryOperation(ExpressionTreeNodePtr& tree, mymath::TokenType op, ExpressionTreeNodePtr other);
  void applyUnaryOperation(ExpressionTreeNodePtr& tree, mymath::TokenType op);


  void formatArithmeticChains(ExpressionTreeNodePtr& tree);

  void formatAddChain(ExpressionTreeNodePtr& tree);
  void getAddNodes(ExpressionTreeNodePtr& currentNode,std::vector<ExpressionTreeNodePtr>& currentAddNodes);

  void formatMulDivChain(ExpressionTreeNodePtr& tree, bool aboveIsNumerator = true);
  void getMulDivNodes(ExpressionTreeNodePtr& currentNode,std::vector<ExpressionTreeNodePtr>& currentNumeratorNodes, std::vector<ExpressionTreeNodePtr>& currentDenominatorNodes, bool aboveIsNumerator = true);

  void formatPowNodes(ExpressionTreeNodePtr& tree);
  void getPowNodes(ExpressionTreeNodePtr& currentNode, std::vector<ExpressionTreeNodePtr>& currentPowNodes, ExpressionTreeNodePtr& base);



  Token operator+(const mymath::Token& a, const mymath::Token& b);
  Token operator-(const mymath::Token& a, const mymath::Token& b);
  Token operator-(const mymath::Token& a);
  Token operator*(const mymath::Token& a, const mymath::Token& b);
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
  vecn<T> cross(const vecn<T>&, const vecn<T>&);

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
  std::ostream& operator<<(std::ostream& os, ExpressionTreeNodePtr tree);
  template<typename T>
  std::ostream& operator<<(std::ostream& os, const vecn<T>&);
  template<typename T>
  std::ostream& operator<<(std::ostream& os, const matn<T>&);
  void print(std::ostream& os, ExpressionTreeNodePtr tree);
  void recursive_print(std::ostream& os, ExpressionTreeNodePtr tree, int currentIndent);

  typedef std::variant<std::nullptr_t, dec_float*, std::complex<Token>*,
   matn<Token>*, vecn<Token>*, ExpressionTreeNodePtr, InfoLog<2, int>*, InfoLog<1, int>*, InfoLog<2, MatDimension>*, TokenType, Variable>
     tokenVariant;

  template<typename T>
  struct tag{};

  typedef std::variant<tag<std::nullptr_t>, tag<dec_float*>, tag<std::complex<Token>*>,
   tag<matn<Token>*>, tag<vecn<Token>*>, tag<ExpressionTreeNodePtr>, tag<InfoLog<2, int>*>, tag<InfoLog<2, int>*>,  tag<InfoLog<2, MatDimension>*>, tag<TokenType>, tag<Variable>>
     tokenTagVariant;
  
  template<typename T>
  struct IsTokenVariantType
  {
    static constexpr bool value = std::is_same_v<T, std::nullptr_t> || std::is_same_v<T, dec_float*> ||
                                  std::is_same_v<T, std::complex<Token>*> || std::is_same_v<T, matn<Token>*> ||
                                  std::is_same_v<T, vecn<Token>*> || std::is_same_v<T, ExpressionTreeNodePtr> ||
                                  std::is_same_v<T, InfoLog<2, int>*> || std::is_same_v<T, InfoLog<2, MatDimension>*> ||
                                  std::is_same_v<T, InfoLog<1, int>> || std::is_same_v<T, TokenType> ||
                                  std::is_same_v<T, Variable>;
  };

  template<typename T>
  constexpr bool IsTokenVariantType_v = IsTokenVariantType<T>::value;

  template<typename T>
  concept IsTokenVariantType_c = IsTokenVariantType_v<T>;

  template<typename T>
  struct IsTokenVariantTypeWithoutPointer
  {
    static constexpr bool value = std::is_same_v<T, std::nullptr_t> || std::is_same_v<T, dec_float> ||
                           std::is_same_v<T, std::complex<Token>> || std::is_same_v<T, matn<Token>> ||
                           std::is_same_v<T, vecn<Token>> || std::is_same_v<T, ExpressionTreeNode> ||
                           std::is_same_v<T, InfoLog<2, int>> || std::is_same_v<T, InfoLog<2, MatDimension>> ||
                           std::is_same_v<T, InfoLog<1, int>> || std::is_same_v<T, TokenType> ||
                           std::is_same_v<T, Variable>;
  };

  template<typename T>
  constexpr bool IsTokenVariantTypeWithoutPointer_v = IsTokenVariantType<T>::value;

  template<typename T>
  concept IsTokenVariantTypeWithoutPointer_c = IsTokenVariantTypeWithoutPointer_v<T>;


  template<typename T>
  requires IsTokenVariantTypeWithoutPointer_c<T>
  struct convertToTokenVariantPointer
  {
    using type = std::conditional_t<std::is_same_v<T, ExpressionTreeNode>, ExpressionTreeNode, T*>;
  };
  template<typename T>
  using convertToTokenVariantPointer_t = typename convertToTokenVariantPointer<T>::type;

  template<typename T>
  struct isInfoLog
  {
    static constexpr bool value = (std::is_same_v<std::remove_pointer_t<T>, InfoLog<2, int>>) || (std::is_same_v<std::remove_pointer_t<T>, InfoLog<2, MatDimension>>) || (std::is_same_v<std::remove_pointer_t<T>, InfoLog<1, int>>);
  };
  template<typename T>
  constexpr bool isInfoLog_v = isInfoLog<T>::value;


  const dec_float div_epsilon("1e+67108000");
}



#include"math/matvec/vec.hpp"
#include"math/matvec/mat.hpp"
#include"math/token.hpp"
#include"math/matvec/matvec_operators.hpp"
#include"math/expressiontree/expressiontree.hpp"
#include"math/dec_float_utilities.hpp"













namespace myparse
{
  using namespace mymath;

  class Parser
  {
    private:
      const std::string& code;
      std::vector<Token> tokens;
      int cursor;

      Token generateTokens(std::vector<Token>& target);
      std::tuple<ExpressionTreeNodePtr, bool> expression();
      std::tuple<ExpressionTreeNodePtr, bool> deletion();
      std::tuple<ExpressionTreeNodePtr, bool> assignment();
      std::tuple<ExpressionTreeNodePtr, bool> term();
      std::tuple<ExpressionTreeNodePtr, bool> factor();
      std::tuple<ExpressionTreeNodePtr, bool> unary();
      std::tuple<ExpressionTreeNodePtr, bool> power();
      std::tuple<ExpressionTreeNodePtr, bool> primary();

      std::tuple<ExpressionTreeNodePtr, bool> functionDeclaration();
      std::tuple<ExpressionTreeNodePtr, bool> functionCall();
      Variable variable();
      Variable existingConstant();
      Variable newConstant();
      std::tuple<ExpressionTreeNodePtr, bool> bracketedTerm();

      Token& currentToken();
      Token& previousToken();
      Token& beforePreviousToken();
      void advance();
      void stepBack();
      template<TokenType...>
      bool match();
      bool matchIdentifier();
      bool matchData();
      bool check(TokenType type);
      bool checkIdentifier();
      bool checkData();
      bool isAtEnd();
      

    public:
      Parser(const std::string& _code) : code(_code), cursor(0), tokens()  {}
      std::tuple<ExpressionTreeNodePtr, bool> parse();
  };



  mr::linear_map<Variable, ExpressionTreeNodePtr> constantMap;
  mr::linear_map<Variable, ExpressionTreeNodePtr> functionMap;
}

#endif