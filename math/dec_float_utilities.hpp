#ifndef DEC_FLOAT_UTILITIES_HPP
#define DEC_FLOAT_UTILITIES_HPP

#include"../fd_decs.hpp"

namespace mymath
{
  const dec_float underflowThreshold("1e-67108800");

  bool isOverflow(const dec_float& num)
  {
    return num.backend().isinf();
  }
  bool isUnderflow(const dec_float& num)
  {
    return abs(num) < underflowThreshold; 
  }
  bool isNan(const dec_float& num)
  {
    return num.backend().isnan();
  }
  bool isDomainError(const dec_float& num)
  {
    return isNan(num);
  }

  bool isEqual(const dec_float& a, const dec_float& b, const dec_float& epsilon)
  {
    return abs(a-b) < epsilon;
  }
}

#endif