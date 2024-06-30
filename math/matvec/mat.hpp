#ifndef MAT_HPP
#define MAT_HPP

#include<algorithm>
#include<iostream>


#include"../../fd_decs.hpp"

namespace mymath
{
  template<typename T>
  matn<T>::matn(int _width, int _height) : width(_width), height(_height), data(new T[_width*_height])  {}

  template<typename T>
  matn<T>::matn(int _width, int _height, T* newdata) : width(_width), height(_height), data(new T[_width*_height])
  {
    std::copy(newdata, newdata+_width*_height, data);
  }


  template<typename T>
  matn<T>::matn(const matn<T>& other) : width(other.width), height(other.height), data(new T[width*height])
  {
    std::copy(other.data, other.data+other.width+other.height, data);
  }
  template<typename T>
  matn<T>::matn(matn<T>&& other) : width(other.width), height(other.height), data(other.data)
  {
    other.data = nullptr;
  }
  template<typename T>
  matn<T>::~matn()
  {
    delete[] data;
  }
  template<typename T>
  matn<T>& matn<T>::operator=(matn<T> other)
  {
    swap(*this,other);
    return *this;
  }
  template<typename T>
  T& matn<T>::at(int index) const
  {
    return data[index];
  }
  template<typename T>
  T& matn<T>::at(int x, int y) const
  {
    return data[y*width + x];
  }
  template<typename T>
  T* matn<T>::row_at(int y) const
  {
    return data + y*width;
  }

  template<typename T>
  matn<T> identity_matn(int width)
  {
    matn<T> new_matn;
    for(int index = 0; index < width; index++)
    {
      new_matn.at(index,index) = 1;
    }
  }
}

#endif