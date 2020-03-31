//
//  RenderPass.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 19/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef RenderPass_hpp
#define RenderPass_hpp

#include "Device.hpp"

namespace pbrlib
{
    class RenderPass;

    using PtrRenderPass = shared_ptr<RenderPass>;

    /**
     * @class SubpassDescription.
     * @brief Данный класс необходим для описания подпрохода.
    */
    class SubpassDescription
    {
    public:
        /**
         * @brief Конструктор.
         *
         * @param num_input_attachment      количество входящих подключений.
         * @param num_color_attachment      количество цветовых подключить.
         * @param num_present_attachment    количество подключений, для которых вы хотите, что бы они пережили подпроход.
        */
        SubpassDescription(
            size_t num_input_attachment   = 1,
            size_t num_color_attachment   = 1,
            size_t num_present_attachment = 1
        );

        SubpassDescription(SubpassDescription&& subpass_descriptoin);
        SubpassDescription(const SubpassDescription& subpass_descriptoin);

        void addInputAttachment(uint32_t attachment, VkImageLayout layout);
        void addColorAttachment(uint32_t attachment, VkImageLayout layout);
        void addPresentAttachment(uint32_t attachment);
        void setDepthStencilAttachment(uint32_t attachment, VkImageLayout layout);
        void resetDepthStencilAttahment() noexcept;

        vector<VkAttachmentReference>&       getInputAttachment()        noexcept;
        const vector<VkAttachmentReference>& getInputAttachment()        const noexcept;
        vector<VkAttachmentReference>&       getColorAttachment()        noexcept;
        const vector<VkAttachmentReference>& getColorAttachment()        const noexcept;
        vector<uint32_t>&                    getPresentAttachment()      noexcept;
        const vector<uint32_t>&              getPresentAttachment()      const noexcept;
        VkAttachmentReference&               getDepthStencilAttachment() noexcept;
        const VkAttachmentReference&         getDepthStencilAttachment() const noexcept;

        /**
         * @brief Метод сообщающий о том, использует ли подпроход подключение глубина-трафарет.
         *
         * @return true - в случае если испоьлзует, иначе false.
        */
        bool useDepthStencilAttachment() const noexcept;

    private:
        vector<VkAttachmentReference>   _input_attachment;
        vector<VkAttachmentReference>   _color_attachment;
        vector<uint32_t>                _present_attachment;
        VkAttachmentReference           _depth_stencil_attachment;
        bool                            _use_depth_stencil_attachment;
    };

    /**
     * @class RenderPassInfo.
     * @brief Класс предназначен для накопления информации о проходе рендера.
    */
    class RenderPassInfo
    {
    public:
        RenderPassInfo() = default;

        /**
         * @brief Конструктор.
         * @details
         *      При передачи в качестве аргумента num_subpass_descriptions отличное от нуля число,
         *      то все num_subpass_descriptions объектов будут доступны сразу же. Это касается только
         *      объектов типа SubpassDescription (описателей подпрохода).
         *
         * @param num_attribute_descriptions    количество определителей подключения.
         * @param num_subpass_descriptions      количество описателей подпрохода.
         * @param num_subpass_dependencies      количество определителей зависимостей подпроходов.
        */
        RenderPassInfo(
            size_t num_attribute_descriptions,
            size_t num_subpass_descriptions,
            size_t num_subpass_dependencies
        );

        RenderPassInfo(RenderPassInfo&& render_pass_info);
        RenderPassInfo(const RenderPassInfo&) = default;

        /**
         * @brief Метод необходимый для выделения num_subpass_description количество описателей подпрохода.
         *
         * @param num_subpass_description количество описателей подпрохода.
        */
        void setNumSubpassDescription(size_t num_subpass_description);

        /**
         * @brief Метод возвращающий i'ый описатель подпрохода.
         *
         * @param i индекс подпрохода.
         * @return i'ый описатель подпрохода.
        */
        SubpassDescription& getSubpassDescription(size_t i) noexcept;

        /**
         * @brief Метод возвращающий i'ый описатель подпрохода.
         *
         * @param i индекс подпрохода.
         * @return i'ый описатель подпрохода.
        */
        const SubpassDescription& getSubpassDescription(size_t i) const noexcept;

