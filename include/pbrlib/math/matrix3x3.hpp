//
//  matrix3x3.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 30/01/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef matrix3x3_hpp
#define matrix3x3_hpp

#include <iostream>

namespace pbrlib::math
{
    template<typename Type>
    struct Vec3;

    template<typename Type>
    class Matrix3x3
    {
    public:
        /**
         * @brief Конструктор по умолчанию создаёт единичную матрицу.
        */
        inline constexpr Matrix3x3();
        inline constexpr Matrix3x3(Type init_value);

        inline constexpr Matrix3x3(
            Type x0, Type y0, Type z0,
            Type x1, Type y1, Type z1,
            Type x2, Type y2, Type z2 
        );

        /**
         * @brief Констуктор.
         * 
         * @details 
         *      Указатель ptr_data должен указывать на массив, размер 
         *      которого больше или равняется 9 * sizeof(Type).
         * 
         * @param ptr_data указатель на массив.
        */
        inline Matrix3x3(const Type* ptr_data);

        inline bool operator == (const Matrix3x3& mat) const;
        inline bool operator != (const Matrix3x3& mat) const;

        inline Matrix3x3    operator + (const Matrix3x3& mat)   const;
        inline Matrix3x3    operator - (const Matrix3x3& mat)   const;
        inline Matrix3x3    operator * (const Matrix3x3& mat)   const;
        inline Matrix3x3    operator * (Type scal)              const;
        inline Vec3<Type>   operator * (const Vec3<Type>& v)    const;

        inline Matrix3x3& operator += (const Matrix3x3& mat);
        inline Matrix3x3& operator -= (const Matrix3x3& mat);
        inline Matrix3x3& operator *= (const Matrix3x3& mat);
        inline Matrix3x3& operator *= (Type scal);

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
            Type _array3x3[3][3];
            Type _array9[9];
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
    inline Matrix3x3<Type> transpose(const Matrix3x3<Type>& mat);

    /**
     * @brief Функция необходимая для вычисления обратной матрицы.
     * 
     * @tparam Type тип элементов матрицы.
     * @param mat матрица.
     * @return обратная матрица mat.
    */
    template<typename Type>
    inline Matrix3x3<Type> inverse(const Matrix3x3<Type>& mat);

    template<typename Type>
    inline std::ostream& operator << (std::ostream& print, const Matrix3x3<Type>& mat);
}

namespace pbrlib::math
{
    using mat3 = Matrix3x3<float>;
}


#include "matrix3x3.inl"

#endif /* matrix3x3_hpp */
