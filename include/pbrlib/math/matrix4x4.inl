#include <pbrlib/exceptions.hpp>

#include <pbrlib/math/vec4.hpp>

#include <pbrlib/utils/combine_hash.hpp>

#include <memory>
#include <cassert>

#include <stdexcept>
#include <format>

namespace pbrlib::math
{
    template<MathArithmetic T>
    inline constexpr Matrix4x4<T>::Matrix4x4() noexcept :
        _array16 
        {
            static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)
        }
    {}

    template<MathArithmetic T>
    inline constexpr Matrix4x4<T>::Matrix4x4(T init_value) noexcept :
        _array16 
        {
            init_value, static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
            static_cast<T>(0), init_value, static_cast<T>(0), static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0), init_value, static_cast<T>(0),
            static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), init_value
        }
    {}

    template<MathArithmetic T>
    inline constexpr Matrix4x4<T>::Matrix4x4 (
        T x0, T y0, T z0, T w0,
        T x1, T y1, T z1, T w1,
        T x2, T y2, T z2, T w2,
        T x3, T y3, T z3, T w3
    ) noexcept :
        _array16 
        {
            x0, y0, z0, w0,
            x1, y1, z1, w1,
            x2, y2, z2, w2,
            x3, y3, z3, w3
        }
    {}

    template<MathArithmetic T>
    inline constexpr bool Matrix4x4<T>::operator == (const Matrix4x4<T>& mat) const noexcept
    {
        bool res = true;

        for (size_t i = 0; i < 16; i++)
            res &= _array16[i] == mat._array16[i];

        return res;
    }

    template<MathArithmetic T>
    inline constexpr bool Matrix4x4<T>::operator != (const Matrix4x4<T>& mat) const noexcept
    {
        bool res = true;

        for (size_t i = 0; i < 16; i++)
            res |= _array16[i] != mat._array16[i];

        return res;
    }

    template<MathArithmetic T>
    inline constexpr Matrix4x4<T> Matrix4x4<T>::operator + (const Matrix4x4<T>& mat) const noexcept
    {
        Matrix4x4<T> res;

        for (size_t i = 0; i < 4; ++i)
            res._vec_array[i] = _vec_array[i] + mat._vec_array[i]; 

        return res;
    }

    template<MathArithmetic T>
    inline constexpr Matrix4x4<T> Matrix4x4<T>::operator - (const Matrix4x4<T>& mat) const noexcept
    {
        Matrix4x4<T> res;

        for (size_t i = 0; i < 4; ++i)
            res._vec_array[i] = _vec_array[i] - mat._vec_array[i]; 

        return res;
    }

    template<MathArithmetic T>
    inline constexpr Matrix4x4<T> Matrix4x4<T>::operator * (const Matrix4x4<T>& mat) const noexcept
    {
        Matrix4x4<T> res (static_cast<T>(0));

        for (size_t i = 0; i < 4; i++) 
        {
            for (size_t k = 0; k < 4; k++) 
            {
                auto v = mat._array4x4[i][k];
                for (size_t j = 0; j < 4; j++) 
                    res._array4x4[i][j] += v * _array4x4[k][j];
            }
        }

        return res;
    }

    template<MathArithmetic T>
    inline constexpr Matrix4x4<T> operator * (const Matrix4x4<T>& mat, T s)
    {
        Matrix4x4<T> res;
        for (size_t i = 0; i < 4; ++i)
        {
            for (size_t j = 0; j < 4; ++j)
                res.at(i, j) = mat.at(i, j) * s;
        }

        return res;
    }

    template<MathArithmetic T>
    inline constexpr Matrix4x4<T> operator * (T s, const Matrix4x4<T>& mat)
    {
        Matrix4x4<T> res;
        for (size_t i = 0; i < 4; ++i)
        {
            for (size_t j = 0; j < 4; ++j)
                res.at(i, j) = mat.at(i, j) * s;
        }

        return res;
    }

    template<MathArithmetic T>
    inline constexpr Vec4<T> Matrix4x4<T>::operator * (const Vec4<T>& v) const noexcept
    {
        Vec4<T> res;

        for (size_t i = 0; i < 4; i++) 
        {
            res.x += _array4x4[i][0] * v[i];
            res.y += _array4x4[i][1] * v[i];
            res.z += _array4x4[i][2] * v[i];
            res.w += _array4x4[i][3] * v[i];
        }

        return res;
    }

    template<MathArithmetic T>
    inline constexpr Matrix4x4<T>& Matrix4x4<T>::operator += (const Matrix4x4<T>& mat) noexcept
    {
        for (size_t i = 0; i < 4; ++i)
            _vec_array[i] += mat._vec_array[i];

        return *this;
    }

    template<MathArithmetic T>
    inline constexpr Matrix4x4<T>& Matrix4x4<T>::operator -= (const Matrix4x4<T>& mat) noexcept
    {
        for (size_t i = 0; i < 4; ++i)
            _vec_array[i] -= mat._vec_array[i];

        return *this;
    }

    template<MathArithmetic T>
    inline constexpr Matrix4x4<T>& Matrix4x4<T>::operator *= (const Matrix4x4<T>& mat) noexcept
    {
        *this = *this * mat;
        return *this;
    }

    template<MathArithmetic T>
    inline constexpr Matrix4x4<T>& Matrix4x4<T>::operator *= (T scal) noexcept
    {
        for (size_t i = 0; i < 4; i++)
            _vec_array[i] *= scal;

        return *this;
    }

    template<MathArithmetic T>
    inline T* Matrix4x4<T>::operator [] (size_t i) noexcept
    {
        return _array4x4[i];
    }

    template<MathArithmetic T>
    inline const T* Matrix4x4<T>::operator [] (size_t i) const noexcept
    {
        return _array4x4[i];
    }

    template<MathArithmetic T>
    inline T& Matrix4x4<T>::at(size_t i, size_t j)
    {
        if (i > 3 || j > 3) [[unlikely]] 
            throw exception::InvalidArgument(std::format("[math::mat4] i = {}, j = {}", i, j));

        return _array4x4[i][j];
    }

    template<MathArithmetic T>
    inline constexpr T Matrix4x4<T>::at(size_t i, size_t j) const noexcept
    {
        // In constexpr context MSVC cannot handle array access in union
        // Use direct conditional access based on i and j
        return (i == 0 && j == 0) ? _array16[0] : 
               (i == 0 && j == 1) ? _array16[1] : 
               (i == 0 && j == 2) ? _array16[2] : 
               (i == 0 && j == 3) ? _array16[3] : 
               (i == 1 && j == 0) ? _array16[4] : 
               (i == 1 && j == 1) ? _array16[5] : 
               (i == 1 && j == 2) ? _array16[6] : 
               (i == 1 && j == 3) ? _array16[7] : 
               (i == 2 && j == 0) ? _array16[8] : 
               (i == 2 && j == 1) ? _array16[9] : 
               (i == 2 && j == 2) ? _array16[10] : 
               (i == 2 && j == 3) ? _array16[11] : 
               (i == 3 && j == 0) ? _array16[12] : 
               (i == 3 && j == 1) ? _array16[13] : 
               (i == 3 && j == 2) ? _array16[14] : _array16[15];
    }

    template<MathArithmetic T>
    inline constexpr T Matrix4x4<T>::det() const noexcept
    {
        auto a33444334 = at(2, 2) * at(3, 3) - at(3, 2) * at(2, 3);
        auto a32443442 = at(2, 1) * at(3, 3) - at(2, 3) * at(3, 1);
        auto a32433342 = at(2, 1) * at(3, 2) - at(2, 2) * at(3, 1);
        auto a31443441 = at(2, 0) * at(3, 3) - at(2, 3) * at(3, 0);
        auto a31433341 = at(2, 0) * at(3, 2) - at(2, 2) * at(3, 0);
        auto a31423241 = at(2, 0) * at(3, 1) - at(2, 1) * at(3, 0);

        auto s1 = at(0, 0) * (
            at(1, 1) * a33444334 -
            at(1, 2) * a32443442 +
            at(1, 3) * a32433342
        );

        auto s2 = at(0, 1) * (
            at(1, 0) * a33444334 -
            at(1, 2) * a31443441 +
            at(1, 3) * a31433341
        );

        auto s3 = at(0, 2) * (
            at(1, 0) * a32443442 -
            at(1, 1) * a31443441 +
            at(1, 3) * a31423241
        );

        auto s4 = at(0, 3) * (
            at(1, 0) * a32433342 -
            at(1, 1) * a31433341 +
            at(1, 2) * a31423241
        );

        return s1 - s2 + s3 - s4;
    }

    template<MathArithmetic T>
    inline void Matrix4x4<T>::transpose() noexcept
    {
        std::swap(_array4x4[0][1], _array4x4[1][0]);
        std::swap(_array4x4[0][2], _array4x4[2][0]);
        std::swap(_array4x4[0][3], _array4x4[3][0]);
        std::swap(_array4x4[1][2], _array4x4[2][1]);
        std::swap(_array4x4[1][3], _array4x4[3][1]);
        std::swap(_array4x4[2][3], _array4x4[3][2]);
    }

    template<MathArithmetic T>
    inline void Matrix4x4<T>::inverse() noexcept
    {
        auto d = det();

        if (d != T(0)) [[likely]] 
        {
            auto tmat = pbrlib::math::transpose(*this);

            auto a33444334 = tmat[2][2] * tmat[3][3] - tmat[3][2] * tmat[2][3];
            auto a32443442 = tmat[2][1] * tmat[3][3] - tmat[2][3] * tmat[3][1];
            auto a32433342 = tmat[2][1] * tmat[3][2] - tmat[2][2] * tmat[3][1];
            auto a31443441 = tmat[2][0] * tmat[3][3] - tmat[2][3] * tmat[3][0];
            auto a31433341 = tmat[2][0] * tmat[3][2] - tmat[2][2] * tmat[3][0];
            auto a31423241 = tmat[2][0] * tmat[3][1] - tmat[2][1] * tmat[3][0];
            auto a23442443 = tmat[1][2] * tmat[3][3] - tmat[1][3] * tmat[3][2];
            auto a22444224 = tmat[1][1] * tmat[3][3] - tmat[3][1] * tmat[1][3];
            auto a22432342 = tmat[1][1] * tmat[3][2] - tmat[1][2] * tmat[3][1];
            auto a21442441 = tmat[1][0] * tmat[3][3] - tmat[1][3] * tmat[3][0];
            auto a21432341 = tmat[1][0] * tmat[3][2] - tmat[1][2] * tmat[3][0];
            auto a21422241 = tmat[1][0] * tmat[3][1] - tmat[1][1] * tmat[3][0];
            auto a23342433 = tmat[1][2] * tmat[2][3] - tmat[1][3] * tmat[2][2];
            auto a22342432 = tmat[1][1] * tmat[2][3] - tmat[1][3] * tmat[2][1];
            auto a22332332 = tmat[1][1] * tmat[2][2] - tmat[1][2] * tmat[2][1];
            auto a21342431 = tmat[1][0] * tmat[2][3] - tmat[1][3] * tmat[2][0];
            auto a21332331 = tmat[1][0] * tmat[2][2] - tmat[1][2] * tmat[2][0];
            auto a21322231 = tmat[1][0] * tmat[2][1] - tmat[1][1] * tmat[2][0];

            _array4x4[0][0] =   tmat[1][1] * a33444334 - tmat[1][2] * a32443442 + tmat[1][3] * a32433342;
            _array4x4[0][1] = -(tmat[1][0] * a33444334 - tmat[1][2] * a31443441 + tmat[1][3] * a31433341);
            _array4x4[0][2] =   tmat[1][0] * a32443442 - tmat[1][1] * a31443441 + tmat[1][3] * a31423241;
            _array4x4[0][3] = -(tmat[1][0] * a32433342 - tmat[1][1] * a31433341 + tmat[1][2] * a31423241);

            _array4x4[1][0] = -(tmat[0][1] * a33444334 - tmat[0][2] * a32443442 + tmat[0][3] * a32433342);
            _array4x4[1][1] =   tmat[0][0] * a33444334 - tmat[0][2] * a31443441 + tmat[0][3] * a31433341;
            _array4x4[1][2] = -(tmat[0][0] * a32443442 - tmat[0][1] * a31443441 + tmat[0][3] * a31423241);
            _array4x4[1][3] =   tmat[0][0] * a32433342 - tmat[0][1] * a31433341 + tmat[0][2] * a31423241;

            _array4x4[2][0] =   tmat[0][1] * a23442443 - tmat[0][2] * a22444224 + tmat[0][3] * a22432342;
            _array4x4[2][1] = -(tmat[0][0] * a23442443 - tmat[0][2] * a21442441 + tmat[0][3] * a21432341);
            _array4x4[2][2] =   tmat[0][0] * a22444224 - tmat[0][1] * a21442441 + tmat[0][3] * a21422241;
            _array4x4[2][3] = -(tmat[0][0] * a22432342 - tmat[0][1] * a21432341 + tmat[0][2] * a21422241);

            _array4x4[3][0] = -(tmat[0][1] * a23342433 - tmat[0][2] * a22342432 + tmat[0][3] * a22332332);
            _array4x4[3][1] =   tmat[0][0] * a23342433 - tmat[0][2] * a21342431 + tmat[0][3] * a21332331;
            _array4x4[3][2] = -(tmat[0][0] * a22342432 - tmat[0][1] * a21342431 + tmat[0][3] * a21322231);
            _array4x4[3][3] =   tmat[0][0] * a22332332 - tmat[0][1] * a21332331 + tmat[0][2] * a21322231;

            for (size_t i = 0; i < 16; i++) 
                _array16[i] /= d;
        }
    }
}

