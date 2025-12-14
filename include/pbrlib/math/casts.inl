#include <numbers>

namespace pbrlib::math
{
    template<std::floating_point T>
    inline constexpr T toRadians(T degrees)
    {
        return degrees * (std::numbers::pi_v<T> / static_cast<T>(180.0));
    }

    template<std::floating_point T>
    inline constexpr T toDegrees(T radians)
    {
        return radians * (static_cast<T>(180.0) / std::numbers::pi_v<T>);
    }
}