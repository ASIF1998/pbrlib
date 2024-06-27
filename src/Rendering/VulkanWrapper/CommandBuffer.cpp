//
//  CommandBuffer.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 07/03/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/VulkanWrapper/CommandBuffer.hpp>

#include <pbrlib/Rendering/VulkanWrapper/RenderPass.hpp>
#include <pbrlib/Rendering/VulkanWrapper/DescriptorSet.hpp>

#include <pbrlib/Rendering/VulkanWrapper/Buffer.hpp>
#include <pbrlib/Rendering/VulkanWrapper/Image.hpp>

#include <pbrlib/Rendering/VulkanWrapper/Framebuffer.hpp>

#include <pbrlib/Rendering/VulkanWrapper/ComputePipeline.hpp>

using namespace pbrlib::math;

namespace pbrlib
{
    CommandBuffer::CommandBuffer(
        const CommandPool*      ptr_command_pool, 
        VkCommandBufferLevel    level
    ) :
        _ptr_command_pool       (ptr_command_pool),
        _command_buffer_handle  (VK_NULL_HANDLE)
    {
        VkCommandBufferAllocateInfo allocate_info = { };
        allocate_info.sType                 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocate_info.commandPool           = _ptr_command_pool->getCommandPoolHandle();
        allocate_info.level                 = level;
        allocate_info.commandBufferCount    = 1;

        assert(vkAllocateCommandBuffers(
            _ptr_command_pool->getDevice()->getDeviceHandle(),
            &allocate_info,
            &_command_buffer_handle
        ) == VK_SUCCESS);

        assert(_command_buffer_handle != VK_NULL_HANDLE);
    }

    CommandBuffer::CommandBuffer(CommandBuffer&& command_buffer) :
        _ptr_command_pool       (std::move(command_buffer._ptr_command_pool)),
        _command_buffer_handle  (VK_NULL_HANDLE)
    {
        std::swap(_command_buffer_handle, command_buffer._command_buffer_handle);
    }

    CommandBuffer::~CommandBuffer()
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

    void CommandBuffer::bindVertexBuffers(
        uint32_t                        first_binding,
        std::span<const Buffer>         buffers,
        std::span<const VkDeviceSize>   offsets
    ) const
    {
        std::vector<VkBuffer> buffer_handles(buffers.size());

        for (size_t i{0}; i < buffer_handles.size(); i++)
            buffer_handles[i] = buffers[i].getBufferHandle();

        assert(offsets.size() >= buffer_handles.size());

        vkCmdBindVertexBuffers(
            _command_buffer_handle,
            first_binding,
            static_cast<uint32_t>(buffer_handles.size()),
            buffer_handles.data(),
            offsets.data()
        );
    }

    void CommandBuffer::bindVertexBuffer(const Buffer& buffer, VkDeviceSize offset) const noexcept
    {
        vkCmdBindVertexBuffers(_command_buffer_handle, 0, 1, &buffer.getBufferHandle(), &offset);
    }

    void CommandBuffer::bindIndexBuffer(
        const Buffer&   index_buffer,  
        VkDeviceSize    offset, 
        VkIndexType     index_type
    ) const noexcept
    {
        vkCmdBindIndexBuffer(_command_buffer_handle, index_buffer.getBufferHandle(), offset, index_type);
    }

    void CommandBuffer::bindDescriptorSet(
        std::shared_ptr<const GraphicsPipeline> ptr_graphics_pipeline,
        const DescriptorSet&                    descriptor_set
    ) const
    {
        vkCmdBindDescriptorSets(
            _command_buffer_handle,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            ptr_graphics_pipeline->getPipelineLayout()->getPipelineLayoutHandle(),
            0, 1, &descriptor_set.getDescriptorSetHandle(),
            0, nullptr
        );
    }

    void CommandBuffer::bindDescriptorSet(
        std::shared_ptr<const ComputePipeline>  ptr_compute_pipeline,
        const DescriptorSet&                    descriptor_set
    ) const
    {
        vkCmdBindDescriptorSets(
            _command_buffer_handle,
            VK_PIPELINE_BIND_POINT_COMPUTE,
            ptr_compute_pipeline->getPipelineLayout()->getPipelineLayoutHandle(),
            0, 1, &descriptor_set.getDescriptorSetHandle(),
            0, nullptr
        );
    }

