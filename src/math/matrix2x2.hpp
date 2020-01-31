//
//  matrix2x2.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 30/01/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef matrix2x2_hpp
#define matrix2x2_hpp

#include <iostream>

#include <xmmintrin.h>

using namespace std;

namespace pbrlib::math
{
    template<typename Type>
    struct Vec2;

    template<typename Type>
    class Matrix2x2
    {
    public:
        Matrix2x2(Type init_value = static_cast<Type>(0));
        inline Matrix2x2(Type v11, Type v12, Type v21, Type v22);

        /**
         * @brief Констуктор.
         * 
         * @details 
         *      Указатель ptr_data должен указывать на массив, размер 
         *      которого больше или равняется 4 * sizeof(Type).
         * 
         * @param ptr_data указатель на массив.
        */
        inline Matrix2x2(const Type* ptr_data);

        inline bool operator == (const Matrix2x2& mat) const;
        inline bool operator != (const Matrix2x2& mat) const;

        inline Matrix2x2 operator + (const Matrix2x2& mat) const;
        inline Matrix2x2 operator - (const Matrix2x2& mat) const;
        inline Matrix2x2 operator * (const Matrix2x2& mat) const;
        inline Matrix2x2 operator * (Type scal) const;
        inline Vec2<Type> operator * (const Vec2<Type>& v) const;

        Matrix2x2& operator += (const Matrix2x2& mat);
        Matrix2x2& operator -= (const Matrix2x2& mat);
        Matrix2x2& operator *= (const Matrix2x2& mat);
        Matrix2x2& operator *= (Type scal);

        inline Type* operator [] (size_t i);
        inline const Type* operator [] (size_t i) const;

        Type& at(size_t i, size_t j);
        Type at(size_t i, size_t j) const;

        /**
         * @brief Метод необходимый для вычисления определителя.
         * 
         * @return определитель матрицы.
        */
        inline Type det() const;

        /**
         * @brief Метод необходимый для транспонирования матрицы.
        */
        void transpose();

        /**
         * @brief Метод находящий обратную матрицу.
        */
        void inverse();

    private:
        union
        {
            Type _array2x2[2][2];
            Type _array4[4];
        };
    };

    template<>
    class Matrix2x2<float>
    {
    public:
        Matrix2x2(float init_value = 0.0f) noexcept;
        inline Matrix2x2(float v11, float v12, float v21, float v22) noexcept;
        inline Matrix2x2(const __m128& init_vec) noexcept;

        /**
         * @brief Констуктор.
         * 
         * @details 
         *      Указатель ptr_data должен указывать на массив, размер 
         *      которого больше или равняется 4 * sizeof(float).
         * 
         * @param ptr_data указатель на массив.
        */
        inline Matrix2x2(const float* ptr_data);

        inline bool operator == (const Matrix2x2& mat) const;
        inline bool operator != (const Matrix2x2& mat) const;

        inline Matrix2x2 operator + (const Matrix2x2& mat) const;
        inline Matrix2x2 operator - (const Matrix2x2& mat) const;
        inline Matrix2x2 operator * (const Matrix2x2& mat) const;
        inline Matrix2x2 operator * (float scal) const;
        inline Vec2<float> operator * (const Vec2<float>& v) const;

        inline Matrix2x2& operator += (const Matrix2x2& mat);
        inline Matrix2x2& operator -= (const Matrix2x2& mat);
        inline Matrix2x2& operator *= (const Matrix2x2& mat);
        inline Matrix2x2& operator *= (float scal);

        inline float* operator [] (size_t i);
        inline const float* operator [] (size_t i) const;

        inline float& at(size_t i, size_t j);
        inline float at(size_t i, size_t j) const;

        /**
         * @brief Метод необходимый для вычисления определителя.
         * 
         * @return определитель матрицы.
        */
        inline float det() const;

        /**
         * @brief Метод необходимый для транспонирования матрицы.
        */
        void transpose();

        /**
         * @brief Метод находящий обратную матрицу.
        */
        void inverse();

    private:
        union
        {
            float _array2x2[2][2];
            float _array4[4];
            __m128 _m128_simd;
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
    inline Matrix2x2<Type> transpose(const Matrix2x2<Type>& mat);

    /**
     * @brief Функция необходимая для вычисления обратной матрицы.
     * 
     * @tparam Type тип элементов матрицы.
     * @param mat матрица.
     * @return обратная матрица mat.
    */
    template<typename Type>
    Matrix2x2<Type> inverse(const Matrix2x2<Type>& mat);

    template<typename Type>
    ostream& operator << (ostream& print, const Matrix2x2<Type>& mat);
}

#include "matrix2x2.inl"

#endif /* matrix2x2_hpp */
