namespace pbrlib::math
{
    template<typename Type>
    inline constexpr Vec2<Type>::Vec2() :
        x{static_cast<Type>(0)},
        y{static_cast<Type>(0)}
    {}

    template<typename Type>
    inline constexpr Vec2<Type>::Vec2(Type xy) :
        x{xy},
        y{xy}
    {}

    template<typename Type>
    inline constexpr Vec2<Type>::Vec2(Type x, Type y) :
        x{x},
        y{y}
    {}

    template<typename Type>
    inline bool Vec2<Type>::operator == (const Vec2<Type>& v) const
    {
        return x == v.x && y == v.y;
    }

    template<typename Type>
    inline bool Vec2<Type>::operator != (const Vec2<Type>& v) const
    {
        return x != v.x || y != v.y;
    }

    template<typename Type>
    inline Vec2<Type> Vec2<Type>::operator + (const Vec2<Type>& v) const
    {
        return Vec2<Type>(x + v.x, y + v.y);
    }

    template<typename Type>
    inline Vec2<Type> Vec2<Type>::operator - (const Vec2<Type>& v) const
    {
        return Vec2<Type>(x - v.x, y - v.y);
    }

    template<typename Type>
    inline Vec2<Type> Vec2<Type>::operator * (Type s) const
    {
        return Vec2<Type>(x * s, y * s);
    }

    template<typename Type>
    inline Vec2<Type>& Vec2<Type>::operator += (const Vec2<Type>& v)
    {
        x += v.x;
        y += v.y;

        return *this;
    }

    template<typename Type>
    inline Vec2<Type>& Vec2<Type>::operator -= (const Vec2<Type>& v)
    {
        x -= v.x;
        y -= v.y;
        
        return *this;
    }

    template<typename Type>
    inline Vec2<Type>& Vec2<Type>::operator *= (Type s)
    {
        x *= s;
        y *= s;

        return *this;
    }

    template<typename Type>
    inline Type& Vec2<Type>::operator [] (size_t i) noexcept
    {
        return xy[i];
    }

    template<typename Type>
    inline Type Vec2<Type>::operator [] (size_t i) const noexcept
    {
        return xy[i];
    }

    template<typename Type>
    inline Type Vec2<Type>::lengthSquared() const
    {
        return x * x + y * y;
    }

    template<typename Type>
    inline Type Vec2<Type>::length() const
    {
        return sqrt(x * x + y * y);
    }

    template<typename Type>
    void Vec2<Type>::normalize()
    {
        auto l = length();

        assert(l != static_cast<Type>(0));

        x /= l;
        y /= l;
    }

    template<typename Type>
    Vec2<Type> normalize(const Vec2<Type>& v)
    {
        auto l = v.length();
        assert(l != static_cast<Type>(0));

        return Vec2<Type>(v.x / l, v.y / l);
    }
}
