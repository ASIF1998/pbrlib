//
//  CommandBuffer.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 07/03/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    inline CommandBuffer::CommandBuffer(
        const PtrCommandPool&   ptr_command_pool, 
        VkCommandBufferLevel    level
    ) :
        _ptr_command_pool       (ptr_command_pool),
        _command_buffer_handle  (VK_NULL_HANDLE)
    {
        VkCommandBufferAllocateInfo allocate_info {
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool        = _ptr_command_pool->getCommandPoolHandle(),
            .level              = level,
            .commandBufferCount = 1
        };

        assert(vkAllocateCommandBuffers(
            _ptr_command_pool->getDevice()->getDeviceHandle(),
            &allocate_info,
            &_command_buffer_handle
        ) == VK_SUCCESS);

        assert(_command_buffer_handle != VK_NULL_HANDLE);
    }

    inline CommandBuffer::CommandBuffer(CommandBuffer&& command_buffer) :
        _ptr_command_pool       (move(command_buffer._ptr_command_pool)),
        _command_buffer_handle  (VK_NULL_HANDLE)
    {
        swap(_command_buffer_handle, command_buffer._command_buffer_handle);
    }

    inline CommandBuffer::~CommandBuffer() noexcept
    {
        if (_command_buffer_handle != VK_NULL_HANDLE) {
            vkFreeCommandBuffers(
                getDevice()->getDeviceHandle(),
                _ptr_command_pool->getCommandPoolHandle(),
                1,
                &_command_buffer_handle
            );
        }
    }

    inline void CommandBuffer::bindVertexBuffers(
        uint32_t                    first_binding,
        const vector<Buffer>&       buffers,
        const vector<VkDeviceSize>& offsets
    ) const
    {
        vector<VkBuffer> buffer_handles(buffers.size());

        for (size_t i{0}; i < buffer_handles.size(); i++) {
            buffer_handles[i] = buffers[i].getBufferHandle();
        }

        assert(offsets.size() >= buffer_handles.size());

        vkCmdBindVertexBuffers(
            _command_buffer_handle,
            first_binding,
            static_cast<uint32_t>(buffer_handles.size()),
            buffer_handles.data(),
            offsets.data()
        );
    }

    inline void CommandBuffer::bindVertexBuffer(const Buffer& buffer, VkDeviceSize offset) const noexcept
    {
        vkCmdBindVertexBuffers(_command_buffer_handle, 0, 1, &buffer.getBufferHandle(), &offset);
    }

    inline void CommandBuffer::bindIndexBuffer(
        const Buffer&   index_buffer,  
        VkDeviceSize    offset, 
        VkIndexType     index_type
    ) const noexcept
    {
        vkCmdBindIndexBuffer(_command_buffer_handle, index_buffer.getBufferHandle(), offset, index_type);
    }

    inline void CommandBuffer::draw(
        uint32_t vertex_count, 
        uint32_t instance_count, 
        uint32_t first_vertex, 
        uint32_t first_instance
    ) const noexcept
    {
        vkCmdDraw(
            _command_buffer_handle,
            vertex_count,
            instance_count,
            first_vertex,
            first_instance
        );
    }

    inline void CommandBuffer::drawIndexed(
        uint32_t    index_count,
        uint32_t    instance_count,
        uint32_t    first_index,
        int32_t     vertex_offset, 
        uint32_t    first_instance
    ) const noexcept
    {
        vkCmdDrawIndexed(
            _command_buffer_handle,
            index_count,
            instance_count,
            first_index,
            vertex_offset,
            first_instance
        );
    }

    inline void CommandBuffer::drawIndexedInderect(
        const Buffer&   buffer,
        VkDeviceSize    offset,
        uint32_t        draw_count,
        uint32_t        stride
    ) const noexcept
    {
        vkCmdDrawIndexedIndirect(
            _command_buffer_handle,
            buffer.getBufferHandle(),
            offset,
            draw_count,
            stride
        );
    }

    template<class Collection>
    inline void CommandBuffer::updateBuffer(
        const Buffer&       dst_buffer,
        const Collection&   data,
        VkDeviceSize        dst_offset
    ) const
    {
        vkCmdUpdateBuffer(
            _command_buffer_handle,
            dst_buffer.getBufferHandle(),
            dst_offset,
            data.size(),
            &data[0]
        );
    }

    template<class Pointer>
    inline void CommandBuffer::updateBuffer(
        const Buffer&       dst_buffer,
        const Pointer       ptr_data,
        const size_t        size_data,
        VkDeviceSize        dst_offset
    ) const noexcept
    {
        vkCmdUpdateBuffer(
            _command_buffer_handle,
            dst_buffer.getBufferHandle(),
            dst_offset,
            size_data,
            ptr_data
        );
    }

    inline void CommandBuffer::copyBuffer(
        const Buffer&   src_buffer,
        const Buffer&   dst_buffer,
        VkDeviceSize    size
    ) const noexcept
    {
        VkBufferCopy region_info {
            .srcOffset  = 0,
            .dstOffset  = 0,
            .size       = size
        };

        vkCmdCopyBuffer(
            _command_buffer_handle,
            src_buffer.getBufferHandle(),
            dst_buffer.getBufferHandle(),
            1,
            &region_info
        );
    }

    inline void CommandBuffer::copyBuffer(
        const Buffer&                   src_buffer,
        const Buffer&                   dst_buffer,
        const vector<VkBufferCopy>&     regions
    ) const noexcept
    {
        vkCmdCopyBuffer(
            _command_buffer_handle,
            src_buffer.getBufferHandle(),
            dst_buffer.getBufferHandle(),
            static_cast<uint32_t>(regions.size()),
            regions.data()
        );
    }

    inline void CommandBuffer::copyImage(
        const Image&                src_image,
        const Image&                dst_image,
        const VkImageCopy&          region
    )
    {
        vkCmdCopyImage(
            _command_buffer_handle,
            src_image.getImageHandle(),
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            dst_image.getImageHandle(),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
        );
    }

    inline void CommandBuffer::copyImage(
        const Image&                src_image,
        const Image&                dst_image,
        const vector<VkImageCopy>&  regions
    )
    {
        vkCmdCopyImage(
            _command_buffer_handle,
            src_image.getImageHandle(),
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            dst_image.getImageHandle(),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            static_cast<uint32_t>(regions.size()),
            regions.data()
        );
    }

    inline void CommandBuffer::imageMemoryBarrier(
        VkPipelineStageFlags            src_stage_mask,
        VkPipelineStageFlags            dst_stage_mask,
        VkAccessFlags                   src_access_mask,
        VkAccessFlags                   dst_access_mask,
        VkImageLayout                   old_image_layout,
        VkImageLayout                   new_image_layout,
        uint32_t                        src_queue_family_index,
        uint32_t                        dst_queue_family_index,
        const Image&                    image,
        const VkImageSubresourceRange&  subresource_range
    ) const noexcept
    {
        VkImageMemoryBarrier image_memory_barier {
            .sType                  = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .srcAccessMask          = src_access_mask,
            .dstAccessMask          = dst_access_mask,
            .oldLayout              = old_image_layout,
            .newLayout              = new_image_layout,
            .srcQueueFamilyIndex    = src_queue_family_index,
            .dstQueueFamilyIndex    = dst_queue_family_index,
            .image                  = image.getImageHandle(),
            .subresourceRange       = subresource_range
        };

        vkCmdPipelineBarrier(
            _command_buffer_handle,
            src_stage_mask,
            dst_stage_mask,
            0,
            0, nullptr,
            0, nullptr,
            1, &image_memory_barier
        );
    }

    inline void CommandBuffer::bufferMemoryBarrier(
        VkPipelineStageFlags            src_stage_mask,
        VkPipelineStageFlags            dst_stage_mask,
        VkAccessFlags                   src_access_mask,
        VkAccessFlags                   dst_access_mask,
        uint32_t                        src_queue_family_index,
        uint32_t                        dst_queue_family_index,
        const Buffer&                   buffer,
        VkDeviceSize                    offset,
        VkDeviceSize                    size
    ) const noexcept
    {
        VkBufferMemoryBarrier buffer_memory_barrier {
            .sType                  = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
            .srcAccessMask          = src_access_mask,
            .dstAccessMask          = dst_access_mask,
            .srcQueueFamilyIndex    = src_queue_family_index,
            .dstQueueFamilyIndex    = dst_queue_family_index,
            .buffer                 = buffer.getBufferHandle(),
            .offset                 = offset,
            .size                   = size
        };

        vkCmdPipelineBarrier(
            _command_buffer_handle,
            src_access_mask,
            dst_access_mask,
            0,
            0, nullptr,
            1, &buffer_memory_barrier,
            0, nullptr
        );
    }
    inline PtrDevice& CommandBuffer::getDevice() noexcept
    {
        return _ptr_command_pool->getDevice();
    }

    inline const PtrDevice& CommandBuffer::getDevice() const noexcept
    {
        return _ptr_command_pool->getDevice();
    }

    inline const VkCommandBuffer& CommandBuffer::getCommandBufferHandle() const noexcept
    {
        return _command_buffer_handle;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline PrimaryCommandBuffer::PrimaryCommandBuffer(
        const PtrCommandPool&       ptr_command_pool,
        const PtrFramebuffer&       ptr_framebuffer,
        const PtrGraphicsPipeline&  ptr_pipeline
    ) :
        CommandBuffer       (ptr_command_pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY),
        _ptr_framebuffer    (ptr_framebuffer),
        _ptr_pipeline       (ptr_pipeline)
    {}

    inline PrimaryCommandBuffer::PrimaryCommandBuffer(PrimaryCommandBuffer&& command_buffer) :
        CommandBuffer       (move(command_buffer)),
        _ptr_framebuffer    (move(command_buffer._ptr_framebuffer)),
        _ptr_pipeline       (move(command_buffer._ptr_pipeline))
    {}

    inline void PrimaryCommandBuffer::begin() const
    {
        VkCommandBufferBeginInfo begin_info {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
        };

        assert(vkBeginCommandBuffer(_command_buffer_handle, &begin_info) == VK_SUCCESS);
    }

    inline void PrimaryCommandBuffer::end() const
    {
        assert(vkEndCommandBuffer(_command_buffer_handle) == VK_SUCCESS);
    }

    inline void PrimaryCommandBuffer::bindToPipeline() const noexcept
    {
        vkCmdBindPipeline(_command_buffer_handle, VK_PIPELINE_BIND_POINT_GRAPHICS, _ptr_pipeline->getPipelineHandle());
    }

    inline void PrimaryCommandBuffer::begineRenderPass(const vector<VkClearValue>& clear_values)  const noexcept
    {
        VkRenderPassBeginInfo begin_indo {
            .sType          = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass     = _ptr_framebuffer->getRenderPass()->getRenderPassHandle(), 
            .framebuffer    = _ptr_framebuffer->getFramebufferHandle(),
            .renderArea     = {
                .offset = {
                    .x = 0, 
                    .y = 0
                },
                .extent = {
                    .width  = _ptr_framebuffer->getWidth(),
                    .height =  _ptr_framebuffer->getHeight()
                }
            },
            .clearValueCount    = static_cast<uint32_t>(clear_values.size()),
            .pClearValues       = clear_values.data()
        };

        vkCmdBeginRenderPass(_command_buffer_handle, &begin_indo, VK_SUBPASS_CONTENTS_INLINE);
    }

    inline void PrimaryCommandBuffer::endRenderPass() const noexcept
    {
        vkCmdEndRenderPass(_command_buffer_handle);
    }

    inline void PrimaryCommandBuffer::nextSubpass() const noexcept
    {
        vkCmdNextSubpass(_command_buffer_handle, VK_SUBPASS_CONTENTS_INLINE);
    }

    inline const PtrFramebuffer& PrimaryCommandBuffer::getFramebuffer() const noexcept
    {
        return _ptr_framebuffer;
    }

    inline const PtrGraphicsPipeline& PrimaryCommandBuffer::getPipeline() const noexcept
    {
        return _ptr_pipeline;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline SecondaryCommandBuffer::SecondaryCommandBuffer(const PtrCommandPool& ptr_command_pool) :
        CommandBuffer(ptr_command_pool, VK_COMMAND_BUFFER_LEVEL_SECONDARY)
    {}

    inline SecondaryCommandBuffer::SecondaryCommandBuffer(SecondaryCommandBuffer&& command_buffer) :
        CommandBuffer       (move(command_buffer))
    {} 

    inline void SecondaryCommandBuffer::bindToPipeline(const GraphicsPipeline& pipeline) const noexcept
    {
        vkCmdBindPipeline(_command_buffer_handle, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipelineHandle());
    }

    inline void SecondaryCommandBuffer::begin(const PrimaryCommandBuffer& primary_command_buffer) const
    {
        VkCommandBufferInheritanceInfo inheritance_info {
            .sType          = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
            .renderPass     = primary_command_buffer.getFramebuffer()->getRenderPass()->getRenderPassHandle(),
            .subpass        = primary_command_buffer.getPipeline()->getSubpassIndex(),
            .framebuffer    = primary_command_buffer.getFramebuffer()->getFramebufferHandle()
        };


        VkCommandBufferBeginInfo begin_info {
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags              = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
            .pInheritanceInfo   = &inheritance_info
        };

        assert(vkBeginCommandBuffer(_command_buffer_handle, &begin_info) == VK_SUCCESS);
    }     

    inline void SecondaryCommandBuffer::end() const
    {
        assert(vkEndCommandBuffer(_command_buffer_handle) == VK_SUCCESS);
    }
}
