#include <backend/renderer/frame_graph/filters/filter.hpp>

#include <pbrlib/exceptions.hpp>

namespace pbrlib::backend
{
    Filter::Filter(vk::Device& device) noexcept :
        RenderPass (device)
    { }

    void Filter::apply(vk::Image& image) noexcept
    {
        _ptr_src_image = &image;
    }

    vk::Image& Filter::srcImage()
    {
        if (!_ptr_src_image)
            throw exception::InvalidState("[filter] didn't set source image");
        
        return *_ptr_src_image;
    }
}
