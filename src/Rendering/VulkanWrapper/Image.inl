//
//  Image.c
//  PBRLib
//
//  Created by Асиф Мамедов on 04/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <type_traits>

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
        _image_info.image_extend.width     = width;
        _image_info.image_extend.height    = height;
        _image_info.image_extend.depth     = depth;
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
    inline void Image::Builder<TexType, Type, NBits>::setDevice(const PtrDevice& ptr_device) noexcept
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
    inline PtrImage Image::Builder<TexType, Type, NBits>::buildPtr() const
    {
        if (_queue_family_indicies.size() == 1) {
            return make_shared<Image>(
                _ptr_device,
                _memory_type_index,
                _image_info,
                _queue_family_indicies[0]
            );
        }

        return make_shared<Image>(
            _ptr_device,
            _memory_type_index,
            _image_info,
            _queue_family_indicies
        );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::BuilderWithData()
    {
        Builder<TexType, Type, NBits>::_image_info.format = FormatDefinition<TexType, Type, NBits>::getFormat();
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::BuilderWithData(
        uint32_t width, 
        uint32_t height, 
        uint32_t depth
    )
    {
        Builder<TexType, Type, NBits>::_image_info.format = FormatDefinition<TexType, Type, NBits>::getFormat();

        Builder<TexType, Type, NBits>::_image_info.image_extend.width     = width;
        Builder<TexType, Type, NBits>::_image_info.image_extend.height    = height;
        Builder<TexType, Type, NBits>::_image_info.image_extend.depth     = depth;

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
    )
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
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::setExtend(
        uint32_t width, 
        uint32_t height, 
        uint32_t depth
    )  noexcept
    {
        Builder<TexType, Type, NBits>::_image_info.image_extend.width     = width;
        Builder<TexType, Type, NBits>::_image_info.image_extend.height    = height;
        Builder<TexType, Type, NBits>::_image_info.image_extend.depth     = depth;

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
        assert(_data.size() == size);
        memcpy(_data.data(), ptr, size * sizeof(Type));
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
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::pushBack(const Vec2<Type>& c)
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
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::pushBack(const Vec3<Type>& c)
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
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::pushBack(const Vec4<Type>& c)
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
            assert(i < _data.size() / sizeof(Vec2<Type>));
            Vec2<Type>* ptr = reinterpret_cast<Vec2<Type>*>(_data.data());
            ptr[i].r = r;
        } else if constexpr (TexType == TexelType::RGB) {
            assert(i < _data.size() / sizeof(Vec3<Type>));
            Vec3<Type>* ptr = reinterpret_cast<Vec3<Type>*>(_data.data());
            ptr[i].r = r;
        } else if constexpr (TexType == TexelType::RGBA) {
            assert(i < _data.size() / sizeof(Vec4<Type>));
            Vec4<Type>* ptr = reinterpret_cast<Vec4<Type>*>(_data.data());
            ptr[i].r = r;
        }
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::set(size_t i, const Vec2<Type>& c)
    {
        if constexpr (TexType == TexelType::R) {
            assert(i < _data.size());
            _data[i] = c.r;
        } else if constexpr (TexType == TexelType::RG) {
            assert(i < _data.size() / sizeof(Vec2<Type>));
            Vec2<Type>* ptr = reinterpret_cast<Vec2<Type>*>(_data.data());

            ptr[i] = c;
        } else if constexpr (TexType == TexelType::RGB) {
            assert(i < _data.size() / sizeof(Vec3<Type>));
            Vec3<Type>* ptr = reinterpret_cast<Vec3<Type>*>(_data.data());

            ptr[i].r = c.r;
            ptr[i].g = c.g;
        } else if constexpr (TexType == TexelType::RGBA) {
            assert(i < _data.size() / sizeof(Vec4<Type>));
            Vec4<Type>* ptr = reinterpret_cast<Vec4<Type>*>(_data.data());

            ptr[i].r = c.r;
            ptr[i].g = c.g;
        }
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::set(size_t i, const Vec3<Type>& c)
    {
        if constexpr (TexType == TexelType::R) {
            assert(i < _data.size());
            _data[i] = c.r;
        } else if constexpr (TexType == TexelType::RG) {
            assert(i < _data.size() / sizeof(Vec2<Type>));
            Vec2<Type>* ptr = reinterpret_cast<Vec2<Type>*>(_data.data());

            ptr[i].r = c.r;
            ptr[i].g = c.g;
        } else if constexpr (TexType == TexelType::RGB) {
            assert(i < _data.size() / sizeof(Vec3<Type>));
            Vec3<Type>* ptr = reinterpret_cast<Vec3<Type>*>(_data.data());

            ptr[i] = c;
        } else if constexpr (TexType == TexelType::RGBA) {
            assert(i < _data.size() / sizeof(Vec4<Type>));
            Vec4<Type>* ptr = reinterpret_cast<Vec4<Type>*>(_data.data());

            ptr[i].r = c.r;
            ptr[i].g = c.g;
            ptr[i].b = c.b;
        }
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::set(size_t i, const Vec4<Type>& c)
    {
        if constexpr (TexType == TexelType::R) {
            assert(i < _data.size());
            _data[i] = c.r;
        } else if constexpr (TexType == TexelType::RG) {
            assert(i < _data.size() / sizeof(Vec2<Type>));
            Vec2<Type>* ptr = reinterpret_cast<Vec2<Type>*>(_data.data());

            ptr[i].r = c.r;
            ptr[i].g = c.g;
        } else if constexpr (TexType == TexelType::RGB) {
            assert(i < _data.size() / sizeof(Vec3<Type>));
            Vec3<Type>* ptr = reinterpret_cast<Vec3<Type>*>(_data.data());

            ptr[i].r = c.r;
            ptr[i].g = c.g;
            ptr[i].b = c.b;
        } else if constexpr (TexType == TexelType::RGBA) {
            assert(i < _data.size() / sizeof(Vec4<Type>));
            Vec4<Type>* ptr = reinterpret_cast<Vec4<Type>*>(_data.data());

            ptr[i] = c;
        }
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::set(size_t i, size_t j, Type r)
    {
        if constexpr (TexType == TexelType::R) {
            assert(!"This type of texel does not support two-dimensional iteration according to data");
        } else if constexpr (TexType == TexelType::RG) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(Vec2<Type>));
            Vec2<Type>* ptr = reinterpret_cast<Vec2<Type>*>(_data.data());
            
            ptr[i * _image_info.image_extend.width + j].r = r;
        } else if constexpr (TexType == TexelType::RGB) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(Vec3<Type>));
            Vec3<Type>* ptr = reinterpret_cast<Vec3<Type>*>(_data.data());
            
            ptr[i * _image_info.image_extend.width + j].r = r;
        } else if constexpr (TexType == TexelType::RGBA) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(Vec4<Type>));
            Vec4<Type>* ptr = reinterpret_cast<Vec4<Type>*>(_data.data());
            
            ptr[i * _image_info.image_extend.width + j].r = r;
        }
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::set(size_t i, size_t j, const Vec2<Type>& c)
    {
        if constexpr (TexType == TexelType::R) {
            assert(!"This type of texel does not support two-dimensional iteration according to data");
        } else if constexpr (TexType == TexelType::RG) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(Vec2<Type>));
            Vec2<Type>* ptr = reinterpret_cast<Vec2<Type>*>(_data.data());
            
            ptr[i * _image_info.image_extend.width + j] = c;
        } else if constexpr (TexType == TexelType::RGB) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(Vec3<Type>));
            Vec3<Type>* ptr = reinterpret_cast<Vec3<Type>*>(_data.data());
            
            ptr[i * _image_info.image_extend.width + j].r = c.r;
            ptr[i * _image_info.image_extend.width + j].g = c.g;
        } else if constexpr (TexType == TexelType::RGBA) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(Vec4<Type>));
            Vec4<Type>* ptr = reinterpret_cast<Vec4<Type>*>(_data.data());
            
            ptr[i * _image_info.image_extend.width + j].r = c.r;
            ptr[i * _image_info.image_extend.width + j].g = c.g;
        }
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::set(size_t i, size_t j, const Vec3<Type>& c)
    {
        if constexpr (TexType == TexelType::R) {
            assert(!"This type of texel does not support two-dimensional iteration according to data");
        } else if constexpr (TexType == TexelType::RG) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(Vec2<Type>));
            Vec2<Type>* ptr = reinterpret_cast<Vec2<Type>*>(_data.data());
            
            ptr[i * _image_info.image_extend.width + j].r = c.r;
            ptr[i * _image_info.image_extend.width + j].g = c.g;
        } else if constexpr (TexType == TexelType::RGB) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(Vec3<Type>));
            Vec3<Type>* ptr = reinterpret_cast<Vec3<Type>*>(_data.data());
            
            ptr[i * _image_info.image_extend.width + j] = c;
        } else if constexpr (TexType == TexelType::RGBA) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(Vec4<Type>));
            Vec4<Type>* ptr = reinterpret_cast<Vec4<Type>*>(_data.data());
            
            ptr[i * _image_info.image_extend.width + j].r = c.r;
            ptr[i * _image_info.image_extend.width + j].g = c.g;
            ptr[i * _image_info.image_extend.width + j].b = c.b;
        }
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline void Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::set(size_t i, size_t j, const Vec4<Type>& c)
    {
        if constexpr (TexType == TexelType::R) {
            assert(!"This type of texel does not support two-dimensional iteration according to data");
        } else if constexpr (TexType == TexelType::RG) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(Vec2<Type>));
            Vec2<Type>* ptr = reinterpret_cast<Vec2<Type>*>(_data.data());
            
            ptr[i * _image_info.image_extend.width + j].r = c.r;
            ptr[i * _image_info.image_extend.width + j].g = c.g;
        } else if constexpr (TexType == TexelType::RGB) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(Vec3<Type>));
            Vec3<Type>* ptr = reinterpret_cast<Vec3<Type>*>(_data.data());
            
            ptr[i * _image_info.image_extend.width + j].r = c.r;
            ptr[i * _image_info.image_extend.width + j].g = c.g;
            ptr[i * _image_info.image_extend.width + j].b = c.b;
        } else if constexpr (TexType == TexelType::RGBA) {
            assert(i * _image_info.image_extend.width + j < _data.size() / sizeof(Vec4<Type>));
            Vec4<Type>* ptr = reinterpret_cast<Vec4<Type>*>(_data.data());
            
            ptr[i * _image_info.image_extend.width + j] = c;
        }
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline Image Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::build() const
    {           
       Image image = Builder<TexType, Type, NBits>::_queue_family_indicies.size() == 1 ?
            Image(
                Builder<TexType, Type, NBits>::_ptr_device,
                Builder<TexType, Type, NBits>::_memory_type_index,
                Builder<TexType, Type, NBits>::_image_info,
                Builder<TexType, Type, NBits>::_queue_family_indicies[0]
            )
            :
            Image(
                Builder<TexType, Type, NBits>::_ptr_device,
                Builder<TexType, Type, NBits>::_memory_type_index,
                Builder<TexType, Type, NBits>::_image_info,
                Builder<TexType, Type, NBits>::_queue_family_indicies
            );

        image.map();
        image.setData(_data);
        image.unmap();

        return image;
    }

    template<Image::TexelType TexType, typename Type, Image::NumBits NBits, typename AllocatorType>
    inline PtrImage Image::BuilderWithData<TexType, Type, NBits, AllocatorType>::buildPtr() const
    {
        PtrImage ptr_image;

        if (Builder<TexType, Type, NBits>::_queue_family_indicies.size() == 1) {
            ptr_image = make_shared<Image>(
                Builder<TexType, Type, NBits>::_ptr_device,
                Builder<TexType, Type, NBits>::_memory_type_index,
                Builder<TexType, Type, NBits>::_image_info,
                Builder<TexType, Type, NBits>::_queue_family_indicies[0]
            );
        } else {
            ptr_image = make_shared<Image>(
                Builder<TexType, Type, NBits>::_ptr_device,
                Builder<TexType, Type, NBits>::_memory_type_index,
                Builder<TexType, Type, NBits>::_image_info,
                Builder<TexType, Type, NBits>::_queue_family_indicies
            );
        }

        ptr_image->map();
        ptr_image->setData(_data);
        ptr_image->unmap();

        return ptr_image;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline void ImageView::Builder::setImage(const PtrImage& ptr_image)
    {
        _ptr_image = ptr_image;
    }

    inline void ImageView::Builder::setFormat(VkFormat format) noexcept
    {
        _format = format;
    }

    inline void ImageView::Builder::setAspectMask(
        VkImageAspectFlags aspect_mask
    ) noexcept
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

    inline PtrImageView ImageView::Builder::buildPtr() const
    {
        return make_shared<ImageView>(_ptr_image, _format, _subresource_range, _image_view_type);
    }
} 

