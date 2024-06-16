#ifndef TOKEN_OPERATOR_MULTIPLY_HPP
#define TOKEN_OPERATOR_MULTIPLY_HPP

#include"../../../fd_decs.hpp"

namespace mymath
{
  Token operator*(const Token& a, const Token& b)
  {
    Token new_token = a;
    new_token *= b;
    return new_token;
  }
}

#endif