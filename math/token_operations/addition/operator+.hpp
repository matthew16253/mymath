#ifndef TOKEN_OPERATOR_PLUS_HPP
#define TOKEN_OPERATOR_PLUS_HPP

#include"../../../fd_decs.hpp"

namespace mymath
{
  mymath::Token operator+(const mymath::Token& a, const mymath::Token& b) // DO NOT USE THE INPUTTED TOKENS
  {
    mymath::Token new_token = a;
    new_token += b;
    return new_token;
  }
}

#endif