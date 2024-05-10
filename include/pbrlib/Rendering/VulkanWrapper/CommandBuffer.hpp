//
//  CommandBuffer.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 07/03/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef CommandBuffer_hpp
#define CommandBuffer_hpp

#include "CommandPool.hpp"

#include "GraphicsPipeline.hpp"

#include <memory>
#include <vector>

#include <pbrlib/math/vec4.hpp>

using namespace std;

namespace pbrlib
{
    class RenderPass;
    class DescriptorSet;
    class Buffer;
    class Image;
    class CommandPool;
    class CommandBuffer;
    class PrimaryCommandBuffer;
    class SecondaryCommandBuffer;
    class Framebuffer;
    class ComputePipeline;

    using PtrCommandBuffer          = shared_ptr<CommandBuffer>;
    using PtrPrimaryCommandBuffer   = shared_ptr<PrimaryCommandBuffer>;
    using PtrSecondaryCommandBuffer = shared_ptr<SecondaryCommandBuffer>;
    using PtrFramebuffer            = shared_ptr<Framebuffer>;
    using PtrComputePipeline        = shared_ptr<ComputePipeline>;

    using namespace math;

    class CommandBuffer
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_command_pool  указатель на пул команд.
         * @param level             уровень командного буфера.
        */
        CommandBuffer(
            const PtrCommandPool&   ptr_command_pool, 
            VkCommandBufferLevel    level
        );

        CommandBuffer(CommandBuffer&& command_buffer);
        CommandBuffer(const CommandBuffer&&) = delete;

        ~CommandBuffer();

        CommandBuffer& operator = (CommandBuffer&&)         = delete;
        CommandBuffer& operator = (const CommandBuffer&&)   = delete;

        /**
         * @brief 
         *      Метод, позволяющий использовать буферы как источник 
         *      вершинных данных (их ещё называют вершинными буферами).
         * 
         * @param first_binding индекс первой привязки.
         * @param buffers       вектор привязываемых буферов.
         * @param offsets       вектор со смещениями в буферах (задаётся в байтах).
         * 
         * @throw в случае, если offsets.size() < buffer_handles.size().
        */
        void bindVertexBuffers(
            uint32_t 		            first_binding, 
            const vector<Buffer>& 	    buffers, 
            const vector<VkDeviceSize>& offsets
    	) const;

        /**
         * @brief 
         *      Метод, позволяющий использовать буфер как источник
         *      вершинных данных (его ещё называют вершинным буфером).
         * 
         * @param buffer    привязываемый буфер.
         * @param offset    смещениями в буфере (задаётся в байтах).
        */
        void bindVertexBuffer(const Buffer& buffer, VkDeviceSize offset) const noexcept;

        /**
         * @brief Метод, позволяющий использовать буфер как индексный буфер.
         * @details 
         *      Когда вы вызываете команду для индексированной отрисовки, Vulkan
         *      начинает читать данные следующим способом:
         *          offset + index * sizeof(index_type).
         *      Если index_type = VK_INDEX_TYPE_UINT16, то sizeof(index_type) = 2.
         *      Если index_type = VK_INDEX_TYPE_UINT32, то sizeof(index_type) = 4.
         * 
         * @param index_buffer  буфер содержащий индексные данные.
         * @param offset        смещение (с помощью него может быть привязана только часть буфера).
         * @param index_type    тип данных для индексов.
        */
        void bindIndexBuffer(const Buffer& index_buffer, VkDeviceSize offset, VkIndexType index_type) const noexcept;

        /**
         * @brief Метод, позволяющий привязать набор дескрипторов к командному буферу.
         * 
         * @param ptr_graphics_pipeline указатель на графический конвейер.
         * @param descriptor_set        множество дескрипторов.
        */
        void bindDescriptorSet(
            const PtrGraphicsPipeline&  ptr_graphics_pipeline,
            const DescriptorSet&        descriptor_set
        );

        /**
         * @brief Метод, позволяющий привязать набор дескрипторов к командному буферу.
         * 
         * @param ptr_compute_pipeline  указатель на вычислительный конвейер.
         * @param descriptor_set        множество дескрипторов.
        */
        void bindDescriptorSet(
            const PtrComputePipeline&   ptr_compute_pipeline,
            const DescriptorSet&        descriptor_set
        );

