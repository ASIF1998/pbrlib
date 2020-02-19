//
//  RenderPass.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 19/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    inline SubpassDescription::SubpassDescription(size_t num_input_attachment,
                                                  size_t num_color_attachment,
                                                  size_t num_present_attachment) :
        _use_depth_stencil_attachment(false)
    {
        _input_attachment.reserve(num_input_attachment);
        _color_attachment.reserve(num_color_attachment);
        _present_attachment.reserve(num_present_attachment);
    }

    inline SubpassDescription::SubpassDescription(SubpassDescription&& subpass_descriptoin) :
        _depth_stencil_attachment(subpass_descriptoin._depth_stencil_attachment),
        _use_depth_stencil_attachment(subpass_descriptoin._use_depth_stencil_attachment)
    {
        swap(_input_attachment, subpass_descriptoin._input_attachment);
        swap(_color_attachment, subpass_descriptoin._color_attachment);
        swap(_present_attachment, subpass_descriptoin._present_attachment);
    }

    inline SubpassDescription::SubpassDescription(const SubpassDescription& subpass_descriptoin) :
        _input_attachment(subpass_descriptoin._input_attachment),
        _color_attachment(subpass_descriptoin._color_attachment),
        _present_attachment(subpass_descriptoin._present_attachment),
        _depth_stencil_attachment(subpass_descriptoin._depth_stencil_attachment),
        _use_depth_stencil_attachment(subpass_descriptoin._use_depth_stencil_attachment)
    {}

    inline void SubpassDescription::addInputAttachment(uint32_t attachment, VkImageLayout layout)
    {
        _input_attachment.push_back({
            .attachment = attachment,
            .layout = layout
        });
    }

    inline void SubpassDescription::addColorAttachment(uint32_t attachment, VkImageLayout layout)
    {
        _color_attachment.push_back({
            .attachment = attachment,
            .layout = layout
        });
    }

    inline void SubpassDescription::addPresentAttachment(uint32_t attachment)
    {
        _present_attachment.push_back(attachment);
    }

    inline void SubpassDescription::setDepthStencilAttachment(uint32_t attachment, VkImageLayout layout)
    {
        _use_depth_stencil_attachment = true;

        _depth_stencil_attachment = {
            .attachment = attachment,
            .layout = layout
        };
    }

    inline void SubpassDescription::resetDepthStencilAttahment() noexcept
    {
        _use_depth_stencil_attachment = false;
    }

    inline vector<VkAttachmentReference>& SubpassDescription::getInputAttachment() noexcept
    {
        return _input_attachment;
    }

    inline const vector<VkAttachmentReference>& SubpassDescription::getInputAttachment() const noexcept
    {
        return _input_attachment;
    }

    inline vector<VkAttachmentReference>& SubpassDescription::getColorAttachment() noexcept
    {
        return _color_attachment;
    }

    inline const vector<VkAttachmentReference>& SubpassDescription::getColorAttachment() const noexcept
    {
        return _color_attachment;
    }

    inline vector<uint32_t>& SubpassDescription::getPresentAttachment() noexcept
    {
        return _present_attachment;
    }

    inline const vector<uint32_t>& SubpassDescription::getPresentAttachment() const noexcept
    {
        return _present_attachment;
    }

    inline VkAttachmentReference& SubpassDescription::getDepthStencilAttachment() noexcept
    {
        return _depth_stencil_attachment;
    }

    inline const VkAttachmentReference& SubpassDescription::getDepthStencilAttachment() const noexcept
    {
        return _depth_stencil_attachment;
    }

    inline bool SubpassDescription::useDepthStencilAttachment() const noexcept
    {
        return _use_depth_stencil_attachment;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline RenderPassInfo::RenderPassInfo(size_t num_attribute_descriptions,
                                          size_t num_subpass_descriptions,
                                          size_t num_subpass_dependencies)
    {
        _attribute_descriptions.reserve(num_attribute_descriptions);
        _subpass_descriptions.resize(num_subpass_descriptions);
        _subpass_dependencies.reserve(num_subpass_dependencies);
    }

    inline RenderPassInfo::RenderPassInfo(RenderPassInfo&& render_pass_info)
    {
        swap(_attribute_descriptions, render_pass_info._attribute_descriptions);
        swap(_subpass_descriptions, render_pass_info._subpass_descriptions);
        swap(_subpass_dependencies, render_pass_info._subpass_dependencies);
    }

    inline void RenderPassInfo::setNumSubpassDescription(size_t num_subpass_description)
    {
        _subpass_descriptions.resize(num_subpass_description);
    }
    inline SubpassDescription& RenderPassInfo::getSubpassDescription(size_t i) noexcept
    {
        return _subpass_descriptions[i];
    }

    inline const SubpassDescription& RenderPassInfo::getSubpassDescription(size_t i) const noexcept
    {
        return _subpass_descriptions[i];
    }

    inline void RenderPassInfo::addAttachmentDescription(VkFormat format,
                                                         VkSampleCountFlagBits samples,
                                                         VkAttachmentLoadOp load_op,
                                                         VkAttachmentStoreOp store_op,
                                                         VkAttachmentLoadOp stencil_load_op,
                                                         VkAttachmentStoreOp stencil_store_op,
                                                         VkImageLayout initial_layout,
                                                         VkImageLayout final_layout)
    {
        _attribute_descriptions.push_back({
            .flags = 0,
            .format = format,
            .samples = samples,
            .loadOp = load_op,
            .storeOp = store_op,
            .stencilLoadOp = stencil_load_op,
            .stencilStoreOp = stencil_store_op,
            .initialLayout = initial_layout,
            .finalLayout = final_layout
        });
    }

    inline void RenderPassInfo::addSubpassDescription(SubpassDescription&& subpass_description)
    {
        _subpass_descriptions.push_back(move(subpass_description));
    }

    inline void RenderPassInfo::addSubpassDescription(const SubpassDescription& subpass_description)
    {
        _subpass_descriptions.push_back(subpass_description);
    }

    inline void RenderPassInfo::addSubpassDependency(uint32_t src_subpass,
                                                     uint32_t dst_subpass,
                                                     VkPipelineStageFlags src_stage_mask,
                                                     VkPipelineStageFlags dst_stage_mask,
                                                     VkAccessFlags src_access_mask,
                                                     VkAccessFlags dst_access_mask)
    {
        _subpass_dependencies.push_back({
            .srcSubpass = src_subpass,
            .dstSubpass = dst_subpass,
            .srcStageMask = src_stage_mask,
            .dstStageMask = dst_stage_mask,
            .srcAccessMask = src_access_mask,
            .dstAccessMask = dst_access_mask,
            .dependencyFlags = 0
        });
    }

    inline vector<VkAttachmentDescription>& RenderPassInfo::getAttachmentDescriptions() noexcept
    {
        return _attribute_descriptions;
    }

    inline const vector<VkAttachmentDescription>& RenderPassInfo::getAttachmentDescriptions() const noexcept
    {
        return _attribute_descriptions;
    }

    inline vector<SubpassDescription>& RenderPassInfo::getSubpassDescriptions() noexcept
    {
        return _subpass_descriptions;
    }

    inline const vector<SubpassDescription>& RenderPassInfo::getSubpassDescriptions() const noexcept
    {
        return _subpass_descriptions;
    }

    inline vector<VkSubpassDependency>& RenderPassInfo::getSubpassDependencies() noexcept
    {
        return _subpass_dependencies;
    }

    inline const vector<VkSubpassDependency>& RenderPassInfo::getSubpassDependencies() const noexcept
    {
        return _subpass_dependencies;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline RenderPass::RenderPass(const shared_ptr<Device>& ptr_device, const RenderPassInfo& render_pass_info) :
        _render_pass_info(render_pass_info),
        _render_pass_handle(VK_NULL_HANDLE),
        _ptr_device(ptr_device)
    {
        _create_render_pass();
    }

    inline RenderPass::RenderPass(const shared_ptr<Device>& ptr_device, RenderPassInfo&& render_pass_info) :
        _render_pass_info(move(render_pass_info)),
        _render_pass_handle(VK_NULL_HANDLE),
        _ptr_device(ptr_device)
    {
        _create_render_pass();
    }

    inline RenderPass::RenderPass(RenderPass&& render_pass) :
        _render_pass_info(move(render_pass._render_pass_info)),
        _render_pass_handle(VK_NULL_HANDLE),
        _ptr_device(render_pass._ptr_device)
    {
        swap(_render_pass_handle, render_pass._render_pass_handle);
    }

    inline RenderPass::~RenderPass() noexcept
    {
        if (_render_pass_handle != VK_NULL_HANDLE) {
            vkDestroyRenderPass(_ptr_device->getDeviceHandle(), _render_pass_handle, nullptr);
        }
    }

    inline void RenderPass::_create_render_pass()
    {
        auto& psubpass_descritions = _render_pass_info.getSubpassDescriptions();
        vector<VkSubpassDescription> subpass_description (psubpass_descritions.size());

        for (size_t i{0}; i < subpass_description.size(); i++) {
            subpass_description[i] = {
                .flags = 0,
                .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                .inputAttachmentCount = static_cast<uint32_t>(psubpass_descritions[i].getInputAttachment().size()),
                .pInputAttachments = psubpass_descritions[i].getInputAttachment().data(),
                .colorAttachmentCount = static_cast<uint32_t>(psubpass_descritions[i].getColorAttachment().size()),
                .pColorAttachments = psubpass_descritions[i].getColorAttachment().data(),
                .pResolveAttachments = nullptr,
                .pDepthStencilAttachment = (psubpass_descritions[i].useDepthStencilAttachment() ?
                                            &psubpass_descritions[i].getDepthStencilAttachment() : nullptr),
                .preserveAttachmentCount = static_cast<uint32_t>(psubpass_descritions[i].getPresentAttachment().size()),
                .pPreserveAttachments = psubpass_descritions[i].getPresentAttachment().data()
            };
        }

       VkRenderPassCreateInfo render_pass_create_info {
           .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
           .pNext = nullptr,
           .flags = 0,
           .attachmentCount = static_cast<uint32_t>(_render_pass_info.getAttachmentDescriptions().size()),
           .pAttachments = _render_pass_info.getAttachmentDescriptions().data(),
           .subpassCount = static_cast<uint32_t>(subpass_description.size()),
           .pSubpasses = subpass_description.data(),
           .dependencyCount = static_cast<uint32_t>(_render_pass_info.getSubpassDependencies().size()),
           .pDependencies = _render_pass_info.getSubpassDependencies().data()
       };

       assert(vkCreateRenderPass(_ptr_device->getDeviceHandle(), &render_pass_create_info, nullptr, &_render_pass_handle) == VK_SUCCESS);
       assert(_render_pass_handle != VK_NULL_HANDLE);
    }

    inline const RenderPassInfo& RenderPass::getRenderPassInfo() const noexcept
    {
        return _render_pass_info;
    }

    inline VkRenderPass RenderPass::getRenderPassHandle() const noexcept
    {
        return _render_pass_handle;
    }

    inline shared_ptr<Device>& RenderPass::getDevice() noexcept
    {
        return _ptr_device;   
    }

    inline const shared_ptr<Device>& RenderPass::getDevice() const noexcept
    {
         return _ptr_device;
    }
}
