#include<iostream>
#include<string>
#include<variant>
#include<type_traits>
#include<vector>
#include<boost/multiprecision/cpp_dec_float.hpp>
#include<boost/math/policies/error_handling.hpp>
#include"fd_decs.hpp"

int main()
{
  //using namespace mymath;
  // Token r_token = dec_float("6");
  // Token real = dec_float("12.4");
  // Token imag = dec_float("1");
  // Token c_token = std::complex<Token>(real, imag);
  // std::cout<<r_token<<"\n";
  // std::cout<<c_token<<"\n";
  // std::cout<<r_token*c_token<<"\n";

  using dec = boost::multiprecision::number<boost::multiprecision::cpp_dec_float<100>>;

  try
  {
    dec a("1e-67108860");
    dec b("1e100");
    std::cout<< std::numeric_limits<dec>::max()<<"\n";
    std::cout<<a * b<<"\n";
  }
  catch(const std::exception& e)
  {
    std::cout << "EXCEPTION: \n " <<e.what();
  }


  //delete frac;
  return 0;
}