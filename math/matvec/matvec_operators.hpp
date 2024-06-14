#ifndef MATVECFUNCTION_HPP
#define MATVECFUNCTION_HPP

#include"../../fd_decs.hpp"

#include<iostream>


//
//
//
// VEC

template<typename T>
mymath::vecn<T> operator+(const mymath::vecn<T>& a, const mymath::vecn<T>& b)
{
  mymath::vecn<T> c(a.height);
  for(int i = 0; i < a.height; i++)
  {
    c.data[i] = a.data[i] + b.data[i];
  }
  return c;
}
template<typename T>
mymath::vecn<T> operator-(const mymath::vecn<T>& a, const mymath::vecn<T>& b)
{
  mymath::vecn<T> c(a.height);
  for(int i = 0; i < a.height; i++)
  {
    c.data[i] = a.data[i] - b.data[i];
  }
  return c;
}

template<typename T>
void print(const mymath::vecn<T>& a)
{
  std::cout << "[";
  for(int y = 0; y < a.height; y++)
  {
    std::cout << a.at(y);
    std::cout << "\n";
  }
}

template<typename T>
T dot(const mymath::vecn<T>& a, const mymath::vecn<T>& b)
{
  T sum = T();
  mymath::vecn<T> c(a.height);
  for(int i = 0; i < a.height; i++)
  {
    sum = sum + a.at(i) * b.at(i);
  }
  return sum;
}

template<typename T>
mymath::vecn<T> cross(const mymath::vecn<T>& a, const mymath::vecn<T>& b)
{
  T vals = 
  {
    a.at(2)*b.at(3) + a.at(3)*b.at(2),
    a.at(3)*b.at(1) + a.at(1)*b.at(3),
    a.at(1)*b.at(2) + a.at(2)*b.at(1)
  };
  mymath::vecn<T> c(a.height,vals);
  return c;
}

template<typename T>
T magnitude(const mymath::vecn<T>& a)
{
  T sum = T();
  for(int i = 0; i < a.height; i++)
  {
    sum = sum + a.at(i) * a.at(i);
  }
  return sqrt(sum);
}


//
//
//
//
// MAT

template<typename T>
mymath::matn<T> operator+(const mymath::matn<T>& a, const mymath::matn<T>& b)
{
  mymath::matn<T> c(a.width, a.height);
  for(int i = 0; i < a.width*a.height; i++)
  {
    c.data[i] = a.data[i] + b.data[i];
  }
  return c;
}

template<typename T>
mymath::matn<T> operator+(const mymath::matn<T>& a, const mymath::vecn<T>& b)
{
  mymath::matn<T> c(1,a.height);
  for(int i = 0; i < a.height; i++)
  {
    c.data[i] = a.data[i] + b.data[i];
  }
  return c;
}

template<typename T>
mymath::matn<T> operator-(const mymath::matn<T>& a, const mymath::matn<T>& b)
{
  mymath::matn<T> c(a.width, a.height);
  for(int i = 0; i < a.width*a.height; i++)
  {
    c.data[i] = a.data[i] - b.data[i];
  }
  return c;
}

template<typename T>
mymath::matn<T> operator*(const mymath::matn<T>& a, const mymath::matn<T>& b)
{
  mymath::matn<T> c(a.height, b.width);
  for(int cy = 0; cy < a.height; cy++)
  {
    for(int cx = 0; cx < b.width; cx++)
    {
      T sum = T();
      int aindex = cy * a.width;
      int bindex = cx;
      for(int i = 0; i < a.width; i++)
      {
        sum = sum + a.data[aindex] * b.data[bindex];
        aindex++;
        bindex += b.width;
      }
      c.at(cx,cy) = sum;
    }
  }
  return c;
}

template<typename T>
mymath::vecn<T> operator*(const mymath::matn<T>& a, const mymath::vecn<T>& b)
{
  mymath::vecn<T> c(a.height);
  for(int cy = 0; cy < a.height; cy++)
  {
    T sum = T();
    int aindex = cy * a.width;
    int bindex = 0;
    for(int i = 0; i < a.width; i++)
    {
      sum = sum + a.data[aindex] * b.data[bindex];
      aindex++;
      bindex++;
    }
    c.at(cy) = sum;
  }
  return c;
}

template<typename T>
mymath::matn<T> gaussianInverse(const mymath::matn<T>& a)
{
  mymath::matn<T> augmented_mat(a.width*2, a.height);
  for(int row = 0; row < augmented_mat.height; row++)
  {
    std::copy(a.row_at(row), a.row_at(row) + a.width, augmented_mat.row_at(row));
    augmented_mat.at(a.width + row, row) = 1;
  }


  for(int workingRow = 0; workingRow < augmented_mat.height; workingRow++)
  {
    T* workingRowPtr = augmented_mat.row_at(workingRow);
    // divide augmented_mat's current row by autgmented_mat.at(row,row)
    T divisor =  augmented_mat.at(workingRow,workingRow);
    for(T* it = workingRowPtr; it != augmented_mat.row_at(workingRow+1); it++)
    {
      *it /= divisor;
    }
    // take away (workingRow*aug_mat.at(row,workingRow)
    for(int row = 0; row < augmented_mat.height; row++)  // row helps to loop over the subtraction rows
    {
      if(row != workingRow)
      {
        T multiplier = augmented_mat.at(row,workingRow);
        T* currentRowPtr = augmented_mat.row_at(row);
        for(int col = 0; col < augmented_mat.height; col++)
        {
          *(currentRowPtr + col) -= *(workingRowPtr + col) * multiplier;
        }
      }
    }
  }
  return augmented_mat;
}

template<typename T>
void print(const mymath::matn<T>& a)
{
  std::cout << "[";
  for(int y = 0; y < a.height; y++)
  {
    for(int x = 0; x < a.width; x++)
    {
      std::cout << a.at(x,y) << ",";
    }
    std::cout << "\n";
  }
}


#endif