#ifndef OPERATOR_FD_SLASH_HPP
#define OPERATOR_FD_SLASH_HPP

#include"../../../fd_decs.hpp"

namespace mymath
{
  Token operator/(const Token& a, const Token& b)
  {
    Token a_copy(a);
    a_copy /= b;
  }
}

#endif