namespace pbrlib::math
{
    template<MathArithmetic T>
    inline Matrix4x4<T> transpose(const Matrix4x4<T>& mat) noexcept
    {
        return 
        {
            mat[0][0], mat[1][0], mat[2][0], mat[3][0],
            mat[0][1], mat[1][1], mat[2][1], mat[3][1],
            mat[0][2], mat[1][2], mat[2][2], mat[3][2],
            mat[0][3], mat[1][3], mat[2][3], mat[3][3]
        };
    }

    template<MathArithmetic T>
    inline Matrix4x4<T> inverse(const Matrix4x4<T>& mat) noexcept
    {
        Matrix4x4<T> res (mat);
        res.inverse();
        return res;
    }

    template<MathArithmetic T>
    inline Matrix4x4<T> round(const Matrix4x4<T>& mat) noexcept
    {
        const Matrix4x4<T> res (
            std::round(mat.at(0, 0)), std::round(mat.at(0, 1)), std::round(mat.at(0, 2)), std::round(mat.at(0, 3)),
            std::round(mat.at(1, 0)), std::round(mat.at(1, 1)), std::round(mat.at(1, 2)), std::round(mat.at(1, 3)),
            std::round(mat.at(2, 0)), std::round(mat.at(2, 1)), std::round(mat.at(2, 2)), std::round(mat.at(2, 3)),
            std::round(mat.at(3, 0)), std::round(mat.at(3, 1)), std::round(mat.at(3, 2)), std::round(mat.at(3, 3))
        );

        return res;
    }
}

