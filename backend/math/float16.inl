namespace pbrlib::backend::math
{
    inline constexpr float16_t::float16_t(float value) noexcept
    {
        const uint32_t f32 = std::bit_cast<uint32_t>(value);

        const uint32_t sign     = (f32 >> 16) & 0x8000;
        const int32_t  exponent = static_cast<int32_t>((f32 >> 23) & 0xff) - 127;
        const uint32_t mantissa = f32 & 0x007fffff;

        if (exponent <= -15)
        {
            if (exponent < -24) 
                _bits = static_cast<uint16_t>(sign);
            else 
            {
                const uint32_t sub_mantissa = (mantissa | 0x00800000) >> (-14 - exponent);
                _bits = static_cast<uint16_t>(sign | (sub_mantissa & 0x3FF));
            }
        }
        else if (exponent >= 16)
            _bits = static_cast<uint16_t>(sign | 0x7C00); // Бесконечность
        else
            _bits = static_cast<uint16_t>(sign | ((exponent + 15) << 10) | (mantissa >> 13));
    }

    inline constexpr float16_t::operator float() const noexcept
    {
        const uint32_t sign     = (_bits & 0x8000) << 16;
        uint32_t       exponent = (_bits & 0x7C00) >> 10;
        uint32_t       mantissa = _bits & 0x03FF;

        uint32_t f32 = 0;
        if (exponent == 0)
        {
            if (mantissa != 0)
            {
                exponent = 1;
                while ((mantissa & 0x0400) == 0)
                {
                    mantissa <<= 1;
                    exponent--;
                }
                mantissa &= 0x03FF;
                f32 = sign | ((exponent + 127 - 15) << 23) | (mantissa << 13);
            }
            else
                f32 = sign;
        }
        else if (exponent == 0x1F)
            f32 = sign | 0x7F800000 | (mantissa << 13);
        else
            f32 = sign | ((exponent + 127 - 15) << 23) | (mantissa << 13);

        return std::bit_cast<float>(f32);
    }

    inline constexpr float16_t& float16_t::operator += (float16_t v) noexcept
    {
        *this = float16_t(float(*this) + float(v));
        return *this;
    }

    inline constexpr float16_t operator + (float16_t a, float16_t b) noexcept 
    {
        return float(a) + float(b);
    }

    inline constexpr float16_t operator - (float16_t a, float16_t b) noexcept 
    {
        return float(a) - float(b);
    }

    inline constexpr float16_t operator * (float16_t a, float16_t b) noexcept 
    {
        return float(a) * float(b);
    }

    inline constexpr float16_t operator / (float16_t a, float16_t b) noexcept 
    {
        return float(a) / float(b);
    }

    inline constexpr bool operator == (float16_t a, float16_t b) noexcept 
    {
        return float(a) == float(b);
    }

    inline constexpr bool operator != (float16_t a, float16_t b) noexcept 
    {
        return float(a) != float(b);
    }

    inline constexpr bool operator < (float16_t a, float16_t b) noexcept 
    {
        return float(a) <  float(b);
    }

    inline constexpr bool operator > (float16_t a, float16_t b) noexcept 
    {
        return float(a) >  float(b);
    }
}