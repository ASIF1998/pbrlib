//
//  AABB.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 19/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef AABB_hpp
#define AABB_hpp

#include "../../math/vec3.hpp"

namespace pbrlib
{
    using namespace math;

    class AABB
    {
    public:
        AABB();
        AABB(const Vec3<float>& p);
        AABB(const Vec3<float>& p1, const Vec3<float>& p2);

        /**
         * @brief 
         *      Перегруженный оператор [] возвращающий минимальную 
         *      или максимальную точку ограничивающего объёма.
         * 
         * @param i индекс точки (i == 0 => p_min, i == 1 => p_max).
         * @return минимальную или максимальную точку ограничивающего объёма.
        */
        Vec3<float>& operator [] (size_t i);

        /**
         * @brief 
         *      Перегруженный оператор [] возвращающий минимальную 
         *      или максимальную точку ограничивающего объёма.
         * 
         * @param i индекс точки (i == 0 => p_min, i == 1 => p_max).
         * @return минимальную или максимальную точку ограничивающего объёма.
        */
        Vec3<float> operator [] (size_t i) const;

        bool operator == (const AABB& bbox) const;
        bool operator != (const AABB& bbox) const;

        /**
         * @brief Метод, возвращающий один из 8'и углов ограничивающего объёма.
         * 
         * @param c индекс угла.
         * @return угол ограничивающего объёма.
        */
        Vec3<float> corner(size_t c) const;

        /**
         * @brief Метод, вычисляющий диагональ ограничивающего объёма.
         * 
         * @return диагональ ограничивающего объёма.
        */
        Vec3<float> diagonal() const;

        float surfaceArea() const;
        float volume()      const;

        uint maximumExtent() const;

        Vec3<float> lerp(const Vec3<float>& t) const;

        /**
         * @brief 
         *      Статический метод, вычисляющий новый ограничивающий объём,
         *      который охватывает как bbox, так и p.
         * 
         * @param bbox  ограничивающий объём.
         * @param p     координаты точки.
         * @return ограничивающий объём, который охватывает как bbox, так и p.
        */
        static AABB aabbUnion(const AABB& bbox, const Vec3<float>& p);

        /**
         * @brief 
         *      Статический метод, вычисляющий новый ограничивающий объём,
         *      который охватывает как bbox1, так и bbox2.
         * 
         * @param bbox1 ограничивающий объём.
         * @param bbox2 ограничивающий объём.
         * @return ограничивающий объём, который охватывает как bbox1, так и bbox2.
        */
        static AABB aabbUnion(const AABB& bbox1, const AABB& bbox2);

        /**
         * @brief 
         *      Статический метод, вычисляющий новый ограничивающий объём,
         *      находящийся на пересечении bbox1 и bbox2.
         * 
         * @param bbox1 ограничивающий объём.
         * @param bbox2 ограничивающий объём.
         * @return ограничивающий объём,находящийся на пересечении bbox1 и bbox2.
        */
        static AABB intersect(const AABB& bbox1, const AABB& bbox2);

        /**
         * @brief 
         *      Статический метод, выявляющий, пересекаются ли 
         *      bbox1 и bbox2.
         * 
         * @param bbox1 ограничивающий объём.
         * @param bbox2 ограничивающий объём.
         * @return true - если bbox1 и bbox2 перекрываются, иначе false.
        */
        static bool overlaps(const AABB& bbox1, const AABB& bbox2);

        /**
         * @brief 
         *      Статический метод, выявляющий, находится ли p 
         *      внутри bbox.
         * 
         * @param bbox  ограничивающий объём.
         * @param p     координаты точки.
         * @return true - если p внутри bbox, иначе false.
        */
        static bool inside(const AABB& bbox, const Vec3<float>& p);
        
        /**
         * @brief 
         *      Статический метод, расширяющий bbox постоянным 
         *      коэффициентом delta.
         * 
         * @param bbox  ограничивающий объём.
         * @param delta постоянный коэффициент.
         * @return bbox, только расширенный delta.
        */
        static AABB expand(const AABB& bbox, float delta);

    private:
        
        union
        {
            struct
            {
                Vec3<float> _p_min;
                Vec3<float> _p_max;
            };

            Vec3<float> _min_max[2];
        };
    };
}

#endif /* AABB_hpp */
