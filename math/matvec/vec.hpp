#ifndef VEC_HPP
#define VEC_HPP

#include<algorithm>
#include<iostream>

#include"../../fd_decs.hpp"

namespace mymath
{
  template<typename T>
  vecn<T>::vecn(int _height) : height(_height), data(new T[height])  {}
  template<typename T>
  vecn<T>::vecn(int _height, T* newdata) : height(_height), data(new T[height])
  {
    std::copy(newdata, newdata+height, data);
  }
  template<typename T>
  vecn<T>::vecn(const vecn<T>& other) : height(other.height), data(new T[height])
  {
    std::copy(other.data, other.data+other.height, data);
  }
  template<typename T>
  vecn<T>::vecn(vecn<T>&& other) : height(other.height), data(other.data)
  {
    other.data = nullptr;
  }
  template<typename T>
  vecn<T>::~vecn()
  {
    delete[] data;
  }
  template<typename T>
  vecn<T>& vecn<T>::operator=(vecn<T> other)
  {
    swap(*this,other);
    return *this;
  }
  template<typename T>
  T& vecn<T>::at(int index) const
  {
    return data[index];
  }
}


#endif