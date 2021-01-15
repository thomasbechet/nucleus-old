#pragma once

#include "vector.hpp"
#include "matrix.hpp"

namespace nu
{
    class Transform
    {
    public:
        void setTranslation(const Vector3f &translation) {m_translation = translation; m_computeRequired = true;}
        Vector3f getTranslation() const {return m_translation;}
        void setRotation(float angle, const Vector3f &axis) {m_rotation = Quaternionf(angle, axis); m_computeRequired = true;}
        Quaternionf getRotation() const {return m_rotation;}
        void setScale(const Vector3f &scale) {m_scale = scale; m_computeRequired = true;}
        Vector3f getScale() const {return m_scale;}

        void rotate(float angle, const Vector3f &axis) {m_rotation *= Quaternionf(angle, axis); m_computeRequired = true;}
        void translate(const Vector3f &translation) {m_translation += translation; m_computeRequired = true;}

        Vector3f getForwardVector() const  {return Vector3f::normalize(m_rotation * Vector3f::forward());}
        Vector3f getBackwardVector() const {return Vector3f::normalize(m_rotation * Vector3f::backward());}
        Vector3f getLeftVector() const     {return Vector3f::normalize(m_rotation * Vector3f::left());}
        Vector3f getRightVector() const    {return Vector3f::normalize(m_rotation * Vector3f::right());}
        Vector3f getUpVector() const       {return Vector3f::normalize(m_rotation * Vector3f::up());}
        Vector3f getDownVector() const     {return Vector3f::normalize(m_rotation * Vector3f::down());}

        Matrix4f getMatrix() const
        {
            if (m_computeRequired) {
                m_transform = Matrix4f::translate(m_translation) * Quaternionf::toMatrix(m_rotation) * Matrix4f::scale(m_scale);
                m_computeRequired = false;
            }

            return m_transform;
        }

    private:
        Vector3f m_translation;
        Quaternionf m_rotation;
        Vector3f m_scale = {1.0f, 1.0f, 1.0f};

        mutable Matrix4f m_transform;
        mutable bool m_computeRequired = true;
    };
}