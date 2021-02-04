#pragma once

#include "vector.hpp"

namespace nu
{
    class Matrix4f
    {
    public:
        union{nu_mat4f_t data;};
        Matrix4f(){nu_mat4f_identity(data);}
        Matrix4f(const nu_mat4f_t m){nu_mat4f_copy(m, data);}

    public:
        Matrix4f operator*(const Matrix4f &m)const{Matrix4f dest; nu_mat4f_mul(m.data, data, dest.data); return dest;}
        Vector4f operator*(const Vector4f &v)const{Vector4f dest; nu_mat4f_mulv(data, v.data, dest.data); return dest;}
        Vector3f operator*(const Vector3f &v)const{Vector3f dest; nu_mat3f_t mat; nu_mat4f_pick_mat3(data, mat); nu_mat3f_mulv(mat, v.data, dest.data); return dest;}
        operator const nu_mat4f_t&()const{return data;}

        static Matrix4f identity(){return Matrix4f();}
        static Matrix4f translate(const Vector3f &v){Matrix4f m; nu_translate(m.data, v.data); return m;}
        static Matrix4f rotate(float angle, const Vector3f &axis){Matrix4f m; nu_rotate(m.data, angle, axis.data); return m;}
        static Matrix4f scale(const Vector3f &s){Matrix4f m; nu_scale(m.data, s.data); return m;}
        static Matrix4f lookAt(const Vector3f &eye, const Vector3f &center, const Vector3f &up){Matrix4f m; nu_lookat(eye.data, center.data, up.data, m.data); return m;}
    };
}