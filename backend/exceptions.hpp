#pragma once

#include <pbrlib/exceptions.hpp>

#include <vulkan/vulkan.h>

#include <string_view>
#include <source_location>

namespace pbrlib::backend::exception
{
    class UndefinedPixelFormat final :
        public pbrlib::exception::Exception
    {
    public:
        explicit UndefinedPixelFormat(VkFormat format, const std::source_location location = std::source_location::current());
        explicit UndefinedPixelFormat(std::string_view msg, const std::source_location location = std::source_location::current());
    };
}