        /**
         * @brief Метод необходимый для добавления определения подключения.
         *
         * @param format            формат подключения.
         * @param samples           число образцов в изображении.
         * @param load_op           операция, определяющая то, что нужно делать с подключением, когда проход рендеринга начинается.
         * @param store_op          операция, определяющая то, что нужно делать с подключением, когда проход рендеринга завершается.
         * @param stencil_load_op   операция, определяющая то, что нужно делать с подключением, соответствующий трафарету,
         *                          когда проход рендеринга начинается (если подключение является совмещённым типа глубина-трафарет).
         * @param stencil_store_op  операция, определяющая то, что нужно делать с подключением, соответствующий трафарету,
         *                          когда проход рендеринга завершается (если подключение является совмещённым типа глубина-трафарет).
         * @param initial_layout    в каком размещении будет изображение в начале прохода рендеринга.
         * @param final_layout      в каком размещении изображение нужно оставить по завершению прохода рендеринга.
        */
        void addAttachmentDescription(
            VkFormat                format,
            VkSampleCountFlagBits   samples,
            VkAttachmentLoadOp      load_op,
            VkAttachmentStoreOp     store_op,
            VkAttachmentLoadOp      stencil_load_op,
            VkAttachmentStoreOp     stencil_store_op,
            VkImageLayout           initial_layout,
            VkImageLayout           final_layout
        );

        /**
         * @brief Метод добавляющий описатель подпрохода.
         *
         * @param subpass_description ссылка на объект типа SubpassDescription.
        */
        void addSubpassDescription(SubpassDescription&& subpass_description);

        /**
         * @brief Метод добавляющий описатель подпрохода.
         *
         * @param subpass_description константная ссылка на объект типа SubpassDescription.
        */
        void addSubpassDescription(const SubpassDescription& subpass_description);

        /**
         * @brief Метод добавляющий определитель зависимостей подпроходов.
         *
         * @param src_subpass       ссылка на исходный подпроход.
         * @param dst_subpass       ссылка на целевой подпроход.
         * @param src_stage_mask    маска, задающая, какие стадии конвейера в исходном подпроходе создают данные.
         * @param dst_stage_mask    маска, задающая стадии подпрохода, поглощающие эти данные.
         * @param src_access_mask   задаёт, как исходный подпроход обращался к данным.
         * @param dst_access_mask   задаёт, как целевой подпроход будет обращаться к данным.
        */
        void addSubpassDependency(
            uint32_t                src_subpass,
            uint32_t                dst_subpass,
            VkPipelineStageFlags    src_stage_mask,
            VkPipelineStageFlags    dst_stage_mask,
            VkAccessFlags           src_access_mask,
            VkAccessFlags           dst_access_mask
        );

        vector<VkAttachmentDescription>&         getAttachmentDescriptions() noexcept;
        const vector<VkAttachmentDescription>&   getAttachmentDescriptions() const noexcept;
        vector<SubpassDescription>&              getSubpassDescriptions()    noexcept;
        const vector<SubpassDescription>&        getSubpassDescriptions()    const noexcept;
        vector<VkSubpassDependency>&             getSubpassDependencies()    noexcept;
        const vector<VkSubpassDependency>&       getSubpassDependencies()    const noexcept;

    private:
        vector<VkAttachmentDescription> _attribute_descriptions;
        vector<SubpassDescription>      _subpass_descriptions;
        vector<VkSubpassDependency>     _subpass_dependencies;
    };

    class RenderPass
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_device        указатель на устройство.
         * @param render_pass_info  информация о проходе рендера.
        */
        RenderPass(const PtrDevice& ptr_device, const RenderPassInfo& render_pass_info);

        /**
         * @brief Конструктор.
         * 
         * @param ptr_device        указатель на устройство.
         * @param render_pass_info  информация о проходе рендера.
        */
        RenderPass(const PtrDevice& ptr_device, RenderPassInfo&& render_pass_info);

        RenderPass(RenderPass&& render_pass);
        RenderPass(const RenderPass&) = delete;

        ~RenderPass() noexcept;

        RenderPass& operator = (RenderPass&&)       = delete;
        RenderPass& operator = (const RenderPass&)  = delete;

        const RenderPassInfo&        getRenderPassInfo()         const noexcept;
        const VkRenderPass&          getRenderPassHandle()       const noexcept;
        PtrDevice&          getDevice()                 noexcept;
        const PtrDevice&    getDevice()                 const noexcept;

        /**
         * @brief Статический метод создающий объект типа PtrRenderPass.
         * 
         * @param ptr_device указатель на устройство.
         * @param render_pass_info информация о проходе рендера.
        */
        static PtrRenderPass make(
            const PtrDevice&   ptr_device, 
            const RenderPassInfo&       render_pass_info
        );

        /**
         * @brief Статический метод создающий объект типа PtrRenderPass.
         * 
         * @param ptr_device указатель на устройство.
         * @param render_pass_info информация о проходе рендера.
        */
        static PtrRenderPass make(
            const PtrDevice&   ptr_device, 
            RenderPassInfo&&            render_pass_info
        );

    private:
        void _create();

    private:
        RenderPassInfo      _render_pass_info;
        VkRenderPass        _render_pass_handle;
        PtrDevice  _ptr_device;
    };
}

#endif /* RenderPass_hpp */
