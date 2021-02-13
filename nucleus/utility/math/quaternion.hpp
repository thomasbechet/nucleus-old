#pragma once

#include <nucleus/utility/math/vector.hpp>
#include <nucleus/utility/math/matrix.hpp>

namespace nu
{
    class Quaternionf
    {
    public:
        union{struct{float x; float y; float z; float w;}; nu_quatf_t data;};
        Quaternionf(){nu_quatf_identity(data);}
        Quaternionf(const nu_quatf_t q){nu_quatf_copy(q, data);}
        Quaternionf(float angle, const Vector3f &axis){nu_quatf_from_axis(angle, axis, data);}
    
    public:
        Quaternionf operator*(const Quaternionf &q)const{Quaternionf dest; nu_quatf_mul(data, q, dest.data); return dest;}
        Quaternionf &operator*=(const Quaternionf &q){nu_quatf_mul(data, q, data); return *this;}
        Vector3f operator*(const Vector3f &v)const{Vector3f dest; nu_quatf_mulv(data, v, dest.data); return dest;}
        operator const nu_quatf_t&()const{return data;}

        static Quaternionf identity(){return Quaternionf();}
        static Matrix4f toMatrix(const Quaternionf &q){Matrix4f m; nu_quatf_to_mat4(q, m.data); return m;}
    };
}