namespace std
{
    template<pbrlib::math::MathArithmetic T>
    constexpr auto formatter<pbrlib::math::Matrix4x4<T>>::parse(format_parse_context& ctx) const
    {
        return ctx.begin();
    }

    template<pbrlib::math::MathArithmetic T>
    auto formatter<pbrlib::math::Matrix4x4<T>>::format (
        const pbrlib::math::Matrix4x4<T>&   mat, 
        format_context&                     ctx
    ) const
    {
        return format_to (
            ctx.out(), 
            "\nmat4 [\n  [{}, {}, {}, {}]\n  [{}, {}, {}, {}]\n  [{}, {}, {}, {}]\n  [{}, {}, {}, {}]\n]", 
                mat[0][0], mat[0][1], mat[0][2], mat[0][3], 
                mat[1][0], mat[1][1], mat[1][2], mat[1][3], 
                mat[2][0], mat[2][1], mat[2][2], mat[1][3],
                mat[3][0], mat[3][1], mat[3][2], mat[3][3]
        );
    }

    template<pbrlib::math::MathArithmetic T>
    inline constexpr size_t hash<pbrlib::math::Matrix4x4<T>>::operator () (const pbrlib::math::Matrix4x4<T>& mat) const noexcept
    {
        size_t hash_value = 0;
        for (size_t i = 0; i < 4; ++i)
        {
            for (size_t j = 0; j < 4; ++j)
                pbrlib::combineHash(hash_value, mat.at(i, j));
        }

        return hash_value;
    }
}