        /**
         * @brief Метод, привязывающий буфер команд к конвейеру.
         * 
         * @param ptr_pipeline указатель на графический конвейер.
        */
        void bindToPipeline(const PtrGraphicsPipeline& ptr_pipeline) const;

        /**
         * @brief Метод, привязывающий буфер команд к конвейеру.
         * 
         * @param ptr_pipeline указатель на вычислительный конвейер.
        */
        void bindToPipeline(const PtrComputePipeline& ptr_pipeline) const;

        /**
         * @brief Метод, позволяющий создать команду для отрисовки.
         * 
         * @param vertex_count      число вершин.
         * @param instance_count    количество экземпляров для отрисовки.
         * @param first_vertex      индекс вершины после которой идут остальные вершины.
         * @param first_instance    индекс первого экземпляра.
        */
        void draw(
            uint32_t vertex_count, 
            uint32_t instance_count, 
            uint32_t first_vertex, 
            uint32_t first_instance
        ) const noexcept;
        
        /**
         * @brief Метод, позволяющий создать команду для индексированной отрисовки.
         * 
         * @param index_count       количество индексов.
         * @param instance_count    количество экземпляров для отрисовки.
         * @param first_index       первый индекс, после которого идут все остальные индексы.
         * @param vertex_offset     значение, добавляемое к индексу вершины перед индексацией в буфер вершин.
         * @param first_instance    индекс первого экземпляра.
        */
        void drawIndexed(
            uint32_t    index_count,
            uint32_t    instance_count,
            uint32_t    first_index,
            int32_t     vertex_offset, 
            uint32_t    first_instance
        ) const noexcept;

        /**
         * @brief Метод, позволяющий осуществлять косвенную индексную отрисовку.
         * @details 
         *      Экземпляр структуры, которая описывает параметры отрисовки:
         *          struct IndirectIndexRenderingOptions
         *          {
         *              uint32_t    index_count;
         *              uint32_t    instance_count;
         *              uint32_t    first_index;
         *              int32_t     vertex_offset;
         *              uint32_t    first_instance;
         *          };
         * 
         * @param buffer        буфер содержащий параметры отрисовки.
         * @param offset        смещение в байтах в buffer, начиная с которого параметры расположены.
         * @param draw_count    количество запусков отдельных операций рендеринга.
         * @param stride        количество байт, отделяющий одну структуру от другой в buffer (в байтах).
        */
        void drawIndexedInderect(
            const Buffer&   buffer,
            VkDeviceSize    offset,
            uint32_t        draw_count,
            uint32_t        stride
        ) const;

        /**
         * @brief Метод, позволяющий запускать глобальную рабочую группу с использованеим вычислитлельного конвейера.
         * 
         * @param group_count_x число локальных рабочих групп по оси X.
         * @param group_count_y число локальных рабочих групп по оси Y.
         * @param group_count_z число локальных рабочих групп по оси Z.
        */
        void dispatch(uint32_t group_count_x, uint32_t group_count_y, uint32_t group_count_z) const noexcept;

        /**
         * @brief Метод, позволяющий создать команду для обновления содержимого буфера.
         * 
         * @param dst_buffer    обновляемый буфер.
         * @param data          данные, которые будут записаны в буфер.
         * @param dst_offset    байтовое смещение в буфере.
        */
        template<class Collection>
        void updateBuffer(
            const Buffer&       dst_buffer,
            const Collection&   data,
            VkDeviceSize        dst_offset = 0
        ) const;

        /**
         * @brief Метод, позволяющий создать команду для обновления содержимого буфера.
         * 
         * @param dst_buffer    обновляемый буфер.
         * @param ptr_data      указатель на данные, которые будут записаны в буфер.
         * @param size_data     размер массива, на который указывает ptr_data (в байтах).
         * @param dst_offset    байтовое смещение в буфере.
        */
        template<class Pointer>
        void updateBuffer(
            const Buffer&       dst_buffer,
            const Pointer       ptr_data,
            const size_t        size_data,
            VkDeviceSize        dst_offset = 0
        ) const noexcept;

        /**
         * @brief 
         *      Метод, позволяющий создать команду для осуществления
         *      копирования из одного буфера в другой.
         * 
         * @param src_buffer    источник данных.
         * @param dst_buffer    буфер в который будут помещены данные из src_buffer.
         * @param size          количество байтов, необходимых для копирования.
        */
        void copyBuffer(
            const Buffer&   src_buffer,
            const Buffer&   dst_buffer,
            VkDeviceSize    size
        ) const noexcept;

