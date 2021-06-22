#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <valarray>
#include <iomanip>
#include <initializer_list>
#include <type_traits>
#include <algorithm>
#include "Matrix.h"

namespace atlatec_test
{

template< typename T>
requires number<T>
class Vector
{
public:
    using value_type = T;
    using container_type = std::valarray<value_type>;


    explicit Vector();
    explicit Vector(size_t s);
    explicit Vector(const std::valarray<T>& v);
    Vector(std::initializer_list<T> l);
    ~Vector() = default;
    Vector(const Vector&) = default;
    Vector& operator=(const Vector&) = default;
    Vector(Vector&&) = default;
    Vector& operator=(Vector&&) = default;

    size_t size() const noexcept;
    void print() const;
    const std::valarray<value_type>& underlying_valarray() const noexcept;

    value_type& operator[](size_t);
    const value_type& operator[](size_t) const;

    value_type& at (size_t);
    const value_type& at (size_t) const;

    void push_back (const value_type&);
    void push_front (const value_type&);
    void pop_back ();
    void pop_front ();

private:
    size_t _size;
    container_type data;
};

class wrong_operand: public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

template< typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& vec)
{
    os<<std::endl;
    for(size_t i = 0 ; i < vec.size(); i++)
    {
        os<< std::setw(10) <<vec.underlying_valarray()[i];
    }
    os<<std::endl;
    return os;
}

template< typename T>
Vector<T>::Vector():_size{0}, data{} {}

template< typename T>
Vector<T>::Vector(size_t s):_size{s}, data(_size) {}

template< typename T>
Vector<T>::Vector(const std::valarray<T>& v):_size{v.size()}, data{v} {}

template< typename T>
Vector<T>::Vector(std::initializer_list<T> l):_size{l.size()}, data(l) {}

template< typename T>
size_t Vector<T>::size() const noexcept
{
    return _size;
}

template< typename T>
void Vector<T>::print() const
{
    for(size_t i = 0 ; i < _size; i++)
    {
        std::cout<< std::setw(10) <<data[i];
    }
}

template< typename T>
Vector<T>::value_type& Vector<T>::operator[](size_t i)
{
    return data[i];
}

template< typename T>
const Vector<T>::value_type& Vector<T>::operator[](size_t i) const
{
    return data[i];
}

template< typename T>
Vector<T>::value_type& Vector<T>::at (size_t n)
{
    if(n >= _size)
    {
        throw std::out_of_range{"wrong index."};
    }
    return data[n];
}

template< typename T>
const Vector<T>::value_type& Vector<T>::at (size_t n) const
{
    if(n >= _size)
    {
        throw std::out_of_range{"wrong index."};
    }
    return data[n];
}

template< typename T>
void Vector<T>::push_back (const Vector<T>::value_type& v)
{
    container_type tmp(_size+1);
    std::copy( std::begin(data), std::end(data), std::begin(tmp));
    *(std::end(tmp)-1) = v;
    std::swap(tmp, data);
    _size++;
}

template< typename T>
void Vector<T>::push_front (const Vector<T>::value_type& v)
{
    container_type tmp(_size+1);
    std::copy( std::begin(data), std::end(data), std::begin(tmp)+1);
    *(std::begin(tmp)) = v;
    std::swap(tmp, data);
    _size++;
}

template< typename T>
void Vector<T>::pop_back ()
{
    if(_size == 0)
    {
        return;
    }
    container_type tmp(_size-1);
    std::copy( std::begin(data), std::end(data)-1, std::begin(tmp));
    std::swap(tmp, data);
    _size--;
}

template< typename T>
void Vector<T>::pop_front ()
{
    if(_size == 0)
    {
        return;
    }
    container_type tmp(_size-1);
    std::copy( std::begin(data)+1, std::end(data), std::begin(tmp));
    std::swap(tmp, data);
    _size--;
}

template< typename T>
const std::valarray<T>& Vector<T>::underlying_valarray() const noexcept
{
    return data;
}

template<typename T, typename U>
requires std::same_as<T,U>
bool operator==( const Vector<T>& r, const Vector<U>& l )
{
    if constexpr( !std::is_floating_point_v< std::common_type_t<T,U>> )
    {
        const std::valarray<bool>& res = r.underlying_valarray()==l.underlying_valarray();
        return std::all_of( std::begin(res), std::end(res), [](const auto& b)
        {
            return b == true;
        } );
    }
    else
    {
        const double epsilon = 0.00001;
        const std::valarray<std::common_type_t<T,U>>& res = std::abs(r.underlying_valarray()-l.underlying_valarray());
        return std::all_of( std::begin(res), std::end(res), [epsilon](const auto& v)
        {
            return v <= epsilon;
        } );
    }
}

template<typename T, typename U>
requires number<T> && std::same_as<T,U>
auto operator*( T sc, const Vector<U>& r )
{
    return Vector<std::common_type_t<T,U>> {sc*r.underlying_valarray()};
}

template<typename T, typename U>
requires number<T> && std::same_as<T,U>
auto operator*( const Vector<T>& r, U sc )
{
    return sc * r;
}

template<size_t m, size_t n, typename T, typename U>
requires number<T> && std::same_as<T,U>
auto operator*( const Matrix<m, n, T>& l_m, const Vector<U>& r_v)
{
    if( l_m.cols != r_v.size() )
    {
        throw wrong_operand{"operands are inconsistent."};
    }
    Vector<std::common_type_t<T,U>> res(l_m.rows);
    for(size_t i = 0; i < res.size(); i++ )
    {
        res[i] = (l_m.underlying_valarray()[std::slice(i*l_m.cols, l_m.cols, 1)] * r_v.underlying_valarray()).sum();
    }
    return res;
}

template<size_t m, size_t n, typename T, typename U>
requires number<T> && std::same_as<T,U>
auto operator*(const Vector<T>& l_v, const Matrix<m, n, U>& r_m)
{
    if( r_m.rows != l_v.size() )
    {
        throw wrong_operand{"operands are inconsistent."};
    }
    Vector<std::common_type_t<T,U>> res(r_m.cols);
    for(size_t i = 0; i < res.size(); i++ )
    {
        res[i] = (r_m.underlying_valarray()[std::slice(i, r_m.rows, r_m.cols)] * l_v.underlying_valarray()).sum();
    }
    return res;
}

template<typename T, typename U>
requires number<T> && std::same_as<T,U>
auto operator+( const Vector<T>& l, const Vector<U>& r )
{
    if(l.size() != r.size())
    {
        throw wrong_operand{"operands are inconsistent."};
    }
    return Vector<std::common_type_t<T,U>> {l.underlying_valarray()+r.underlying_valarray()};
}

}
#endif // VECTOR_H
