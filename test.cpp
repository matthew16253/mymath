#include<iostream>
#include"fd_decs.hpp"

int main()
{
  using namespace mymath;


  Token a(new long double(1.0), DT_REAL);
  Token b(new long double(2.0), DT_REAL);

  Token c = a + b;
  std::cout<<"type should be 24, actual is: "<<c.type<<"\n"<<"data held is: "<<*static_cast<long double*>(c.dataptr)<<"\n";


  return 0;
}