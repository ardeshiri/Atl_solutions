#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <valarray>
#include <iomanip>
#include <initializer_list>
#include <type_traits>
#include <concepts>

namespace atlatec_test
{

template<typename T>
concept number = std::integral<T> || std::floating_point<T>;

template<size_t m0, size_t n0, size_t m1, size_t n1>
concept same_dimansion = (m0==m1) && (n0==n1);

template<size_t m0, size_t n0, size_t m1, size_t n1, typename T, typename U>
concept productable = (n0==m1) && std::same_as<T,U>;

template<size_t m0, size_t n0, size_t m1, size_t n1, typename T, typename U>
concept addable = (m0==m1) && (n0==n1) && std::same_as<T,U>;

template< size_t m, size_t n, typename T>
requires number<T>
class Matrix
{
public:
    using value_type = T;
    using container_type = std::valarray<value_type>;
    const size_t rows = m;
    const size_t cols = n;
    const size_t size = rows*cols;

    Matrix();
    Matrix(std::initializer_list<std::initializer_list<T>> l);
    explicit Matrix(const std::valarray<T>& v);

    ~Matrix() = default;
    Matrix(const Matrix&) = default;
    Matrix& operator=(const Matrix&) = default;
    Matrix(Matrix&&) = default;
    Matrix& operator=(Matrix&&) = default;

    void print() const;
    const std::valarray<value_type>& underlying_valarray() const noexcept;

    value_type& operator[](size_t i); ///to get value directly from underlying valarray
    const value_type& operator[](size_t i) const;
    value_type& at (size_t, size_t); ///to get value with x and y
    const value_type& at (size_t, size_t) const;

private:
    container_type data;
};

class wrong_input: public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

template< size_t m, size_t n, typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<m, n, T>& mtx )
{
    os<<std::endl;
    for(size_t i = 0 ; i < mtx.rows; i++)
    {
        auto r = mtx.underlying_valarray()[std::slice(i*mtx.cols, mtx.cols, 1)];
        for(size_t j = 0 ; j < mtx.cols; j++)
        {
            os<< std::setw(10) <<r[j];
        }
        os<<std::endl;
    }
    return os;
}

template< size_t m, size_t n, typename T>
Matrix<m, n, T>::Matrix():data(size)
{}

template< size_t m, size_t n, typename T>
void Matrix<m, n, T>::print() const
{
    for(size_t i = 0 ; i < rows; i++)
    {
        auto r = data[std::slice(i*cols, cols, 1)];
        for(size_t j = 0 ; j < cols; j++)
        {
            std::cout<< std::setw(10) <<r[j];
        }
        std::cout<<std::endl;
    }
}

template< size_t m, size_t n, typename T>
Matrix<m, n, T>::Matrix(std::initializer_list<std::initializer_list<T>> l):data(size)
{
    if(rows != l.size())
    {
        throw wrong_input{"wrong input!"};
    }
    for(auto row : l)
    {
        if(cols != row.size())
        {
            throw wrong_input{"wrong input!"};
        }
    }
    int i = 0;
    for(auto row : l)
    {
        for(auto item : row)
        {
            data[i++] = item;
        }
    }
}

template< size_t m, size_t n, typename T>
Matrix<m, n, T>::Matrix(const std::valarray<T>& v):data{v}
{
    if(rows*cols != v.size())
    {
        throw wrong_input{"wrong input!"};
    }
}

template< size_t m, size_t n, typename T>
const Matrix<m, n, T>::container_type & Matrix<m, n, T>::underlying_valarray() const noexcept
{
    return data;
}

template< size_t m, size_t n, typename T>
Matrix<m, n, T>::value_type& Matrix<m, n, T>::operator[](size_t i)
{
    return data[i];
}

template< size_t m, size_t n, typename T>
const Matrix<m, n, T>::value_type& Matrix<m, n, T>::operator[](size_t i) const
{
    return data[i];
}

template< size_t m, size_t n, typename T>
Matrix<m, n, T>::value_type& Matrix<m, n, T>::at (size_t i, size_t j)
{
    if( j >= cols || i >= rows)
    {
        throw std::out_of_range{"wrong index."};
    }
    return data[(i)*cols + j];
}

template< size_t m, size_t n, typename T>
const Matrix<m, n, T>::value_type& Matrix<m, n, T>::at (size_t i, size_t j) const
{
    if( j >= cols || i >= rows)
    {
        throw std::out_of_range{"wrong index."};
    }
    return data[(i)*cols + j];
}

template<size_t m0, size_t n0, size_t m1, size_t n1, typename T, typename U>
requires same_dimansion<m0, n0, m1, n1> && std::same_as<T,U>
bool operator==( const Matrix<m0, n0, T>& l, const Matrix<m1, n1, U>& r )
{
    if constexpr( !std::is_floating_point_v< std::common_type_t<T,U>> )
    {
        const std::valarray<bool>& res = l.underlying_valarray()==r.underlying_valarray();
        return std::all_of( std::begin(res), std::end(res), [](const auto& b)
        {
            return b == true;
        } );
    }
    else
    {
        const double epsilon = 0.00001; /// needs proper adjustment
        const std::valarray<std::common_type_t<T,U>>& res = std::abs(r.underlying_valarray()-l.underlying_valarray());
        return std::all_of( std::begin(res), std::end(res), [epsilon](const auto& v)
        {
            return v <= epsilon;
        } );
    }
}

template<size_t m0, size_t n0, size_t m1, size_t n1, typename T, typename U>
requires productable<m0, n0, m1, n1, T, U>
auto operator*( const Matrix<m0, n0, T>& l, const Matrix<m1, n1, U>& r )
{
    Matrix<m0, n1, std::common_type_t<T,U>> res{};
    for( size_t i = 0 ; i < l.rows; i++)
    {
        auto l_row = l.underlying_valarray()[std::slice(i*l.cols, l.cols, 1)];
        for(size_t j = 0 ; j < r.cols ; j++)
        {
            auto r_col = r.underlying_valarray()[std::slice(j, r.rows, r.cols)];
            res[i*r.cols + j ] = (l_row*r_col).sum();
        }
    }
    return res;
}

template<size_t m0, size_t n0, size_t m1, size_t n1, typename T, typename U>
requires addable<m0, n0, m1, n1, T, U>
auto operator+( const Matrix<m0, n0, T>& l, const Matrix<m1, n1, U>& r )
{
    return Matrix<m0, n0, std::common_type_t<T,U>> { l.underlying_valarray()+r.underlying_valarray() };
}

template<size_t m, size_t n, typename T, typename U>
requires number<T> && number<U> && std::same_as<T,U>
auto operator*( U sc, const Matrix<m, n, T>& r )
{
    return Matrix<m, n, T> { sc*r.underlying_valarray() };
}

template<size_t m, size_t n, typename T, typename U>
requires std::same_as<T,U>
auto operator*( const Matrix<m, n, T>& l, U sc )
{
    return sc*l;
}

}
#endif // MATRIX_H
