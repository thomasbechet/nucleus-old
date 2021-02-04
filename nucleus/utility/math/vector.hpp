#pragma once

#include <nucleus/nucleus.h>

#define NU_VECTOR_BASIC_OPERATORS(cxx_vec, c_vec, scalar) \
    cxx_vec operator+(const cxx_vec& vec)const{cxx_vec dest; c_vec##_add(data, vec, dest.data); return dest;} \
    cxx_vec operator+(const scalar& factor)const{cxx_vec dest; c_vec##_adds(data, factor, dest.data); return dest;} \
    cxx_vec operator-(const cxx_vec& vec)const{cxx_vec dest; c_vec##_sub(data, vec, dest.data); return dest;} \
    cxx_vec operator-(const scalar& factor)const{cxx_vec dest; c_vec##_subs(data, factor, dest.data); return dest;} \
    cxx_vec operator*(const cxx_vec& vec)const{cxx_vec dest; c_vec##_mul(data, vec, dest.data); return dest;} \
    cxx_vec operator*(const scalar& factor)const{cxx_vec dest; c_vec##_muls(data, factor, dest.data); return dest;} \
    cxx_vec operator/(const cxx_vec& vec)const{cxx_vec dest; c_vec##_div(data, vec, dest.data); return dest;} \
    cxx_vec operator/(const scalar& factor)const{cxx_vec dest; c_vec##_divs(data, factor, dest.data); return dest;} \
    cxx_vec operator+=(const cxx_vec& vec){c_vec##_add(data, vec, data); return *this;} \
    cxx_vec operator+=(const scalar& factor){c_vec##_adds(data, factor, data); return *this;} \
    cxx_vec operator-=(const cxx_vec& vec){c_vec##_sub(data, vec, data); return *this;} \
    cxx_vec operator-=(const scalar& factor){c_vec##_subs(data, factor, data); return *this;} \
    cxx_vec operator*=(const cxx_vec& vec){c_vec##_mul(data, vec, data); return *this;} \
    cxx_vec operator*=(const scalar& factor){c_vec##_muls(data, factor, data); return *this;} \
    cxx_vec operator/=(const cxx_vec& vec){c_vec##_div(data, vec, data); return *this;} \
    cxx_vec operator/=(const scalar& factor){c_vec##_divs(data, factor, data); return *this;} \
    cxx_vec operator-(){c_vec##_muls(data, -1, data); return *this;} \
    operator const c_vec##_t&()const{return data;}

namespace nu
{
    class Vector2f;
    class Vector2i;
    class Vector2u;
    class Vector3f;
    class Vector4f;

    class Vector2f
    {
    public:
        union{struct{float x; float y;}; nu_vec2f_t data;};
        Vector2f(){nu_vec2f_zero(data);}
        Vector2f(const nu_vec2f_t v){nu_vec2f_copy(v, data);}
        Vector2f(float s){this->x = s; this->y = s;}
        Vector2f(float x, float y){this->x = x; this->y = y;}
        NU_VECTOR_BASIC_OPERATORS(Vector2f, nu_vec2f, float)

    public:
        static float length(const Vector2f &vec){return nu_vec2f_length(vec);}
        static void normalize(Vector2f &vec){nu_vec2f_normalize(vec.data);}
        float length(){return Vector2f::length(*this);}
        void normalize(){Vector2f::normalize(*this);}
    };

    class Vector2i
    {
    public:
        union{struct{int32_t x; int32_t y;}; nu_vec2i_t data;};
        Vector2i(){nu_vec2i_zero(data);}
        Vector2i(const nu_vec2i_t v){nu_vec2i_copy(v, data);}
        Vector2i(int32_t x, int32_t y){this->x = x; this->y = y;}
        NU_VECTOR_BASIC_OPERATORS(Vector2i, nu_vec2i, int32_t)
    };

    class Vector2u
    {
    public:
        union{struct{uint32_t x; uint32_t y;}; nu_vec2u_t data;};
        Vector2u(){nu_vec2u_zero(data);}
        Vector2u(const nu_vec2u_t v){nu_vec2u_copy(v, data);}
        Vector2u(uint32_t x, uint32_t y){this->x = x; this->y = y;}
        NU_VECTOR_BASIC_OPERATORS(Vector2u, nu_vec2u, uint32_t)
    };

    class Vector3f
    {
    public:
        union{struct{float x; float y; float z;}; nu_vec3f_t data;};
        Vector3f(){nu_vec3f_zero(data);}
        Vector3f(const nu_vec3f_t v){nu_vec3f_copy(v, data);}
        Vector3f(float s){this->x = s; this->y = s; this->z = s;}
        Vector3f(float x, float y, float z){this->x = x; this->y = y; this->z = z;}
        NU_VECTOR_BASIC_OPERATORS(Vector3f, nu_vec3f, float)

    public:
        Vector3f(const Vector2f &v, float z = 0.0f){this->x = v.x; this->y = v.y; this->z = z;}

    public:
        static Vector3f normalize(const Vector3f &vec){Vector3f v = vec; nu_vec3f_normalize(v.data); return v;}
        static Vector3f lerp(const Vector3f &from, const Vector3f &to, float t){Vector3f dest; nu_vec3f_lerp(from, to, t, dest.data); return dest;}
        float length(){return nu_vec3f_length(*this);}
        void normalize(){*this = Vector3f::normalize(*this);}
        void lerp(const Vector3f &to, float t){*this = Vector3f::lerp(*this, to, t);}

    public:
        static Vector3f forward()  {return Vector3f(0.0f,  0.0f, -1.0f);}
        static Vector3f backward() {return Vector3f(0.0f,  0.0f,  1.0f);}
        static Vector3f left()     {return Vector3f(-1.0f,  0.0f, 0.0f);}
        static Vector3f right()    {return Vector3f(1.0f,  0.0f,  0.0f);}
        static Vector3f up()       {return Vector3f(0.0f,  1.0f,  0.0f);}
        static Vector3f down()     {return Vector3f(0.0f, -1.0f,  0.0f);}
    };

    class Vector4f
    {
    public:
        union{struct{float x; float y; float z; float w;}; nu_vec4f_t data;};
        Vector4f(){nu_vec4f_zero(data);}
        Vector4f(const nu_vec4f_t v){nu_vec4f_copy(v, data);}
        Vector4f(float s){this->x = s; this->y = s; this->z = s; this->w = s;}
        Vector4f(float x, float y, float z, float w){this->x = x; this->y = y; this->z = z; this->w = w;}
        NU_VECTOR_BASIC_OPERATORS(Vector4f, nu_vec4f, float)

    public:
        Vector4f(const Vector3f &v, float w = 0.0f){this->x = v.x; this->y = v.y; this->z = v.z; this->w = w;}
    };
}