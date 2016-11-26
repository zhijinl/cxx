// ---------------------------------------------------------------------------
// Copyright (c) 2016 by Zhijin Li
//
// main.cc for Expr
//
// Made by Zhijin Li
// Mail:   <jonathan.zj.lee@gmail.com>
//
// Started on  Thu Jun  9 15:17:29 2016 Zhijin Li
// Last update Sat Nov 26 23:46:29 2016 Zhijin Li
// ---------------------------------------------------------------------------


# include <chrono>
# include <iomanip>
# include <random>
# include <iostream>
# include "vec.hh"
# include "expr.hh"
# include "boost/core/demangle.hpp"


int main()
{
  using dtype = float;
  constexpr int size = 1e7;
  
  std::vector<dtype> __vec1(size);
  std::vector<dtype> __vec2(size);
  std::vector<dtype> __vec3(size);

  std::uniform_real_distribution<dtype> __ug;
  std::mt19937 engine
    ( (std::chrono::duration_cast<std::chrono::milliseconds>
       (std::chrono::system_clock::now().time_since_epoch()).count()) );

  for(auto & __elm: __vec1) __elm = __ug(engine);
  for(auto & __elm: __vec2) __elm = __ug(engine);
  for(auto & __elm: __vec3) __elm = __ug(engine);

  Vector<dtype> vec1(std::move(__vec1));
  Vector<dtype> vec2(std::move(__vec2));
  Vector<dtype> vec3(std::move(__vec3));

  unsigned long start_ms_no_ets =
    std::chrono::duration_cast<std::chrono::milliseconds>
    (std::chrono::system_clock::now().time_since_epoch()).count();
  std::cout << "\nNo-ETs evaluation starts.\n";
  
  // Evaluate the expression without ETS.
  Vector<dtype> result_no_ets = (vec1 + (vec2*vec3)) * vec3 +
    vec1 * vec2 + vec3*(vec1+vec2);
  
  unsigned long stop_ms_no_ets =
    std::chrono::duration_cast<std::chrono::milliseconds>
    (std::chrono::system_clock::now().time_since_epoch()).count();
  std::cout << std::setprecision(6) << std::fixed
            << "No-ETs. Time eclapses: " << (stop_ms_no_ets-start_ms_no_ets)/1000.0
            << " s.\n" << std::endl;
  
  unsigned long start_ms_ets =
    std::chrono::duration_cast<std::chrono::milliseconds>
    (std::chrono::system_clock::now().time_since_epoch()).count();
  std::cout << "Evaluation using ETs starts.\n";
  
  expr::terminal<Vector<dtype>> vec4(vec1);
  expr::terminal<Vector<dtype>> vec5(vec2);
  expr::terminal<Vector<dtype>> vec6(vec3);  

  // Evaluate the same expression with ETS.
  Vector<dtype> result_ets = (vec4 + (vec5*vec6)) * vec6 +
    vec4 * vec5 + vec6*(vec4+vec5);
  
  unsigned long stop_ms_ets =
    std::chrono::duration_cast<std::chrono::milliseconds>
    (std::chrono::system_clock::now().time_since_epoch()).count();
  std::cout << std::setprecision(6) << std::fixed
            << "With ETs. Time eclapses: " << (stop_ms_ets-start_ms_ets)/1000.0
            << " s.\n" << std::endl;

  for(const auto &val: result_no_ets())
    assert(val == result_ets[&val-result_no_ets().data()]);
  
  // Using auto with expression will not trigger the evaluation:
  // to obtain the expression tree.
  auto ets_ret_type = (vec4 + (vec5*vec6)) * vec6 +
    vec4 * vec5 + vec6*(vec4+vec5);
  std::cout << "\nETs result's type:\n";
  std::cout << boost::core::demangle( typeid(decltype(ets_ret_type)).name() )
            << '\n'; 

  return 0;
}