        /**
         * @brief 
         *      Метод, позволяющий создать команду для осуществления
         *      копирования из одного буфера в другой.
         * 
         * @param src_buffer    источник данных.
         * @param dst_buffer    буфер в который будут помещены данные из src_buffer.
         * @param regions       регионы для копирования.
        */
        void copyBuffer(
            const Buffer&                   src_buffer,
            const Buffer&                   dst_buffer,
            const vector<VkBufferCopy>&     regions
        ) const noexcept;

        /**
         * @brief 
         *      Метод, позволяющий создать команду для осуществления
         *      копирования из одного изображения в другое.
         * 
         * @param src_image    источник данных.
         * @param dst_image    изображение в которое будут помещены данные из src_image.
         * @param region       регион для копирования.
        */
        void copyImage(
            const Image&                src_image,
            const Image&                dst_image,
            const VkImageCopy&          region
        );

        /**
         * @brief 
         *      Метод, позволяющий создать команду для осуществления
         *      копирования из одного изображения в другое.
         * 
         * @param src_image    источник данных.
         * @param dst_image    изображение в которое будут помещены данные из src_image.
         * @param regions      регионы для копирования.
        */
        void copyImage(
            const Image&                src_image,
            const Image&                dst_image,
            const vector<VkImageCopy>&  regions
        );

        /**
         * @brief Метод, позволяющий копировать данные из буфера в изображение.
         * 
         * @param src_buffer                    буфер источник.
         * @param src_buffer_offset             смещение в буфере.
         * @param dst_image                     изображение, в которое будет осуществляться копирование.
         * @param dst_image_subresource_layers  указывает подресурс, в который копируются данные.
         * @param dst_image_offset              смещение в изображение.
         * @param dst_image_extent              размер.
        */
        void copyBufferToImage(
            const Buffer&               src_buffer,
            uint32_t                    src_buffer_offset,
            const Image&                dst_image,
            VkImageLayout               dst_image_layout,
            VkImageSubresourceLayers    dst_image_subresource_layers,
            VkOffset3D                  dst_image_offset,
            VkExtent3D                  dst_image_extent
        );

        /**
         * @brief 
         *      Метод, позволяющий создавать команду для перевода
         *      изображения из одного состояния в другое.
         * 
         * @param src_stage_mask            определяет когда исходная стадия осуществила чтение или запись ресурса.
         * @param dst_stage_mask            задаёт точку в конвейере, в которой конвейер будет ждать перед продолжением.
         * @param src_access_mask           тип доступа для источника.
         * @param dst_access_mask           тип доступа для цели.
         * @param old_image_layout          размещение изображения до барьера.
         * @param new_image_layout          размещение изображения после барьера.
         * @param src_queue_family_index    индекс исходного семейства очереди.
         * @param dst_queue_family_index    индекс целевого семейства очереди.
         * @param image                     изображение, на которое влияет барьер.
         * @param subresource_range         указывает части изображения, на которое влияет барьер.
        */
        void imageMemoryBarrier(
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
        ) const noexcept;

        /**
         * @brief 
         *      Метод, позволяющий создавать команду для перевода
         *      буфера из одного состояния в другое.
         * 
         * @param src_stage_mask            определяет когда исходная стадия хавершила чтение или запись ресурса.
         * @param dst_stage_mask            задаёт точку в конвейере, в которой конвейер будет ждать перед продолжением.
         * @param src_access_mask           тип доступа для источника.
         * @param dst_access_mask           тип доступа для цели.
         * @param src_queue_family_index    индекс исходного семейства очереди.
         * @param dst_queue_family_index    индекс целевого семейства очереди.
         * @param buffer                    изображение, на которое влияет барьер.
         * @param offset                    смещение в buffer   (используется для синхронизации к диапазону внутри буфера).
         * @param size                      размер в байтах     (используется для синхронизации к диапазону внутри буфера).
        */
        void bufferMemoryBarrier(
            VkPipelineStageFlags            src_stage_mask,
            VkPipelineStageFlags            dst_stage_mask,
            VkAccessFlags                   src_access_mask,
            VkAccessFlags                   dst_access_mask,
            uint32_t                        src_queue_family_index,
            uint32_t                        dst_queue_family_index,
            const Buffer&                   buffer,
            VkDeviceSize                    offset,
            VkDeviceSize                    size
        ) const noexcept;

