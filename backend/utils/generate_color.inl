namespace pbrlib::backend::vk::marker_colors
{
    /// from Chris Wellons https://nullprogram.com/blog/2018/07/31/
    constexpr uint32_t lowbias32(uint32_t x)
    {
        x ^= x >> 16;
        x *= 0x7feb352dU;
        x ^= x >> 15;
        x *= 0x846ca68bU;
        x ^= x >> 16;
        return x;
    }

    constexpr pbrlib::math::vec3 generateColor(uint32_t val)
    {
        auto hash = lowbias32(val);

        auto r = static_cast<float>((hash >> 22) & 0x3ff) * 0.000977517f;
        auto g = static_cast<float>((hash >> 12) & 0x3ff) * 0.000977517f;
        auto b = static_cast<float>(hash & 0xfff) * 0.0002442002f;

        return pbrlib::math::vec3(r, g, b);
    }
}
