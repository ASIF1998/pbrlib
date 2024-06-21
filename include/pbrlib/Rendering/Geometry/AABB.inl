//
//  AABB.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 10/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    template<typename TPos>
    AABB AABB::computeAABB(const TPos* ptr_positions, size_t num_positions)
    {
        if (ptr_positions && num_positions) {
            const math::Vec3<float>*    ptr     (reinterpret_cast<const math::Vec3<float>*>(ptr_positions));
            AABB                        bbox    (ptr[0]);

            for (size_t i{1}; i < num_positions; i++) {
                bbox = AABB::aabbUnion(bbox, ptr[i]);
            }

            return bbox;
        }

        return AABB();
    }

    template<typename TPositions>
    AABB AABB::computeAABB(const TPositions& positions)
    {
        if (positions.size()) {
            const math::Vec3<float>*    ptr     (reinterpret_cast<const math::Vec3<float>*>(&positions[0]));
            AABB                        bbox    (ptr[0]);

            for (size_t i{1}; i < positions.size(); i++) {
                bbox = AABB::aabbUnion(bbox, ptr[i]);
            }

            return bbox;
        }

        return AABB();
    }
}
