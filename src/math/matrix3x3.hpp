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
        Matrix3x3(Type init_value = static_cast<Type>(0));
        Matrix3x3(Type v11, Type v12, Type v13,
                  Type v21, Type v22, Type v23,
                  Type v31, Type v32, Type v33);

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

        bool operator == (const Matrix3x3& mat) const;
        bool operator != (const Matrix3x3& mat) const;

        Matrix3x3 operator + (const Matrix3x3& mat) const;
        Matrix3x3 operator - (const Matrix3x3& mat) const;
        Matrix3x3 operator * (const Matrix3x3& mat) const;
        Matrix3x3 operator * (Type scal) const;
        Vec3<Type> operator * (const Vec3<Type>& v) const;

        Matrix3x3& operator += (const Matrix3x3& mat);
        Matrix3x3& operator -= (const Matrix3x3& mat);
        Matrix3x3& operator *= (const Matrix3x3& mat);
        Matrix3x3& operator *= (Type scal);

        inline Type* operator [] (size_t i);
        inline const Type* operator [] (size_t i) const;

        Type& at(size_t i, size_t j);
        Type at(size_t i, size_t j) const;

        /**
         * @brief Метод необходимый для вычисления определителя.
         * 
         * @return определитель матрицы.
        */
        Type det() const;

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
    Matrix3x3<Type> transpose(const Matrix3x3<Type>& mat);

    /**
     * @brief Функция необходимая для вычисления обратной матрицы.
     * 
     * @tparam Type тип элементов матрицы.
     * @param mat матрица.
     * @return обратная матрица mat.
    */
    template<typename Type>
    Matrix3x3<Type> inverse(const Matrix3x3<Type>& mat);

    template<typename Type>
    ostream& operator << (ostream& print, const Matrix3x3<Type>& mat);
}

#include "matrix3x3.inl"

#endif /* matrix3x3_hpp */
