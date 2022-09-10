#pragma once

#include <cmath>
#include <iostream>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

namespace sf
{
    constexpr float PI = 3.14159265359f;
    constexpr float PI_2 = PI * 2.0f;
    constexpr float PI_OVER_2 = PI * 0.5f;
    constexpr float PI_OVER_4 = PI * 0.25f;
    constexpr float PI_OVER_8 = PI * 0.125f;

    template<typename T>
    std::ostream& operator<<(std::ostream& os, sf::Vector2<T> v)
    {
        os << '[' << v.x << ' ' << v.y << ']';
        return os;
    }
    
    template<typename T>
    std::ostream& operator<<(std::ostream& os, sf::Vector3<T> v)
    {
        os << '[' << v.x << ' ' << v.y << ' ' << v.z << ']';
        return os;
    }

    template<typename T>
    Vector2<T> operator*(Vector2<T> a, Vector2<T> b)
    {
        return Vector2<T> { a.x * b.x, a.y * b.y };
    }

    template<typename T>
    Vector2<T> operator/(const Vector2<T>& a, const Vector2<T>& b)
    {
        return Vector2<T> { a.x / b.x, a.y / b.y };
    }

    template<typename T>
    Vector3<T> operator*(Vector3<T> a, Vector3<T> b)
    {
        return Vector3<T> { a.x * b.x, a.y * b.y, a.z * b.z };
    }

    template<typename T>
    Vector3<T> operator/(const Vector3<T>& a, const Vector3<T>& b)
    {
        return Vector3<T> { a.x / b.x, a.y / b.y, a.z / b.z };
    }

    template<typename T>
    T lerp(T a, T b, T step)
    {
        return a + (b - a) * step;
    }

    template<typename T>
    Vector2<T> lerp(Vector2<T> a, Vector2<T> b, T step)
    {
        return a + (b - a) * step;
    }

    template<typename T>
    Vector3<T> lerp(Vector3<T> a, Vector3<T> b, T step)
    {
        return a + (b - a) * step;
    }

    template<typename T>
    T lerpAngle(T a, T b, T step)
    {
        if(std::abs(a - b) > PI)
        {
            if(b < 0) b += PI_2;
            else      a += PI_2;
        }

        return a + (b - a) * step;
    }

    template<typename T>
    T dot(Vector2<T> a, Vector2<T> b)
    {
        return a.x * b.x + a.y * b.y;
    }

    template<typename T>
    T dot(Vector3<T> a, Vector3<T> b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    template<typename T>
    T length(Vector2<T> v)
    {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    template<typename T>
    T length(Vector3<T> v)
    {
        return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    template<typename T>
    T lengthSquared(const Vector2<T>& v)
    {
        return v.x * v.x + v.y * v.y;
    }

    template<typename T>
    T lengthSquared(Vector3<T> v)
    {
        return v.x * v.x + v.y * v.y + v.z * v.z;
    }

    template<typename T>
    void normalize(Vector2<T>& v)
    {
        T l = length(v);

        if (l != 0)
            v /= l;
    }

    template<typename T>
    void normalize(Vector3<T>& v)
    {
        T l = length(v);

        if (l != 0)
            v /= l;
    }

    template<typename T>
    Vector2<T> getNormalized(Vector2<T> v)
    {
        T l = length(v);

        if (l != 0)
            v /= l;

        return v;
    }

    template<typename T>
    Vector3<T> getNormalized(Vector3<T> v)
    {
        T l = length(v);

        if (l != 0)
            v /= l;

        return v;
    }

    template<typename T>
    sf::Vector2<T> cross(Vector2<T> v)
    {
        return sf::Vector2<T>
        {
            -v.y,
             v.x
        };
    }

    template<typename T>
    T cross(Vector2<T> a, Vector2<T> b)
    {
        return sf::Vector2<T> { a.x * b.y - a.y * b.x };
    }

    template<typename T>
    sf::Vector3<T> cross(Vector3<T> a, Vector3<T> b)
    {
        return sf::Vector3<T>
        {
            a.y * b.z - b.y * a.z,
            a.z * b.x - b.z * a.x,
            a.x * b.y - b.x * a.y
        };
    }

    template<typename T>
    sf::Vector2<T> tripleCross2D(Vector2<T> a, Vector2<T> b, Vector2<T> c)
    {
        T z = a.x * b.y - b.x * a.y;

        return sf::Vector2<T>
        {
            -z * c.y,
             z * c.x
        };
    }

    template<typename T>
    sf::Vector2<T> reflect(Vector2<T> v, Vector2<T> normal)
    {
        return v + (normal * 2.0f * -sf::dot(v, normal));
    }

    template<typename T>
    sf::Vector3<T> reflect(Vector3<T> v, Vector3<T> normal)
    {
        return v + (normal * 2.0f * -sf::dot(v, normal));
    }

    template<typename T>
    sf::Vector2<T> rotate(sf::Vector2<T> v, float angle)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);

        return sf::Vector2f 
        {
            v.x * c + v.y * -s,
            v.x * s + v.y *  c
        };
    }

    template<typename T>
    sf::Vector2<T> rotate(sf::Vector2<T> v, float c, float s)
    {
        return sf::Vector2f
        {
            v.x * c + v.y * -s,
            v.x * s + v.y * c
        };
    }

    template<typename T>
    T clamp(T value, T min, T max)
    {
        if(value < min)
            return min;

        if(value > max)
            return max;

        return value;
    }

    template<typename T>
    sf::Vector2<T> clamp(sf::Vector2<T> value, sf::Vector2<T> min, sf::Vector2<T> max)
    {
        value.x = clamp(value.x, min.x, max.x);
        value.y = clamp(value.y, min.y, max.y);

        return value;
    }

    template<typename T>
    inline constexpr T radians(T angle)
    {
        return angle * PI / 180.0f;
    }

    template<typename T>
    inline constexpr T degrees(T angle)
    {
        return angle * 180.0f / PI;
    }

    inline constexpr float sign(float value)
    {
        return (value < 0.0f) ? -1.0f : 1.0f;
    }
};

inline constexpr float operator"" _ToRadians(long double angle)
{
    return static_cast<float>(angle) * sf::PI / 180.0f;
}

inline constexpr float operator"" _ToDegrees(long double angle)
{
    return static_cast<float>(angle) * 180.0f / sf::PI;
}