    void CommandBuffer::bindToPipeline(std::shared_ptr<const GraphicsPipeline> ptr_pipeline) const
    {
        vkCmdBindPipeline(_command_buffer_handle, VK_PIPELINE_BIND_POINT_GRAPHICS, ptr_pipeline->getPipelineHandle());
    }

    void CommandBuffer::bindToPipeline(std::shared_ptr<const ComputePipeline> ptr_pipeline) const
    {
        vkCmdBindPipeline(_command_buffer_handle, VK_PIPELINE_BIND_POINT_COMPUTE, ptr_pipeline->getPipelineHandle());
    }

    void CommandBuffer::draw(
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

    void CommandBuffer::drawIndexed(
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

    void CommandBuffer::drawIndexedInderect(
        const Buffer&   buffer,
        VkDeviceSize    offset,
        uint32_t        draw_count,
        uint32_t        stride
    ) const
    {
        vkCmdDrawIndexedIndirect(
            _command_buffer_handle,
            buffer.getBufferHandle(),
            offset,
            draw_count,
            stride
        );
    }

    void CommandBuffer::dispatch(uint32_t group_count_x, uint32_t group_count_y, uint32_t group_count_z) const noexcept
    {
        vkCmdDispatch(_command_buffer_handle, group_count_x, group_count_y, group_count_z);
    }

    template<class Collection>
    void CommandBuffer::updateBuffer(
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
    void CommandBuffer::updateBuffer(
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

    void CommandBuffer::copyBuffer(
        const Buffer&   src_buffer,
        const Buffer&   dst_buffer,
        VkDeviceSize    size
    ) const noexcept
    {
        VkBufferCopy region_info = { };
        region_info.srcOffset   = 0;
        region_info.dstOffset   = 0;
        region_info.size        = size;

        vkCmdCopyBuffer(
            _command_buffer_handle,
            src_buffer.getBufferHandle(),
            dst_buffer.getBufferHandle(),
            1,
            &region_info
        );
    }

    void CommandBuffer::copyBuffer(
        const Buffer&                   src_buffer,
        const Buffer&                   dst_buffer,
        std::span<const VkBufferCopy>   regions
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

    void CommandBuffer::copyImage(
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

    void CommandBuffer::copyImage(
        const Image&                    src_image,
        const Image&                    dst_image,
        std::span<const VkImageCopy>    regions
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

    void CommandBuffer::copyBufferToImage(
        const Buffer&               src_buffer,
        uint32_t                    src_buffer_offset,
        const Image&                dst_image,
        VkImageLayout               dst_image_layout,
        VkImageSubresourceLayers    dst_image_subresource_layers,
        VkOffset3D                  dst_image_offset,
        VkExtent3D                  dst_image_extent
    )
    {
        VkBufferImageCopy region = { };
        region.bufferOffset         = src_buffer_offset;
        region.bufferRowLength      = 0;
        region.bufferImageHeight    = 0;
        region.imageSubresource     = dst_image_subresource_layers;
        region.imageOffset          = dst_image_offset;
        region.imageExtent          = dst_image_extent;

        vkCmdCopyBufferToImage(_command_buffer_handle, src_buffer.getBufferHandle(), dst_image.getImageHandle(), dst_image_layout, 1, &region);
    }

    void CommandBuffer::imageMemoryBarrier(
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
        VkImageMemoryBarrier image_memory_barier = { };
        image_memory_barier.sType                   = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        image_memory_barier.srcAccessMask           = src_access_mask;
        image_memory_barier.dstAccessMask           = dst_access_mask;
        image_memory_barier.oldLayout               = old_image_layout;
        image_memory_barier.newLayout               = new_image_layout;
        image_memory_barier.srcQueueFamilyIndex     = src_queue_family_index;
        image_memory_barier.dstQueueFamilyIndex     = dst_queue_family_index;
        image_memory_barier.image                   = image.getImageHandle();
        image_memory_barier.subresourceRange        = subresource_range;

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

    void CommandBuffer::bufferMemoryBarrier(
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
        VkBufferMemoryBarrier buffer_memory_barrier = { };
        buffer_memory_barrier.sType                 = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
        buffer_memory_barrier.srcAccessMask         = src_access_mask;
        buffer_memory_barrier.dstAccessMask         = dst_access_mask;
        buffer_memory_barrier.srcQueueFamilyIndex   = src_queue_family_index;
        buffer_memory_barrier.dstQueueFamilyIndex   = dst_queue_family_index;
        buffer_memory_barrier.buffer                = buffer.getBufferHandle();
        buffer_memory_barrier.offset                = offset;
        buffer_memory_barrier.size                  = size;

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

    void CommandBuffer::clearColorImage(
        const Image&                    image,
        VkImageLayout                   image_layout,
        const Vec4<float>&              clear_color,
        const VkImageSubresourceRange&  image_subresource_range
    ) const noexcept
    {
        VkClearColorValue vk_clear_color = { };
        vk_clear_color.float32[0] = clear_color.r;
        vk_clear_color.float32[1] = clear_color.g;
        vk_clear_color.float32[2] = clear_color.b;
        vk_clear_color.float32[3] = clear_color.a;

        vkCmdClearColorImage(
            _command_buffer_handle, 
            image.getImageHandle(), image_layout, 
            &vk_clear_color, 
            1, &image_subresource_range
        );
    }

    void CommandBuffer::clearDepthStencilImage(
        const Image&                    image,
        VkImageLayout                   image_layout,
        float                           depth_val,
        uint32_t                        stencil_val,
        const VkImageSubresourceRange&  image_subresource_range
    ) const noexcept
    {
        VkClearDepthStencilValue vk_clear_depth_stencil_val = { };
        vk_clear_depth_stencil_val.depth    = depth_val;
        vk_clear_depth_stencil_val.stencil  = stencil_val;

        vkCmdClearDepthStencilImage(
            _command_buffer_handle,
            image.getImageHandle(), image_layout,
            &vk_clear_depth_stencil_val, 
            1, &image_subresource_range
        );
    }

    void CommandBuffer::reset() const noexcept
    {
        vkResetCommandBuffer(_command_buffer_handle, 0);
    }

    const Device* CommandBuffer::getDevice() const noexcept
    {
        return _ptr_command_pool->getDevice();
    }

    const VkCommandBuffer& CommandBuffer::getCommandBufferHandle() const noexcept
    {
        return _command_buffer_handle;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PrimaryCommandBuffer::PrimaryCommandBuffer(const CommandPool* ptr_command_pool) :
        CommandBuffer (ptr_command_pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY)
    {}

    PrimaryCommandBuffer::PrimaryCommandBuffer(PrimaryCommandBuffer&& command_buffer) :
        CommandBuffer (std::move(command_buffer)),
        _ptr_pipeline (std::move(command_buffer._ptr_pipeline))
    {}

    void PrimaryCommandBuffer::begin() const
    {
        VkCommandBufferBeginInfo begin_info = { };
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        assert(vkBeginCommandBuffer(_command_buffer_handle, &begin_info) == VK_SUCCESS);
    }

    void PrimaryCommandBuffer::end() const
    {
        assert(vkEndCommandBuffer(_command_buffer_handle) == VK_SUCCESS);
    }

    void PrimaryCommandBuffer::begineRenderPass(std::shared_ptr<const Framebuffer> ptr_framebuffer, std::span<const VkClearValue> clear_values)  const noexcept
    {
        VkRenderPassBeginInfo begin_indo = { };
        begin_indo.sType                    = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        begin_indo.renderPass               = ptr_framebuffer->getRenderPass()->getRenderPassHandle();
        begin_indo.framebuffer              = ptr_framebuffer->getFramebufferHandle();
        begin_indo.renderArea.offset.x      = 0;
        begin_indo.renderArea.offset.y      = 0;
        begin_indo.renderArea.extent.width  = ptr_framebuffer->getWidth();
        begin_indo.renderArea.extent.height = ptr_framebuffer->getHeight();
        begin_indo.clearValueCount          = static_cast<uint32_t>(clear_values.size());
        begin_indo.pClearValues             = clear_values.data();

        vkCmdBeginRenderPass(_command_buffer_handle, &begin_indo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void PrimaryCommandBuffer::begineRenderPass(std::shared_ptr<const Framebuffer> ptr_framebuffer, const VkClearValue& clear_value) const noexcept
    {   
        VkRenderPassBeginInfo begin_indo = { };
        begin_indo.sType                    = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        begin_indo.renderPass               = ptr_framebuffer->getRenderPass()->getRenderPassHandle();
        begin_indo.framebuffer              = ptr_framebuffer->getFramebufferHandle();
        begin_indo.renderArea.offset.x      = 0;
        begin_indo.renderArea.offset.y      = 0;
        begin_indo.renderArea.extent.width  = ptr_framebuffer->getWidth();
        begin_indo.renderArea.extent.height = ptr_framebuffer->getHeight();
        begin_indo.clearValueCount          = 1;
        begin_indo.pClearValues             = &clear_value;

        vkCmdBeginRenderPass(_command_buffer_handle, &begin_indo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void PrimaryCommandBuffer::begineRenderPass(std::shared_ptr<const Framebuffer> ptr_framebuffer) const noexcept
    {
        VkRenderPassBeginInfo begin_indo = { };
        begin_indo.sType                    = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        begin_indo.renderPass               = ptr_framebuffer->getRenderPass()->getRenderPassHandle();
        begin_indo.framebuffer              = ptr_framebuffer->getFramebufferHandle();
        begin_indo.renderArea.offset.x      = 0;
        begin_indo.renderArea.offset.y      = 0;
        begin_indo.renderArea.extent.width  = ptr_framebuffer->getWidth();
        begin_indo.renderArea.extent.height = ptr_framebuffer->getHeight();
        begin_indo.clearValueCount          = 0;
        begin_indo.pClearValues             = nullptr;

        vkCmdBeginRenderPass(_command_buffer_handle, &begin_indo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void PrimaryCommandBuffer::endRenderPass() const noexcept
    {
        vkCmdEndRenderPass(_command_buffer_handle);
    }

    void PrimaryCommandBuffer::nextSubpass() const noexcept
    {
        vkCmdNextSubpass(_command_buffer_handle, VK_SUBPASS_CONTENTS_INLINE);
    }

    std::shared_ptr<const GraphicsPipeline> PrimaryCommandBuffer::getPipeline() const noexcept
    {
        return _ptr_pipeline;
    }

    std::unique_ptr<PrimaryCommandBuffer> PrimaryCommandBuffer::make(const CommandPool* ptr_command_pool)
    {
        return std::make_unique<PrimaryCommandBuffer>(ptr_command_pool);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SecondaryCommandBuffer::SecondaryCommandBuffer(const CommandPool* ptr_command_pool) :
        CommandBuffer(ptr_command_pool, VK_COMMAND_BUFFER_LEVEL_SECONDARY)
    {}

    SecondaryCommandBuffer::SecondaryCommandBuffer(SecondaryCommandBuffer&& command_buffer) :
        CommandBuffer (std::move(command_buffer))
    {} 

    void SecondaryCommandBuffer::begin(const PrimaryCommandBuffer& primary_command_buffer, std::shared_ptr<const Framebuffer> ptr_framebuffer) const
    {
        VkCommandBufferInheritanceInfo inheritance_info = { };
        inheritance_info.sType          = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
        inheritance_info.renderPass     = ptr_framebuffer->getRenderPass()->getRenderPassHandle();
        inheritance_info.subpass        = primary_command_buffer.getPipeline()->getSubpassIndex();
        inheritance_info.framebuffer    = ptr_framebuffer->getFramebufferHandle();

        VkCommandBufferBeginInfo begin_info = { };
        begin_info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags            = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        begin_info.pInheritanceInfo = &inheritance_info;

        assert(vkBeginCommandBuffer(_command_buffer_handle, &begin_info) == VK_SUCCESS);
    }     

    void SecondaryCommandBuffer::end() const
    {
        assert(vkEndCommandBuffer(_command_buffer_handle) == VK_SUCCESS);
    }

    std::unique_ptr<SecondaryCommandBuffer> SecondaryCommandBuffer::make(const CommandPool* ptr_command_pool)
    {
        return std::make_unique<SecondaryCommandBuffer>(ptr_command_pool);
    }
}
