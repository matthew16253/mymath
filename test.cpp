#include<iostream>
#include"fd_decs.hpp"

int main()
{
  using namespace mymath;


  Token a(new std::complex<Token>(Token(new long double(15),DT_REAL),Token(new long double(12),DT_REAL)), DT_COMPLEX);
  Token b(new long double(16.0), DT_REAL);

  Token c = b - a;
  Token d = a - b;
  Token e = -a;

  Token real_part(static_cast<Token>(static_cast<std::complex<Token>*>(c.dataptr)->real()));
  Token imag_part(static_cast<Token>(static_cast<std::complex<Token>*>(c.dataptr)->imag()));
  std::cout<<"result: type should be 25, actual is: "<<c.type<<"\n"<<"data held is: "<<*static_cast<long double*>(real_part.dataptr)<<", "<<*static_cast<long double*>(imag_part.dataptr)<<"i\n";

  real_part = static_cast<Token>(static_cast<std::complex<Token>*>(d.dataptr)->real());
  imag_part = static_cast<Token>(static_cast<std::complex<Token>*>(d.dataptr)->imag());
  std::cout<<"result: type should be 25, actual is: "<<d.type<<"\n"<<"data held is: "<<*static_cast<long double*>(real_part.dataptr)<<", "<<*static_cast<long double*>(imag_part.dataptr)<<"i\n";

  real_part = static_cast<Token>(static_cast<std::complex<Token>*>(e.dataptr)->real());
  imag_part = static_cast<Token>(static_cast<std::complex<Token>*>(e.dataptr)->imag());
  std::cout<<"result: type should be 25, actual is: "<<e.type<<"\n"<<"data held is: "<<*static_cast<long double*>(real_part.dataptr)<<", "<<*static_cast<long double*>(imag_part.dataptr)<<"i\n";

  return 0;
}