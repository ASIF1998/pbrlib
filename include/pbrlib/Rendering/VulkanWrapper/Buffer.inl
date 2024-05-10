//
//  Buffer.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 03/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    template<typename Type>
    inline void Buffer::Builder<Type>::setSize(size_t size) 
    {
        _size = size;
    }

    template<typename Type>
    inline void Buffer::Builder<Type>::setUsage(VkBufferUsageFlags usage)
    {
        _usage = usage;
    }

    template<typename Type>
    inline void Buffer::Builder<Type>::addQueueFamily(uint32_t queue_family)
    {
        _queue_family_indicies.push_back(queue_family);
    }

    template<typename Type>
    inline void Buffer::Builder<Type>::setDevice(const PtrDevice& ptr_device)
    {
        _ptr_device = ptr_device;
    }

    template<typename Type>
    inline void Buffer::Builder<Type>::setMemoryTypeIndex(uint32_t memory_type_index)
    {
        _memory_type_index = memory_type_index;
    }

    template<typename Type>
    inline Buffer Buffer::Builder<Type>::build() const
    {
        if (_queue_family_indicies.size() == 1) {
            return Buffer(
                _ptr_device,
                static_cast<VkDeviceSize>(_size * sizeof(Type)),
                _usage,
                _memory_type_index,
                _queue_family_indicies[0]
            );
        }

        return Buffer(
            _ptr_device,
            static_cast<VkDeviceSize>(_size * sizeof(Type)),
            _usage,
            _memory_type_index,
            _queue_family_indicies
        );
    }

    template<typename Type>
    inline PtrBuffer Buffer::Builder<Type>::buildPtr() const
    {
        PtrBuffer ptr_buffer;
        
        if (_queue_family_indicies.size() == 1) {
            return make_shared<Buffer>(
                _ptr_device,
                static_cast<VkDeviceSize>(_size * sizeof(Type)),
                _usage,
                _memory_type_index,
                _queue_family_indicies[0]
            );
        } 
        return make_shared<Buffer>(
            _ptr_device,
            static_cast<VkDeviceSize>(_size * sizeof(Type)),
            _usage,
            _memory_type_index,
            _queue_family_indicies
        );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename Type, typename AllocatorType>
    inline Buffer::BuilderWithData<Type, AllocatorType>::BuilderWithData(size_t n) :
        _data(n)
    {}

    template<typename Type, typename AllocatorType>
    inline void Buffer::BuilderWithData<Type, AllocatorType>::setSize(size_t size)
    {
        _data.resize(size);
    }

    template<typename Type, typename AllocatorType>
    inline void Buffer::BuilderWithData<Type, AllocatorType>::reserve(size_t size)
    {
        _data.reserve(size);
    }

    template<typename Type, typename AllocatorType>
    inline void Buffer::BuilderWithData<Type, AllocatorType>::pushBack(Type v)
    {
        _data.push_back(v);
    }

    template<typename Type, typename AllocatorType>
    template<typename Container>
    inline void Buffer::BuilderWithData<Type, AllocatorType>::setData(const Container& data)
    {
        static_assert(
            is_same<const Type&, decltype(data[0])>::value,
            "The type of items in the container does not match the type of items in the buffer"
        );

        if (data.size() != _data.size()) {
            _data.resize(data.size());
        }

        if (data.size()) {
            memcpy(_data.data(), data.data(), sizeof(Type) * data.size());
        }
    }

    template<typename Type, typename AllocatorType>
    inline void Buffer::BuilderWithData<Type, AllocatorType>::setData(const Type* ptr, size_t size)
    {
        assert(ptr);

        if (size != _data.size()) {
            _data.resize(size);
        }

        if (size) {
            memcpy(_data.data(), ptr, sizeof(Type) * size);
        }
    }

    template<typename Type, typename AllocatorType>
    inline size_t Buffer::BuilderWithData<Type, AllocatorType>::getSize() const
    {
        return _data.size();
    }

    template<typename Type, typename AllocatorType>
    inline void Buffer::BuilderWithData<Type, AllocatorType>::addData(const Type& data)
    {
        _data.push_back(data);
    }

    template<typename Type, typename AllocatorType>
    inline void Buffer::BuilderWithData<Type, AllocatorType>::addData(Type&& data)
    {
        _data.push_back(move(data));
    }

    template<typename Type, typename AllocatorType>
    inline Type& Buffer::BuilderWithData<Type, AllocatorType>::operator [] (size_t i) noexcept
    {
        return _data[i];
    }

    template<typename Type, typename AllocatorType>
    inline Buffer Buffer::BuilderWithData<Type, AllocatorType>::build() const
    {
        Buffer buffer = Builder<Type>::_queue_family_indicies.size() == 1 ?
            Buffer(
                Builder<Type>::_ptr_device,
                static_cast<VkDeviceSize>(_data.size() * sizeof(Type)),
                Builder<Type>::_usage,
                Builder<Type>::_memory_type_index,
                Builder<Type>::_queue_family_indicies[0]
            ) 
            :
            Buffer(
                Builder<Type>::_ptr_device,
                static_cast<VkDeviceSize>(_data.size() * sizeof(Type)),
                Builder<Type>::_usage,
                Builder<Type>::_memory_type_index,
                Builder<Type>::_queue_family_indicies
            );
        
        buffer.getDeviceMemory()->map();
        buffer.getDeviceMemory()->setData(_data);
        buffer.getDeviceMemory()->unmap();

        return buffer;
    }

    template<typename Type, typename AllocatorType>
    inline PtrBuffer Buffer::BuilderWithData<Type, AllocatorType>::buildPtr() const
    {
        PtrBuffer ptr_buffer;
        
        if (Builder<Type>::_queue_family_indicies.size() == 1) {
            ptr_buffer = make_shared<Buffer>(
                Builder<Type>::_ptr_device,
                static_cast<VkDeviceSize>(_data.size() * sizeof(Type)),
                Builder<Type>::_usage,
                Builder<Type>::_memory_type_index,
                Builder<Type>::_queue_family_indicies[0]
            );
        } else {
            ptr_buffer = make_shared<Buffer>(
                Builder<Type>::_ptr_device,
                static_cast<VkDeviceSize>(_data.size() * sizeof(Type)),
                Builder<Type>::_usage,
                Builder<Type>::_memory_type_index,
                Builder<Type>::_queue_family_indicies
            );
        }

        ptr_buffer->getDeviceMemory()->map();
        ptr_buffer->getDeviceMemory()->setData(_data);
        ptr_buffer->getDeviceMemory()->unmap();

        return ptr_buffer;
    }
}
