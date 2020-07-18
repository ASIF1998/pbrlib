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

using namespace std;

namespace pbrlib
{
    class ShaderModule;
    class PipelineLayout;
    class ComputePipeline;

    using PtrShaderModule       = shared_ptr<ShaderModule>;
    using PtrPipelineLayout     = shared_ptr<PipelineLayout>;
    using PtrComputePipeline    = shared_ptr<ComputePipeline>;

    class ComputePipeline
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param shader_module         шейдерный модуль.
         * @param ptr_pipeline_layout   указатель на PipelineLayout
        */
        ComputePipeline(
            const ShaderModule&         shader_module,
            const PtrPipelineLayout&    ptr_pipeline_layout
        );

        /**
         * @brief Конструктор.
         * 
         * @param ptr_shader_module     указатель на шейдерный модуль.
         * @param ptr_pipeline_layout   указатель на PipelineLayout
        */
        ComputePipeline(
            const PtrShaderModule&      ptr_shader_module,
            const PtrPipelineLayout&    ptr_pipeline_layout
        );

        ~ComputePipeline();

        VkPipeline                  getPipelineHandle()     const noexcept;
        PtrPipelineLayout&          getPipelineLayout()     noexcept;
        const PtrPipelineLayout&    getPipelineLayout()     const noexcept;

    private:
        VkPipeline          _pipeline_handle;
        PtrPipelineLayout   _ptr_pipeline_layout;
    };
}

#endif /* ComputePipeline_hpp */
