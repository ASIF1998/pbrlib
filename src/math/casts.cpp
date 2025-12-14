#include <pbrlib/math/casts.hpp>

#include <algorithm>
#include <cmath>

namespace pbrlib::math
{
    mat4 toMatrix(const quat& q)
    {
        const float xx = q.v.x * q.v.x;
        const float yy = q.v.y * q.v.y;
        const float zz = q.v.z * q.v.z;
        const float xz = q.v.x * q.v.z;
        const float xy = q.v.x * q.v.y;
        const float yz = q.v.y * q.v.z;
        const float wx = q.w * q.v.x;
        const float wy = q.w * q.v.y;
        const float wz = q.w * q.v.z;

        return mat4 (
            1.0f - 2.0f * (yy + zz),    2.0f * (xy + wz),           2.0f * (xz - wy),           0.0f,
            2.0f * (xy - wz),           1.0f - 2.0f * (xx +  zz),   2.0f * (yz + wx),           0.0f,
            2.0f * (xz + wy),           2.0f * (yz - wx),           1.0f - 2.0f * (xx +  yy),   0.0f,
            0.0f,                       0.0f,                       0.0f,                       1.0f
        );
    }

    quat toQuaternion(const mat4& mat)
    {
        /**
         * The algorithm selects the largest quaternion component (based on the trace  
         * or diagonal elements) to ensure accurate calculations and avoid division 
         * by zero  or near-zero values, a technique commonly used in robotics and 
         * graphics  (e.g., in methods described by Chiaverini and Siciliano).
        */

        const float trace = mat[0][0] + mat[1][1] + mat[2][2];

        if (trace > 0.0f)
        {
            const auto s        = std::sqrt(trace + 1.0f) * 2.0f;
            const auto inv_s    = 1.0f / s;

            return quat (
                (mat[2][1] - mat[1][2]) * inv_s, 
                (mat[0][2] - mat[2][0]) * inv_s, 
                (mat[1][0] - mat[0][1]) * inv_s, 
                0.25f * s
            );
        }
        else if ((mat[0][0] > mat[1][1]) && (mat[0][0] > mat[2][2]))
        {
            const auto s        = std::sqrt(1.0f + mat[0][0] - mat[1][1] - mat[2][2]) * 2.0f;
            const auto inv_s    = 1.0f / s;

            return quat (
                0.25f * s, 
                (mat[0][1] + mat[1][0]) * inv_s, 
                (mat[0][2] + mat[2][0]) * inv_s, 
                (mat[2][1] - mat[1][2]) * inv_s
            );
        }
        else if (mat[1][1] > mat[2][2])
        {
            const auto s        = std::sqrt(1.0f + mat[1][1] - mat[0][0] - mat[2][2]) * 2.0f;
            const auto inv_s    = 1.0f / s;

            return quat (
                (mat[0][1] + mat[1][0]) * inv_s, 
                0.25f * s, 
                (mat[1][2] + mat[2][1]) * inv_s, 
                (mat[0][2] - mat[2][0]) * inv_s
            );
        }

        const auto s        = std::sqrt(1.0f + mat[2][2] - mat[0][0] - mat[1][1]) * 2.0f;
        const auto inv_s    = 1.0f / s;

        return quat (
            (mat[0][2] + mat[2][0]) * inv_s, 
            (mat[1][2] + mat[2][1]) * inv_s, 
            0.25f * s, 
            (mat[1][0] - mat[0][1]) * inv_s
        );
    }
}