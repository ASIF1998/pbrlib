//
//  Image.c
//  PBRLib
//
//  Created by Асиф Мамедов on 04/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <type_traits>

#include "../../Util/enumCast.hpp"

#include "CommandBuffer.hpp"

#include "Buffer.hpp"

namespace pbrlib
{
    template<Image::TexelType TexType, typename Type, Image::NumBits NBits>
    struct FormatDefinition;

    template<>
    struct FormatDefinition<Image::TexelType::R, float, Image::NumBits::NB16>
    {
        inline static VkFormat getFormat() noexcept
        {
            return VK_FORMAT_R16_SFLOAT;
        }

        inline static uint32_t getNumComponents() noexcept
        {
            return 1;
        }
    };

    template<>
    struct FormatDefinition<Image::TexelType::RG, float, Image::NumBits::NB16>
    {
        inline static VkFormat getFormat() noexcept
        {
            return VK_FORMAT_R16G16_SFLOAT;
        }

        inline static uint32_t getNumComponents() noexcept
        {
            return 2;
        }
    };

    template<>
    struct FormatDefinition<Image::TexelType::RGB, float, Image::NumBits::NB16>
    {
        inline static VkFormat getFormat() noexcept
        {
            return VK_FORMAT_R16G16B16_SFLOAT;
        }

        inline static uint32_t getNumComponents() noexcept
        {
            return 3;
        }
    };

    template<>
    struct FormatDefinition<Image::TexelType::RGBA, float, Image::NumBits::NB16>
    {
        inline static VkFormat getFormat() noexcept
        {
            return VK_FORMAT_R16G16B16A16_SFLOAT;
        }

        inline static uint32_t getNumComponents() noexcept
        {
            return 4;
        }
    };

    template<>
    struct FormatDefinition<Image::TexelType::R, float, Image::NumBits::NB32>
    {
        inline static VkFormat getFormat() noexcept
        {
            return VK_FORMAT_R32_SFLOAT;
        }

        inline static uint32_t getNumComponents() noexcept
        {
            return 1;
        }
    };

    template<>
    struct FormatDefinition<Image::TexelType::RG, float, Image::NumBits::NB32>
    {
        inline static VkFormat getFormat() noexcept
        {
            return VK_FORMAT_R32G32_SFLOAT;
        }

        inline static uint32_t getNumComponents() noexcept
        {
            return 2;
        }
    };

    template<>
    struct FormatDefinition<Image::TexelType::RGB, float, Image::NumBits::NB32>
    {
        inline static VkFormat getFormat() noexcept
        {
            return VK_FORMAT_R32G32B32_SFLOAT;
        }

        inline static uint32_t getNumComponents() noexcept
        {
            return 3;
        }
    };

    template<>
    struct FormatDefinition<Image::TexelType::RGBA, float, Image::NumBits::NB32>
    {
        inline static VkFormat getFormat() noexcept
        {
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        }

        inline static uint32_t getNumComponents() noexcept
        {
            return 4;
        }
    };

    template<>
    struct FormatDefinition<Image::TexelType::R, uint8_t, Image::NumBits::NB8>
    {
        inline static VkFormat getFormat() noexcept
        {
            return VK_FORMAT_R8_UINT;
        }

        inline static uint32_t getNumComponents() noexcept
        {
            return 1;
        }
    };

    template<>
    struct FormatDefinition<Image::TexelType::RG, uint8_t, Image::NumBits::NB8>
    {
        inline static VkFormat getFormat() noexcept
        {
            return VK_FORMAT_R8G8_SRGB;
        }

        inline static uint32_t getNumComponents() noexcept
        {
            return 2;
        }
    };

    template<>
    struct FormatDefinition<Image::TexelType::RGB, uint8_t, Image::NumBits::NB8>
    {
        inline static VkFormat getFormat() noexcept
        {
            return VK_FORMAT_R8G8B8_SRGB;
        }

