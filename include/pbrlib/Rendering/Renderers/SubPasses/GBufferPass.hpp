//
//  GBufferPass.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 20/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef GBufferPass_hpp
#define GBufferPass_hpp

#include <pbrlib/SceneGraph/Scene.hpp>

#include "Pass.hpp"

#include <memory> 

namespace pbrlib
{
    class DescriptorPool;

    using PtrAttachments = std::shared_ptr<std::vector<ImageView>>;
}

namespace pbrlib
{

    /**
     * @class GBufferPass.
     * @brief Данный класс позволяет создавать g-buffer.
     * @details 
     *      Для работы с этим классом, проход рендера должен 
     *      содержать следующие подключения:
     *          1) для позиции (vec3) и металичности (float);
     *          2) для нормали (vec3) и шероховатости (float);
     *          3) для альбедо (vec3) и запечённого AO (float);
     *          4) для параметра анизотропности (float).
    */
    class GBufferPass :
        public IPassOutput<GBufferPass>
    {
    public:
        struct OutputImagesViewsIDs
        {
            enum :
                size_t
            {
                PositionAndMetallic = 0,
                NormalAndRoughness,
                AlbedoAndBakedAO,
                Depth,
                TangentAndAnisotropy,

                Count
            };
        };

        class Builder
        {
        public:
            Builder() = default;

            Builder(Builder&&)      = delete;
            Builder(const Builder&) = delete;

            Builder& operator = (Builder&&)         = delete;
            Builder& operator = (const Builder&)    = delete;

            void setDevice(const Device* ptr_device);
            void setPhysicalDevice(const PhysicalDevice* ptr_physical_device);
            void setDescriptorPool(std::shared_ptr<const DescriptorPool> ptr_descriptor_pool);
            void setDeviceMemoryIndex(uint32_t memory_index);
            void setQueue(std::shared_ptr<DeviceQueue> ptr_queue);
            void windowSize(uint32_t width, uint32_t height);

            GBufferPass                     build();
            std::unique_ptr<GBufferPass>    buildPtr();

        private:
            const Device*                           _ptr_device;
            std::shared_ptr<const DescriptorPool>   _ptr_descriptor_pool;
            const PhysicalDevice*                   _ptr_physical_device;
            uint32_t                                _window_width;
            uint32_t                                _window_height;
            uint32_t                                _gpu_memory_index;
            std::shared_ptr<DeviceQueue>            _ptr_device_queue;
        };
        
    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_device                указатель на логическое устройство.
         * @param ptr_queue                 укзатель на очередь устройства.
         * @param ptr_physical_device       указатель на физическое устройство.
         * @param ptr_descriptor_pool       указатель на пул дескрипторов.
         * @param gpu_memory_index          индекс памяти устройства (должен поддерживать отображение).
         * @param window_width              ширина окна.
         * @param window_height             высота окна.
        */
        GBufferPass(
            const Device*                           ptr_device, 
            std::shared_ptr<DeviceQueue>            ptr_queue,
            const PhysicalDevice*                   ptr_physical_device,
            std::shared_ptr<const DescriptorPool>   ptr_descriptor_pool,  
            uint32_t                                gpu_memory_index,
            uint32_t                                window_width,
            uint32_t                                window_height
        );

        /**
         * @brief Метод, позволяющий запустить графический конвейер.
         * @details 
         *      Объект будет отображён в случае, если у него есть
         *      меш с материалом.
         * 
         * @param projection            проекция.
         * @param view                  преобразование в пространство вида (камеры).
         * @param drawable_objects      отображаемые объекты.
         * @param ptr_command_buffer    указатель на командный буфер.
         * @param ptr_sampler           указатель на сэмплер.
        */
        void draw(
            const Transform&                            projection,
            const Transform&                            view,
            const VisibleList&                          drawable_objects, 
            std::shared_ptr<const PrimaryCommandBuffer> ptr_command_buffer,
            std::shared_ptr<const Sampler>              ptr_sampler
        );

        std::shared_ptr<const GraphicsPipeline> getPipeline()       const noexcept;
        std::shared_ptr<const DescriptorSet>    getDescriptorSet()  const noexcept;
        std::shared_ptr<const Framebuffer>      getFramebuffer()    const noexcept;

        /**
         * @brief Метод, позволяющий создавать указатель на объект типа GBufferPass.
         * 
         * @param ptr_device                указатель на логическое устройство.
         * @param ptr_queue                 укзатель на очередь устройства.
         * @param ptr_physical_device       указатель на физическое устройство.
         * @param ptr_descriptor_pool       указатель на пул дескрипторов.
         * @param gpu_memory_index          индекс памяти устройства (должен поддерживать отображение).
         * @param window_width              ширина окна.
         * @param window_height             высота окна.
         * @return Указатель на GBufferPass.
        */
        static std::unique_ptr<GBufferPass> make(
            const Device*                           ptr_device, 
            std::shared_ptr<DeviceQueue>            ptr_queue,
            const PhysicalDevice*                   ptr_physical_device,
            std::shared_ptr<const DescriptorPool>   ptr_descriptor_pool,  
            uint32_t                                gpu_memory_index,
            uint32_t                                window_width,
            uint32_t                                window_height
        );

    private:
        friend class IPassOutput<GBufferPass>;

        const ImageView&    outputImpl(size_t id) const;
        void                outputImpl(std::shared_ptr<const ImageView> ptr_image_view, size_t id);

    private:
        std::shared_ptr<const GraphicsPipeline> _ptr_pipeline;
        std::shared_ptr<DeviceQueue>            _ptr_device_queue;
        std::shared_ptr<const DescriptorSet>    _ptr_descriptor_set;
        std::shared_ptr<Buffer>                 _ptr_uniform_matrices_data_buffer;
        std::shared_ptr<const RenderPass>       _ptr_render_pass;
        PtrAttachments                          _ptr_framebuffer_attachments;
        std::shared_ptr<Buffer>                 _ptr_uniform_material_data_buffer;
        std::shared_ptr<const Framebuffer>      _ptr_framebuffer;
    };
}

#endif /* GBufferPass_hpp */
