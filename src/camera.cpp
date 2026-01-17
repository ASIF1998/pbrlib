#include <pbrlib/camera.hpp>
#include <pbrlib/transforms.hpp>

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

    void Camera::range(float near_dist, float far_dist)
    {
        _range.near_dist    = near_dist;
        _range.far_dist     = far_dist;
    }

    void Camera::fovY(float fov_y)
    {
        _fov_y = fov_y;
    }

    void Camera::width(uint32_t w)
    {
        _width = w;
    }

    void Camera::height(uint32_t h)
    {
        _height = h;
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

    uint32_t Camera::width() const noexcept
    {
        return _width;
    }

    uint32_t Camera::height() const noexcept
    {
        return _height;
    }

    pbrlib::math::mat4 Camera::view() const
    {
        return pbrlib::transforms::lookAt(_pos, _eye, _up);
    }

    pbrlib::math::mat4 Camera::projection() const
    {
        return pbrlib::transforms::perspective(_fov_y, _width, _height, _range.near_dist, _range.far_dist);
    }

    void Camera::translate(const math::vec3& t)
    {
        _eye = _eye + t;
        _pos = _pos + t;
    }

    void Camera::moveEye(const math::vec3& t)
    {
        _eye = _pos + t;
    }
}
