// ---------------------------------------------------------------------------
// Copyright (c) 2016 by Zhijin Li
//
// expr.hh for Expr
//
// Made by Zhijin Li
// Mail:   <jonathan.zj.lee@gmail.com>
//
// Started on  Thu Jun  9 13:53:18 2016 Zhijin Li
// Last update Sat Nov 26 23:45:39 2016 Zhijin Li
// ---------------------------------------------------------------------------


#ifndef EXPR_EXPR_HH
# define EXPR_EXPR_HH


/// Fwd declaration.
template<typename> class Vector;

namespace expr
{

  /// CRTP base class for all expressions.
  template<typename Expr> class expr_base
  {
  public:

    const Expr& self() const { return static_cast<const Expr&>(*this); }
    Expr& self() { return static_cast<Expr&>(*this); }

  protected:

    explicit expr_base() {};
    int size() const { return self().size_impl(); }
    auto operator[](int indx) const { return self().at_impl(indx); }
    auto operator()() const { return self()(); };
  };

  /// Generic wrapper for underlying data structure.
  template<typename DataType> class terminal: expr_base<terminal<DataType>>
  {
  public:

    using base_type = expr_base<terminal<DataType>>;
    using base_type::size;
    using base_type::operator[];
    friend base_type;
    
    explicit terminal(const DataType &val): _val(val) {}

  private:

    int size_impl() const { return _val.size(); };
    auto at_impl(int indx) const { return _val[indx]; };
    decltype(auto) operator()() const { return (_val); }
    
    const DataType &_val;
  };
  
  /// Generic wrapper for binary operations (that are element-wise).
  template<typename Ops, typename lExpr, typename rExpr>
  class binary_ops: public expr_base<binary_ops<Ops,lExpr,rExpr>>
  {
  public:

    using base_type = expr_base<binary_ops<Ops,lExpr,rExpr>>;
    using base_type::size;
    using base_type::operator[];
    friend base_type;
    
    explicit binary_ops(const Ops &ops, const lExpr &lxpr, const rExpr &rxpr)
      : _ops(ops), _lxpr(lxpr), _rxpr(rxpr) {};

    /// Implicit conversion from arbitrary expr to concrete data type.
    /// This forces evaluation of the underlying expression.
    template<typename DataType> operator DataType()
    {
      DataType __vec(size());
      for(int __ind = 0; __ind < __vec.size(); ++__ind)
        __vec[__ind] = (*this)[__ind];
      return __vec;
    }
    
  private:

    int size_impl() const { return _lxpr.size(); };
    auto at_impl(int indx) const { return _ops(_lxpr[indx], _rxpr[indx]); };

    Ops   _ops;
    lExpr _lxpr;
    rExpr _rxpr;
  };
    
  /// Point-wise plus operation.
  struct vec_plus_t
  {
    constexpr explicit vec_plus_t() = default; 
    template<typename LType, typename RType>
    auto operator()(const LType &lhs, const RType &rhs) const
    { return lhs+rhs; }
  };

  /// Point-wise multiplies operation.
  struct vec_prod_t
  {
    constexpr explicit vec_prod_t() = default; 
    template<typename LType, typename RType>
    auto operator()(const LType &lhs, const RType &rhs) const
    { return lhs*rhs; }
  };

  /// Constant plus and multiplies operator objects.
  constexpr vec_plus_t vec_plus{};
  constexpr vec_prod_t vec_prod{};
  
  /// Proxy plus operator.
  template<typename lExpr, typename rExpr>
  auto operator+(const lExpr &lhs, const rExpr &rhs)
  { return binary_ops<vec_plus_t,lExpr,rExpr>(vec_plus,lhs,rhs); }
  
  /// Proxy multiplication operator.
  template<typename lExpr, typename rExpr>
  auto operator*(const lExpr &lhs, const rExpr &rhs)
  { return binary_ops<vec_prod_t,lExpr,rExpr>(vec_prod,lhs,rhs); }
  
} //!expr


#endif //!EXPR_EXPR_HH

