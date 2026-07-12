#pragma once

#include <pbrlib/exceptions.hpp>
#include <string_view>
#include <vulkan/vulkan.h>

namespace pbrlib::backend::exception
{
    class UndefinedPixelFormat final :
        public pbrlib::exception::Exception
    {
    public:
        explicit UndefinedPixelFormat(VkFormat format);
        explicit UndefinedPixelFormat(std::string_view msg);
    };
}
