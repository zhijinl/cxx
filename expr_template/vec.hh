// ---------------------------------------------------------------------------
// Copyright (c) 2016 by Zhijin Li
//
// vec.hh for Expr
//
// Made by Zhijin Li
// Mail:   <jonathan.zj.lee@gmail.com>
//
// Started on  Thu Jun  9 14:28:24 2016 Zhijin Li
// Last update Sat Nov 26 23:44:39 2016 Zhijin Li
// ---------------------------------------------------------------------------


#ifndef EXPR_VEC_HH
# define EXPR_VEC_HH

# include <vector>
# include <cassert>
# include <utility>
# include <iostream>
# include <algorithm>
# include <functional>


/// std::vector wrapper.
template<typename ScalarType> class Vector
{
public:

  Vector()
  { std::cout << "ctor called.\n"; };

  explicit Vector(int size):
    _vec(size) { std::cout << "ctor called.\n"; };

  explicit Vector(const std::vector<ScalarType> &vec):
    _vec(vec) { std::cout << "ctor called.\n"; };
  
  Vector(const Vector<ScalarType> & vec):
    _vec{vec()} { std::cout << "copy ctor called.\n"; };

  Vector(Vector<ScalarType> && vec):
    _vec(std::move(vec())) { std::cout << "move ctor called.\n"; };

  Vector<ScalarType> & operator=(const Vector<ScalarType> &) = default;
  Vector<ScalarType> & operator=(Vector<ScalarType> &&) = default;

  decltype(auto) operator[](int indx) { return _vec[indx]; }
  decltype(auto) operator[](int indx) const { return _vec[indx]; }

  decltype(auto) operator()() & { return (_vec); };        // return reference.
  decltype(auto) operator()() const & { return (_vec); };  // likewise.
  Vector<ScalarType> && operator()() && { return std::move(*this); }

  int size() const { return _vec.size(); }
  
private:

  std::vector<ScalarType> _vec;
};

/// Addition operator.
template<typename ScalarType>
auto operator+(const Vector<ScalarType> &lhs, const Vector<ScalarType> &rhs)
{
  assert(lhs().size() == rhs().size() &&
         "error: ops plus -> lhs and rhs size mismatch.");
  
  std::vector<ScalarType> __vec;
  __vec.resize(lhs().size());
  std::transform(std::cbegin(lhs()), std::cend(lhs()),
                 std::cbegin(rhs()), std::begin(__vec),
                 std::plus<>());
  
  return Vector<ScalarType>(std::move(__vec));
}

/// Multiplication operator.
template<typename ScalarType>
auto operator*(const Vector<ScalarType> &lhs, const Vector<ScalarType> &rhs)
{
  assert(lhs().size() == rhs().size() &&
         "error: ops multiplies -> lhs and rhs size mismatch.");
  
  std::vector<ScalarType> __vec;
  __vec.resize(lhs().size());
  std::transform(std::cbegin(lhs()), std::cend(lhs()),
                 std::cbegin(rhs()), std::begin(__vec),
                 std::multiplies<>());

  return Vector<ScalarType>(std::move(__vec));
}


#endif //!EXPR_VEC_HH
