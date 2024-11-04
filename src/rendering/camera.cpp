#include <pbrlib/rendering/camera.hpp>
#include <pbrlib/moving/transform.hpp>

namespace pbrlib
{
    void Camera::pos(const pbrlib::math::vec3& pos)
    {
        _pos = pos;
    }

    void Camera::eye(const pbrlib::math::vec3& eye)
    {
        _eye = eye;
    }
    
    void Camera::up(const pbrlib::math::vec3& up)
    {
        _up = up;
    }

    void Camera::range(float near, float far)
    {
        _range.near = near;
        _range.far  = far;
    }

    void Camera::fovY(float fov_y)
    {
        _fov_y = fov_y;
    }

    void Camera::aspect(float aspect)
    {
        _aspect = aspect;
    }

    const pbrlib::math::vec3& Camera::pos() const noexcept
    {
        return _pos;
    }
    
    const pbrlib::math::vec3& Camera::eye() const noexcept
    {
        return _eye;
    }
    
    const pbrlib::math::vec3& Camera::up() const noexcept
    {
        return _up;
    }

    const Camera::Range& Camera::range() const noexcept
    {
        return _range;
    }

    float Camera::fovY() const noexcept
    {
        return _fov_y;
    }

    float Camera::aspect() const noexcept
    {
        return _aspect;
    }

    pbrlib::math::mat4 Camera::view() const
    {
        return pbrlib::Transform::lookAt(_pos, _eye, _up).getMatrix();
    }

    pbrlib::math::mat4 Camera::projection() const
    {
        return pbrlib::Transform::perspective(_fov_y, _aspect, _range.near, _range.far).getMatrix();
    }
}