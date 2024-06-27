//
//  ComputePipeline.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 16/07/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef ComputePipeline_hpp
#define ComputePipeline_hpp

#include <vulkan/vulkan.h>

#include <memory>

namespace pbrlib
{
    class ShaderModule;
    class PipelineLayout;
}

namespace pbrlib
{
    class ComputePipeline
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param shader_module         шейдерный модуль.
         * @param ptr_pipeline_layout   указатель на PipelineLayout.
        */
        ComputePipeline(
            const ShaderModule&                     shader_module,
            std::shared_ptr<const PipelineLayout>   ptr_pipeline_layout
        );

        /**
         * @brief Конструктор.
         * 
         * @param ptr_shader_module     указатель на шейдерный модуль.
         * @param ptr_pipeline_layout   указатель на PipelineLayout
        */
        ComputePipeline(
            std::shared_ptr<const ShaderModule>     ptr_shader_module,
            std::shared_ptr<const PipelineLayout>   ptr_pipeline_layout
        );

        ComputePipeline(ComputePipeline&& pipeline);
        ComputePipeline(const ComputePipeline&) = delete;

        ~ComputePipeline();

        ComputePipeline& operator = (ComputePipeline&&)         = delete;
        ComputePipeline& operator = (const ComputePipeline&)    = delete;

        VkPipeline                              getPipelineHandle()     const noexcept;
        std::shared_ptr<const PipelineLayout>   getPipelineLayout()     const noexcept;

        /**
         * @brief Статический метод, создающий объект типа ComputePipeline.
         * 
         * @param shader_module         шейдерный модуль.
         * @param ptr_pipeline_layout   указатель на PipelineLayout.
         * @return Указатель на ComputePipeline.
        */
        static std::unique_ptr<ComputePipeline> make(
            const ShaderModule&                     shader_module,
            std::shared_ptr<const PipelineLayout>   ptr_pipeline_layout
        );

    private:
        VkPipeline                              _pipeline_handle;
        std::shared_ptr<const PipelineLayout>   _ptr_pipeline_layout;
    };
}

#endif /* ComputePipeline_hpp */