        inline static uint32_t getNumComponents() noexcept
        {
            return 3;
        }
    };

    template<>
    struct FormatDefinition<Image::TexelType::RGBA, uint8_t, Image::NumBits::NB8>
    {
        inline static VkFormat getFormat() noexcept
        {
            return VK_FORMAT_R8G8B8A8_SRGB;
        }

        inline static uint32_t getNumComponents() noexcept
        {
            return 4;
        }
    };

    template<>
    struct FormatDefinition<Image::TexelType::Depth, float, Image::NumBits::NB32>
    {
        inline static VkFormat getFormat() noexcept
        {
            return VK_FORMAT_D32_SFLOAT;
        }

        inline static uint32_t getNumComponents() noexcept
        {
            return 1;
        }
    };

    template<>
    struct FormatDefinition<Image::TexelType::DepthStencil, float, Image::NumBits::NB32>
    {
        inline static VkFormat getFormat() noexcept
        {
            return VK_FORMAT_D32_SFLOAT_S8_UINT;
        }

        inline static uint32_t getNumComponents() noexcept
        {
            return 1;
        }
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<Image::TexelType TexType, typename Type, Image::NumBits NBits>
    Image::Builder<TexType, Type, NBits>::Builder()
    {
        _image_info.format = FormatDefinition<TexType, Type, NBits>::getFormat();
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits>
    Image::Builder<TexType, Type, NBits>::~Builder()
    {}

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits>
    inline void Image::Builder<TexType, Type, NBits>::setExtend(
        uint32_t width,
        uint32_t height,
        uint32_t depth
    )  noexcept
    {
        _image_info.image_extend.width  = width;
        _image_info.image_extend.height = height;
        _image_info.image_extend.depth  = depth;
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits>
    inline void Image::Builder<TexType, Type, NBits>::setExtend(const VkExtent3D& extend)  noexcept
    {
        _image_info.image_extend = extend;
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits>
    inline void Image::Builder<TexType, Type, NBits>::setNumSamples(
        VkSampleCountFlagBits num_samples
    ) noexcept
    {
        _image_info.num_samples = num_samples;
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits>
    inline void Image::Builder<TexType, Type, NBits>::setTiling(VkImageTiling tiling) noexcept
    {
        _image_info.tiling = tiling;
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits>
    inline void Image::Builder<TexType, Type, NBits>::setUsage(VkImageUsageFlags usage) noexcept
    {
        _image_info.usage = usage;
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits>
    inline void Image::Builder<TexType, Type, NBits>::setNumMipLevels(uint32_t mip_levels) noexcept
    {
        _image_info.mip_levels = mip_levels;
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits>
    inline void Image::Builder<TexType, Type, NBits>::setImageType(VkImageType image_type) noexcept
    {
        _image_info.image_type = image_type;
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits>
    inline void Image::Builder<TexType, Type, NBits>::setNumArrayLayers(uint32_t array_layers) noexcept
    {
        _image_info.array_layers = array_layers;
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits>
    inline void Image::Builder<TexType, Type, NBits>::setDevice(const Device* ptr_device) noexcept
    {
        _ptr_device = ptr_device;
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits>
    inline void Image::Builder<TexType, Type, NBits>::setMemoryTypeIndex(uint32_t memory_type_index) noexcept
    {
        _memory_type_index = memory_type_index;
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits>
    inline void Image::Builder<TexType, Type, NBits>::addQueueFamilyIndex(uint32_t queue_family_index) noexcept
    {
        _queue_family_indicies.push_back(queue_family_index);
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits>
    inline Image Image::Builder<TexType, Type, NBits>::build() const
    {
        if (_queue_family_indicies.size() == 1) {
            return Image(
                _ptr_device,
                _memory_type_index,
                _image_info,
                _queue_family_indicies[0]
            );
        }

        return Image(
            _ptr_device,
            _memory_type_index,
            _image_info,
            _queue_family_indicies
        );
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits>
    inline std::unique_ptr<Image> Image::Builder<TexType, Type, NBits>::buildPtr() const
    {
        if (_queue_family_indicies.size() == 1) {
            return std::make_unique<Image>(
                _ptr_device,
                _memory_type_index,
                _image_info,
                _queue_family_indicies[0]
            );
        }

        return std::make_unique<Image>(
            _ptr_device,
            _memory_type_index,
            _image_info,
            _queue_family_indicies
        );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::BuilderWithData() : 
        Builder<TexType, Type, NBits>()
    {
        Builder<TexType, Type, NBits>::_image_info.format = FormatDefinition<TexType, Type, NBits>::getFormat();
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::BuilderWithData(
        uint32_t width,
        uint32_t height,
        uint32_t depth
    ) :
        Builder<TexType, Type, NBits>()
    {
        _image_info.format = FormatDefinition<TexType, Type, NBits>::getFormat();

        Builder<TexType, Type, NBits>::_image_info.image_extend.width   = width;
        Builder<TexType, Type, NBits>::_image_info.image_extend.height  = height;
        Builder<TexType, Type, NBits>::_image_info.image_extend.depth   = depth;

        _data.resize(
            width   *
            height  *
            depth   *
            FormatDefinition<TexType, Type, NBits>::getNumComponents()
        );
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::BuilderWithData(
        const VkExtent3D& extend
    ) :
        Builder<TexType, Type, NBits>()
    {
        Builder<TexType, Type, NBits>::_image_info.format       = FormatDefinition<TexType, Type, NBits>::getFormat();
        Builder<TexType, Type, NBits>::_image_info.image_extend = extend;

        _data.resize(
            Builder<TexType, Type, NBits>::_image_info.image_extend.width   *
            Builder<TexType, Type, NBits>::_image_info.image_extend.height  *
            Builder<TexType, Type, NBits>::_image_info.image_extend.depth   *
            FormatDefinition<TexType, Type, NBits>::getNumComponents()
        );
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::~BuilderWithData()
    {}

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::setExtend(uint32_t width, uint32_t height, uint32_t depth) noexcept
    {
        Builder<TexType, Type, NBits>::_image_info.image_extend.width   = width;
        Builder<TexType, Type, NBits>::_image_info.image_extend.height  = height;
        Builder<TexType, Type, NBits>::_image_info.image_extend.depth   = depth;

        _data.resize(
            width   *
            height  *
            depth   *
            FormatDefinition<TexType, Type, NBits>::getNumComponents()
        );
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::setExtend(const VkExtent3D& extend)  noexcept
    {
        Builder<TexType, Type, NBits>::_image_info.image_extend = extend;

        _data.resize(
            Builder<TexType, Type, NBits>::_image_info.image_extend.width   *
            Builder<TexType, Type, NBits>::_image_info.image_extend.height  *
            Builder<TexType, Type, NBits>::_image_info.image_extend.depth   *
            FormatDefinition<TexType, Type, NBits>::getNumComponents()
        );
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    template<typename Container>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::setData(const Container& data)
    {
        assert(_data.size() == data.size());
        memcpy(_data.data(), &data[0], _data.size() * sizeof(Type));
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::setData(const Type* ptr, size_t size)
    {
        _data.resize(size);
        memcpy(_data.data(), ptr, size * sizeof(_data[0]));
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::setDeviceLocalMemoryTypeIndex(uint32_t memory_type_index) noexcept
    {
        _device_local_memory_type_index = memory_type_index;
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::setHostLocalMemoryTypeIndex(uint32_t memory_type_index) noexcept
    {
        Image::Builder<TexType, Type, NBits>::_memory_type_index = memory_type_index;
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::setCommandBuffer(std::shared_ptr<PrimaryCommandBuffer> ptr_command_buffer)
    {
        _ptr_command_buffer = ptr_command_buffer;
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::setDeviceQueue(const DeviceQueue* ptr_device_queue)
    {
        _ptr_queue = ptr_device_queue;
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::pushBack(Type r)
    {
        if constexpr (TexType == TexelType::R) {
            _data.push_back(r);
        } else if constexpr (TexType == TexelType::RG) {
            _data.push_back(r);
            _data.push_back(static_cast<Type>(0));
        } else if constexpr (TexType == TexelType::RGB) {
            _data.push_back(r);
            _data.push_back(static_cast<Type>(0));
            _data.push_back(static_cast<Type>(0));
        } else if constexpr (TexType == TexelType::RGBA) {
            _data.push_back(r);
            _data.push_back(static_cast<Type>(0));
            _data.push_back(static_cast<Type>(0));
            _data.push_back(static_cast<Type>(0));
        }
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::pushBack(const math::Vec2<Type>& c)
    {
        if constexpr (TexType == TexelType::R) {
            _data.push_back(c.r);
        } else if constexpr (TexType == TexelType::RG) {
            _data.push_back(c.r);
            _data.push_back(c.g);
        } else if constexpr (TexType == TexelType::RGB) {
            _data.push_back(c.r);
            _data.push_back(c.g);
            _data.push_back(static_cast<Type>(0));
        } else if constexpr (TexType == TexelType::RGBA) {
            _data.push_back(c.r);
            _data.push_back(c.g);
            _data.push_back(static_cast<Type>(0));
            _data.push_back(static_cast<Type>(0));
        }
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::pushBack(const math::Vec3<Type>& c)
    {
        if constexpr (TexType == TexelType::R) {
            _data.push_back(c.r);
        } else if constexpr (TexType == TexelType::RG) {
            _data.push_back(c.r);
            _data.push_back(c.g);
        } else if constexpr (TexType == TexelType::RGB) {
            _data.push_back(c.r);
            _data.push_back(c.g);
            _data.push_back(c.b);
        } else if constexpr (TexType == TexelType::RGBA) {
            _data.push_back(c.r);
            _data.push_back(c.g);
            _data.push_back(c.b);
            _data.push_back(static_cast<Type>(0));
        }
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::pushBack(const math::Vec4<Type>& c)
    {
        if constexpr (TexType == TexelType::R) {
            _data.push_back(c.r);
        } else if constexpr (TexType == TexelType::RG) {
            _data.push_back(c.r);
            _data.push_back(c.g);
        } else if constexpr (TexType == TexelType::RGB) {
            _data.push_back(c.r);
            _data.push_back(c.g);
            _data.push_back(c.b);
        } else if constexpr (TexType == TexelType::RGBA) {
            _data.push_back(c.r);
            _data.push_back(c.g);
            _data.push_back(c.b);
            _data.push_back(c.a);
        }
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::set(size_t i, Type r)
    {
        if constexpr (TexType == TexelType::R) {
            assert(i < _data.size());
            _data[i] = r;
        } else if constexpr (TexType == TexelType::RG) {
            assert(i < _data.size() / sizeof(math::Vec2<Type>));
            math::Vec2<Type>* ptr = reinterpret_cast<math::Vec2<Type>*>(_data.data());
            ptr[i].r = r;
        } else if constexpr (TexType == TexelType::RGB) {
            assert(i < _data.size() / sizeof(math::Vec3<Type>));
            math::Vec3<Type>* ptr = reinterpret_cast<math::Vec3<Type>*>(_data.data());
            ptr[i].r = r;
        } else if constexpr (TexType == TexelType::RGBA) {
            assert(i < _data.size() / sizeof(math::Vec4<Type>));
            math::Vec4<Type>* ptr = reinterpret_cast<math::Vec4<Type>*>(_data.data());
            ptr[i].r = r;
        }
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::set(size_t i, const math::Vec2<Type>& c)
    {
        if constexpr (TexType == TexelType::R) {
            assert(i < _data.size());
            _data[i] = c.r;
        } else if constexpr (TexType == TexelType::RG) {
            assert(i < _data.size() / sizeof(math::Vec2<Type>));
            math::Vec2<Type>* ptr = reinterpret_cast<math::Vec2<Type>*>(_data.data());

            ptr[i] = c;
        } else if constexpr (TexType == TexelType::RGB) {
            assert(i < _data.size() / sizeof(math::Vec3<Type>));
            math::Vec3<Type>* ptr = reinterpret_cast<math::Vec3<Type>*>(_data.data());

            ptr[i].r = c.r;
            ptr[i].g = c.g;
        } else if constexpr (TexType == TexelType::RGBA) {
            assert(i < _data.size() / sizeof(math::Vec4<Type>));
            math::Vec4<Type>* ptr = reinterpret_cast<math::Vec4<Type>*>(_data.data());

            ptr[i].r = c.r;
            ptr[i].g = c.g;
        }
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::set(size_t i, const math::Vec3<Type>& c)
    {
        if constexpr (TexType == TexelType::R) {
            assert(i < _data.size());
            _data[i] = c.r;
        } else if constexpr (TexType == TexelType::RG) {
            assert(i < _data.size() / sizeof(math::Vec2<Type>));
            math::Vec2<Type>* ptr = reinterpret_cast<math::Vec2<Type>*>(_data.data());

            ptr[i].r = c.r;
            ptr[i].g = c.g;
        } else if constexpr (TexType == TexelType::RGB) {
            assert(i < _data.size() / sizeof(math::Vec3<Type>));
            math::Vec3<Type>* ptr = reinterpret_cast<math::Vec3<Type>*>(_data.data());

            ptr[i] = c;
        } else if constexpr (TexType == TexelType::RGBA) {
            assert(i < _data.size() / sizeof(math::Vec4<Type>));
            math::Vec4<Type>* ptr = reinterpret_cast<math::Vec4<Type>*>(_data.data());

            ptr[i].r = c.r;
            ptr[i].g = c.g;
            ptr[i].b = c.b;
        }
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::set(size_t i, const math::Vec4<Type>& c)
    {
        if constexpr (TexType == TexelType::R) {
            assert(i < _data.size());
            _data[i] = c.r;
        } else if constexpr (TexType == TexelType::RG) {
            assert(i < _data.size() / sizeof(math::Vec2<Type>));
            math::Vec2<Type>* ptr = reinterpret_cast<math::Vec2<Type>*>(_data.data());

            ptr[i].r = c.r;
            ptr[i].g = c.g;
        } else if constexpr (TexType == TexelType::RGB) {
            assert(i < _data.size() / sizeof(math::Vec3<Type>));
            math::Vec3<Type>* ptr = reinterpret_cast<math::Vec3<Type>*>(_data.data());

            ptr[i].r = c.r;
            ptr[i].g = c.g;
            ptr[i].b = c.b;
        } else if constexpr (TexType == TexelType::RGBA) {
            assert(i < _data.size() / sizeof(math::Vec4<Type>));
            math::Vec4<Type>* ptr = reinterpret_cast<math::Vec4<Type>*>(_data.data());

            ptr[i] = c;
        }
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::set(size_t i, size_t j, Type r)
    {
        if constexpr (TexType == TexelType::R) {
            assert(!"This type of texel does not support two-dimensional iteration according to data");
        } else if constexpr (TexType == TexelType::RG) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(math::Vec2<Type>));
            math::Vec2<Type>* ptr = reinterpret_cast<math::Vec2<Type>*>(_data.data());

            ptr[i * _image_info.image_extend.width + j].r = r;
        } else if constexpr (TexType == TexelType::RGB) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(math::Vec3<Type>));
            math::Vec3<Type>* ptr = reinterpret_cast<math::Vec3<Type>*>(_data.data());

            ptr[i * _image_info.image_extend.width + j].r = r;
        } else if constexpr (TexType == TexelType::RGBA) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(math::Vec4<Type>));
            math::Vec4<Type>* ptr = reinterpret_cast<math::Vec4<Type>*>(_data.data());

            ptr[i * _image_info.image_extend.width + j].r = r;
        }
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::set(size_t i, size_t j, const math::Vec2<Type>& c)
    {
        if constexpr (TexType == TexelType::R) {
            assert(!"This type of texel does not support two-dimensional iteration according to data");
        } else if constexpr (TexType == TexelType::RG) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(math::Vec2<Type>));
            math::Vec2<Type>* ptr = reinterpret_cast<math::Vec2<Type>*>(_data.data());

            ptr[i * _image_info.image_extend.width + j] = c;
        } else if constexpr (TexType == TexelType::RGB) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(math::Vec3<Type>));
            math::Vec3<Type>* ptr = reinterpret_cast<math::Vec3<Type>*>(_data.data());

            ptr[i * _image_info.image_extend.width + j].r = c.r;
            ptr[i * _image_info.image_extend.width + j].g = c.g;
        } else if constexpr (TexType == TexelType::RGBA) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(math::Vec4<Type>));
            math::Vec4<Type>* ptr = reinterpret_cast<math::Vec4<Type>*>(_data.data());

            ptr[i * _image_info.image_extend.width + j].r = c.r;
            ptr[i * _image_info.image_extend.width + j].g = c.g;
        }
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::set(size_t i, size_t j, const math::Vec3<Type>& c)
    {
        if constexpr (TexType == TexelType::R) {
            assert(!"This type of texel does not support two-dimensional iteration according to data");
        } else if constexpr (TexType == TexelType::RG) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(math::Vec2<Type>));
            math::Vec2<Type>* ptr = reinterpret_cast<math::Vec2<Type>*>(_data.data());

            ptr[i * _image_info.image_extend.width + j].r = c.r;
            ptr[i * _image_info.image_extend.width + j].g = c.g;
        } else if constexpr (TexType == TexelType::RGB) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(math::Vec3<Type>));
            math::Vec3<Type>* ptr = reinterpret_cast<math::Vec3<Type>*>(_data.data());

            ptr[i * _image_info.image_extend.width + j] = c;
        } else if constexpr (TexType == TexelType::RGBA) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(math::Vec4<Type>));
            math::Vec4<Type>* ptr = reinterpret_cast<math::Vec4<Type>*>(_data.data());

            ptr[i * _image_info.image_extend.width + j].r = c.r;
            ptr[i * _image_info.image_extend.width + j].g = c.g;
            ptr[i * _image_info.image_extend.width + j].b = c.b;
        }
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::set(size_t i, size_t j, const math::Vec4<Type>& c)
    {
        if constexpr (TexType == TexelType::R) {
            assert(!"This type of texel does not support two-dimensional iteration according to data");
        } else if constexpr (TexType == TexelType::RG) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(math::Vec2<Type>));
            math::Vec2<Type>* ptr = reinterpret_cast<math::Vec2<Type>*>(_data.data());

            ptr[i * _image_info.image_extend.width + j].r = c.r;
            ptr[i * _image_info.image_extend.width + j].g = c.g;
        } else if constexpr (TexType == TexelType::RGB) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(math::Vec3<Type>));
            math::Vec3<Type>* ptr = reinterpret_cast<math::Vec3<Type>*>(_data.data());

            ptr[i * _image_info.image_extend.width + j].r = c.r;
            ptr[i * _image_info.image_extend.width + j].g = c.g;
            ptr[i * _image_info.image_extend.width + j].b = c.b;
        } else if constexpr (TexType == TexelType::RGBA) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(math::Vec4<Type>));
            math::Vec4<Type>* ptr = reinterpret_cast<math::Vec4<Type>*>(_data.data());

            ptr[i * _image_info.image_extend.width + j] = c;
        }
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline Image Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::build() const
    {
        Image image = Builder<TexType, Type, NBits>::_queue_family_indicies.size() == 1 ?
            Image(
                Builder<TexType, Type, NBits>::_ptr_device,
                _device_local_memory_type_index,
                Builder<TexType, Type, NBits>::_image_info,
                _ptr_queue->getFamilyIndex()
            )
            :
            Image(
                Builder<TexType, Type, NBits>::_ptr_device,
                _device_local_memory_type_index,
                Builder<TexType, Type, NBits>::_image_info,
                _ptr_queue->getFamilyIndex()
            );

        if (Image::Builder<TexType, Type, NBits>::_image_info.tiling == VK_IMAGE_TILING_OPTIMAL) {
            Buffer::BuilderWithData<Type> builder_buffer;

            builder_buffer.setData(_data.data(), _data.size());
            builder_buffer.setUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
            builder_buffer.addQueueFamily(_ptr_queue->getFamilyIndex());
            builder_buffer.setDevice(Builder<TexType, Type, NBits>::_ptr_device);
            builder_buffer.setMemoryTypeIndex(Builder<TexType, Type, NBits>::_memory_type_index);

            Buffer temp_buffer = builder_buffer.build();

            VkImageSubresourceLayers image_subresource_layers = { };
            image_subresource_layers.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
            image_subresource_layers.mipLevel       = 0;
            image_subresource_layers.baseArrayLayer = 0 ;
            image_subresource_layers.layerCount     = 1;

            VkImageSubresourceRange image_subresource_range = { };
            image_subresource_range.aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT;
            image_subresource_range.baseMipLevel    = 0;
            image_subresource_range.levelCount      = 1;
            image_subresource_range.baseArrayLayer  = 0;
            image_subresource_range.layerCount      = 1;

            _ptr_command_buffer->reset();
            _ptr_command_buffer->begin();

            _ptr_command_buffer->imageMemoryBarrier(
                VK_PIPELINE_STAGE_HOST_BIT,
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                0,
                0,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                _ptr_queue->getFamilyIndex(),
                _ptr_queue->getFamilyIndex(),
                image,
                image_subresource_range
            );

            _ptr_command_buffer->copyBufferToImage(
                temp_buffer,
                0,
                image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                image_subresource_layers,
                {0, 0, 0},
                Builder<TexType, Type, NBits>::_image_info.image_extend
            );

            _ptr_command_buffer->imageMemoryBarrier(
                VK_PIPELINE_STAGE_HOST_BIT,
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                0,
                0,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                _ptr_queue->getFamilyIndex(),
                _ptr_queue->getFamilyIndex(),
                image,
                image_subresource_range
            );

            _ptr_command_buffer->end();

            _ptr_queue->submit(*_ptr_command_buffer);
            _ptr_queue->waitIdle();
        }
        else {
            image.getDeviceMemory()->map();
            image.getDeviceMemory()->setData(_data);
            image.getDeviceMemory()->unmap();
        }

        return image;
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline std::unique_ptr<Image> Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::buildPtr() const
    {
        std::unique_ptr<Image> ptr_image;

        if (Builder<TexType, Type, NBits>::_queue_family_indicies.size() == 1) {
            ptr_image = std::make_unique<Image>(
                Builder<TexType, Type, NBits>::_ptr_device,
                _device_local_memory_type_index,
                Builder<TexType, Type, NBits>::_image_info,
                _ptr_queue->getFamilyIndex()
            );
        } else {
            ptr_image = std::make_unique<Image>(
                Builder<TexType, Type, NBits>::_ptr_device,
                _device_local_memory_type_index,
                Builder<TexType, Type, NBits>::_image_info,
                _ptr_queue->getFamilyIndex()
            );
        }

        if (Image::Builder<TexType, Type, NBits>::_image_info.tiling == VK_IMAGE_TILING_OPTIMAL) {
            Buffer::BuilderWithData<Type> builder_buffer;

            builder_buffer.setData(_data.data(), _data.size());
            builder_buffer.setUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
            builder_buffer.addQueueFamily(_ptr_queue->getFamilyIndex());
            builder_buffer.setDevice(Builder<TexType, Type, NBits>::_ptr_device);
            builder_buffer.setMemoryTypeIndex(Builder<TexType, Type, NBits>::_memory_type_index);

            Buffer temp_buffer = builder_buffer.build();

            VkImageSubresourceLayers image_subresource_layers = { };
            image_subresource_layers.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
            image_subresource_layers.mipLevel       = 0;
            image_subresource_layers.baseArrayLayer = 0;
            image_subresource_layers.layerCount     = 1; 

            VkImageSubresourceRange image_subresource_range = { };
            image_subresource_range.aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT;
            image_subresource_range.baseMipLevel    = 0;
            image_subresource_range.levelCount      = 1;
            image_subresource_range.baseArrayLayer  = 0;
            image_subresource_range.layerCount      = 1;

            _ptr_command_buffer->reset();
            _ptr_command_buffer->begin();

            _ptr_command_buffer->imageMemoryBarrier(
                VK_PIPELINE_STAGE_HOST_BIT,
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                0,
                0,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                _ptr_queue->getFamilyIndex(),
                _ptr_queue->getFamilyIndex(),
                *ptr_image,
                image_subresource_range
            );

            _ptr_command_buffer->copyBufferToImage(
                temp_buffer,
                0,
                *ptr_image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                image_subresource_layers,
                { 0, 0, 0 },
                Builder<TexType, Type, NBits>::_image_info.image_extend
            );

            _ptr_command_buffer->imageMemoryBarrier(
                VK_PIPELINE_STAGE_HOST_BIT,
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                0,
                0,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                _ptr_queue->getFamilyIndex(),
                _ptr_queue->getFamilyIndex(),
                *ptr_image,
                image_subresource_range
            );

            _ptr_command_buffer->end();

            _ptr_queue->submit(*_ptr_command_buffer);
            _ptr_queue->waitIdle();
        } else {
            ptr_image->getDeviceMemory()->map();
            ptr_image->getDeviceMemory()->setData(_data);
            ptr_image->getDeviceMemory()->unmap();
        }

        return ptr_image;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline void ImageView::Builder::setImage(std::shared_ptr<const Image> ptr_image)
    {
        _ptr_image = ptr_image;
    }

    inline void ImageView::Builder::setFormat(VkFormat format) noexcept
    {
        _format = format;
    }

    inline void ImageView::Builder::setAspectMask(VkImageAspectFlags aspect_mask) noexcept
    {
        _subresource_range.aspectMask = aspect_mask;
    }

    inline void ImageView::Builder::setBaseMipLevel(uint32_t base_mip_level) noexcept
    {
        _subresource_range.baseMipLevel = base_mip_level;
    }

    inline void ImageView::Builder::setLevelCount(uint32_t level_count) noexcept
    {
        _subresource_range.levelCount = level_count;
    }

    inline void ImageView::Builder::setBaseArrayLayer(
        uint32_t base_array_layer
    ) noexcept
    {
        _subresource_range.baseArrayLayer = base_array_layer;
    }

    inline void ImageView::Builder::setLayerCount(uint32_t layer_count) noexcept
    {
        _subresource_range.layerCount = layer_count;
    }

    inline void ImageView::Builder::setType(VkImageViewType type) noexcept
    {
        _image_view_type = type;
    }

    inline ImageView ImageView::Builder::build() const
    {
        return ImageView(_ptr_image, _format, _subresource_range, _image_view_type);
    }

    inline std::unique_ptr<ImageView> ImageView::Builder::buildPtr() const
    {
        return make_unique<ImageView>(_ptr_image, _format, _subresource_range, _image_view_type);
    }
}