        /**
         * @brief Метод, позволяющий очистить изображение.
         * 
         * @param image                     изображение.
         * @param image_layout              размещение изображения.
         * @param clear_color               цвет, которым будет очищаться изображение.
         * @param image_subresource_range   указывает части изображения, которые будут очищаться.
        */
        void clearColorImage(
            const Image&                    image,
            VkImageLayout                   image_layout,
            const Vec4<float>&              clear_color,
            const VkImageSubresourceRange&  image_subresource_range
        ) const noexcept;

        /**
         * @brief Метод, позволяющий очистить изображение.
         * 
         * @param image                     изображение.
         * @param image_layout              размещение изображения.
         * @param depth_val                 значение глубины, которым будет осуществляться очистка изображения.
         * @param stencil_val               значение трафарета, которым будет осуществляться очистка изображения.
         * @param image_subresource_range   указывает части изображения, которые будут очищаться.
        */
        void clearDepthStencilImage(
            const Image&                    image,
            VkImageLayout                   image_layout,
            float                           depth_val,
            uint32_t                        stencil_val,
            const VkImageSubresourceRange&  image_subresource_range
        ) const noexcept;

        /**
         * @brief Метод, сбрасывающий буфер команд в исходное состояние.
        */
        void reset() const noexcept;

        PtrDevice&               getDevice()                 noexcept;
        const PtrDevice&         getDevice()                 const noexcept;
        const VkCommandBuffer&   getCommandBufferHandle()    const noexcept;

    protected:
        PtrCommandPool  _ptr_command_pool;
        VkCommandBuffer _command_buffer_handle;
    };

    class PrimaryCommandBuffer :
        public CommandBuffer
    {
    public:
        PrimaryCommandBuffer(const PtrCommandPool& ptr_command_pool);

        PrimaryCommandBuffer(PrimaryCommandBuffer&& command_buffer);
        PrimaryCommandBuffer(const PrimaryCommandBuffer&) = delete;

        PrimaryCommandBuffer& operator = (PrimaryCommandBuffer&&)       = delete;
        PrimaryCommandBuffer& operator = (const PrimaryCommandBuffer&)  = delete;

        void begin()                                                                                            const;
        void end()                                                                                              const;
        void begineRenderPass(const PtrFramebuffer& ptr_framebuffer, const vector<VkClearValue>& clear_values)  const noexcept;
        void begineRenderPass(const PtrFramebuffer& ptr_framebuffer, const VkClearValue& clear_value)           const noexcept;
        void begineRenderPass(const PtrFramebuffer& ptr_framebuffer)                                            const noexcept;
        void endRenderPass()                                                                                    const noexcept;
        void nextSubpass()                                                                                      const noexcept;

        const PtrGraphicsPipeline& getPipeline() const noexcept;

        static PtrPrimaryCommandBuffer make(const PtrCommandPool& ptr_command_pool);

    private:
        PtrGraphicsPipeline _ptr_pipeline;
    };

    class SecondaryCommandBuffer :
        public CommandBuffer
    {
    public:
        SecondaryCommandBuffer(const PtrCommandPool& ptr_command_pool);

        SecondaryCommandBuffer(SecondaryCommandBuffer&& command_buffer);
        SecondaryCommandBuffer(const SecondaryCommandBuffer&) = delete;

        SecondaryCommandBuffer& operator = (SecondaryCommandBuffer&&)       = delete;
        SecondaryCommandBuffer& operator = (const SecondaryCommandBuffer&)  = delete;

        /**
         * @brief Метод позволяющий начать запись в командный буфер.
         * @details 
         *      primary_command_buffer необходим для того, что бы наследовать 
         *      его состояния (это должен быть тот командный буфер, который
         *      вызовет этот).
         * 
         * @param primary_command_buffer    первичный командный буфер.
         * @param ptr_framebuffer           указатель на кадровый буфер.
        */
        void begin(const PrimaryCommandBuffer& primary_command_buffer, const PtrFramebuffer& ptr_framebuffer) const;
        
        void end() const;

        static PtrSecondaryCommandBuffer make(const PtrCommandPool& ptr_command_pool);
    };
}

#endif /* CommandBuffer_hpp */
