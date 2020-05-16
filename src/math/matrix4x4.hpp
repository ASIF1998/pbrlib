//
//  matrix4x4.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 22/01/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef matrix4x4_hpp
#define matrix4x4_hpp

#include <iostream>

#include <immintrin.h>

#include "vec4.hpp"

using namespace std;

namespace pbrlib::math
{
    template<typename Type>
    class Matrix4x4
    {
    public:
        /**
         * @brief Конструктор по умолчанию создаёт единичную матрицу.
        */
        inline constexpr Matrix4x4();

        inline constexpr Matrix4x4(Type init_value);

        inline constexpr Matrix4x4(
            Type v11, Type v12, Type v13, Type v14,
            Type v21, Type v22, Type v23, Type v24,
            Type v31, Type v32, Type v33, Type v34,
            Type v41, Type v42, Type v43, Type v44
        );

        /**
         * @brief Констуктор.
         * 
         * @details 
         *      Указатель ptr_data должен указывать на массив, размер 
         *      которого больше или равняется 16 * sizeof(Type).
         * 
         * @param ptr_data указатель на массив.
        */
        inline Matrix4x4(const Type* ptr_data);

        inline bool operator == (const Matrix4x4& mat) const;
        inline bool operator != (const Matrix4x4& mat) const;

        inline Matrix4x4    operator + (const Matrix4x4& mat)   const;
        inline Matrix4x4    operator - (const Matrix4x4& mat)   const;
        inline Matrix4x4    operator * (const Matrix4x4& mat)   const;
        inline Matrix4x4    operator * (Type scal)              const;
        inline Vec4<Type>   operator * (const Vec4<Type>& v)    const;

        inline Matrix4x4& operator += (const Matrix4x4& mat);
        inline Matrix4x4& operator -= (const Matrix4x4& mat);
        inline Matrix4x4& operator *= (const Matrix4x4& mat);
        inline Matrix4x4& operator *= (Type scal);

        inline Type*        operator [] (size_t i);
        inline const Type*  operator [] (size_t i) const;

        inline Type&    at(size_t i, size_t j);
        inline Type     at(size_t i, size_t j) const;

        /**
         * @brief Метод необходимый для вычисления определителя.
         * 
         * @return определитель матрицы.
        */
        inline Type det() const;

        /**
         * @brief Метод необходимый для транспонирования матрицы.
        */
        inline void transpose();

        /**
         * @brief Метод находящий обратную матрицу.
        */
        inline void inverse();

    private:
        union
        {
            Type _array4x4[4][4];
            Type _array16[16];
        };
    };

    template<>
    class Matrix4x4<float>
    {
    public:
        /**
         * @brief Конструктор по умолчанию создаёт единичную матрицу.
        */
        inline constexpr Matrix4x4();
        
        inline constexpr Matrix4x4(float init_value);

        inline constexpr Matrix4x4(
            float v11, float v12, float v13, float v14,
            float v21, float v22, float v23, float v24,
            float v31, float v32, float v33, float v34,
            float v41, float v42, float v43, float v44
        );

        /**
         * @brief Констуктор.
         * 
         * @details 
         *      Указатель ptr_data должен указывать на массив, размер 
         *      которого больше или равняется 16 * sizeof(float).
         * 
         * @param ptr_data указатель на массив.
        */
        inline Matrix4x4(const float* ptr_data);

        inline bool operator == (const Matrix4x4& mat) const;
        inline bool operator != (const Matrix4x4& mat) const;

        inline Matrix4x4    operator + (const Matrix4x4& mat)   const;
        inline Matrix4x4    operator - (const Matrix4x4& mat)   const;
        inline Matrix4x4    operator * (const Matrix4x4& mat)   const;
        inline Matrix4x4    operator * (float scal)             const;
        inline Vec4<float>  operator * (const Vec4<float>& v)   const;

        inline Matrix4x4& operator += (const Matrix4x4& mat);
        inline Matrix4x4& operator -= (const Matrix4x4& mat);
        inline Matrix4x4& operator *= (const Matrix4x4& mat);
        inline Matrix4x4& operator *= (float scal);

        inline float*       operator [] (size_t i) noexcept;
        inline const float* operator [] (size_t i) const noexcept;

        inline float&   at (size_t i, size_t j);
        inline float    at (size_t i, size_t j) const;

        /**
         * @brief Метод необходимый для вычисления определителя.
         * 
         * @return определитель матрицы.
        */
        inline float det() const;

        /**
         * @brief Метод необходимый для транспонирования матрицы.
        */
        inline void transpose();

        /**
         * @brief Метод находящий обратную матрицу.
        */
        inline void inverse();

    private:
        inline Matrix4x4(const __m256& s1, const __m256& s2);

        union
        {
            float   _array4x4[4][4];
            float   _array16[16];
            __m128  _m128_simd[4];
            __m256  _m256_simd[2];
        };
    };

    /**
     * @brief Функция необходимая для транспонирования матрицы.
     * 
     * @tparam Type тип элементов матрицы.
     * @param mat матрица.
     * @return транспонированная матрица mat.
    */
    template<typename Type>
    inline Matrix4x4<Type> transpose(const Matrix4x4<Type>& mat);

    /**
     * @brief Функция необходимая для вычисления обратной матрицы.
     * 
     * @tparam Type тип элементов матрицы.
     * @param mat матрица.
     * @return обратная матрица mat.
    */
    template<typename Type>
    inline Matrix4x4<Type> inverse(const Matrix4x4<Type>& mat);

    template<typename Type>
    inline ostream& operator << (ostream& print, const Matrix4x4<Type>& mat);
}

#include "matrix4x4.inl"

#endif /* matrix4x4_hpp */
