//
//  GraphicsPipeline.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 24/02/2020.
//
//  Copyright © 2020 Асиф Мамедов. All rights reserved.

#ifndef GraphicsPipeline_hpp
#define GraphicsPipeline_hpp

#include "PipelineLayout.hpp"
#include "RenderPass.hpp"

#include <vector>
#include <memory>

namespace pbrlib
{
    class GraphicsPipeline;
    class ShaderModule;

    using PtrGraphicsPipeline = std::shared_ptr<GraphicsPipeline>;

    /**
     * @class VertexInputState.
     * @brief Класс описывающий состояние входных данных вершин.
    */
    class VertexInputState 
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param num_vertex_biding_descriptions    число вершинных привязок используемых конвейером.
         * @param num_vertex_attribute_descriptions число атрибутов вершины.
        */
        VertexInputState(
            size_t num_vertex_biding_descriptions       = 0,
            size_t num_vertex_attribute_descriptions    = 0
        );

        VertexInputState(VertexInputState&& vertex_input_state);
        VertexInputState(const VertexInputState& vertex_input_state);

        ~VertexInputState();

        VertexInputState& operator = (VertexInputState&&)       = delete;
        VertexInputState& operator = (const VertexInputState&)  = delete;

        /**
         * @brief Метод необходимый для добавления привязки.
         * 
         * @param binding       индекс привязки.
         * @param stride        шаг в массиве (каждая привязка может рассматриваться как массив структур, размещённых в буфере). 
         * @param input_rate    задаёт способ обращения либо индексу вершины, либо по индексу экземпляра.
         * 
         * @throw В случае если количество привязок больше или равняется capacityVertexInputBindingDescription().
        */
        void addVertexInputBindingDescription(uint32_t binding, uint32_t stride, VkVertexInputRate input_rate);

        /**
         * @brief Метод необходимый для добавления атрибута.
         * 
         * @param location  положение, по которому вершинный шейдер обращается к атрибуту.
         * @param binding   индекс привязки, из которой этот атрибут берёт данные.
         * @param format    формат атрибута.
         * @param offset    смещение внутри каждой структуры.
         * 
         * @throw В случае если количество атрибутов больше или равняется capacityVertexInputAttributeDescription().
        */
        void addVertexInputAttributeDescription(uint32_t location, uint32_t binding, VkFormat format, uint32_t offset);

        const VkVertexInputBindingDescription*      getVertexInputBindingDescriptions()         const noexcept;
        const VkVertexInputAttributeDescription*    getVertexInputAttributeDescriptions()       const noexcept;
        size_t                                      numVertexInputBindingDescription()          const noexcept;
        size_t                                      numVertexInputAttributeDescription()        const noexcept;
        size_t                                      capacityVertexInputBindingDescription()     const noexcept;
        size_t                                      capacityVertexInputAttributeDescription()   const noexcept;
        const VkPipelineVertexInputStateCreateInfo& getPipelineVertexInputStateCreateInfo()     const noexcept;
        VkPipelineVertexInputStateCreateInfo&       getPipelineVertexInputStateCreateInfo()     noexcept;

    private:
        VkVertexInputBindingDescription*        _ptr_vertex_biding_descriptions;
        VkVertexInputAttributeDescription*      _ptr_vertex_attribute_descriptions;
        size_t                                  _curent_vertex_biding_description;
        size_t                                  _curent_vertex_attribute_description;
        VkPipelineVertexInputStateCreateInfo    _pipeline_vertex_input_state_ci;
    };

    /**
     * @class MultisampleState.
     * @brief Класс описывающий состояние мультисэмплинга.
    */
    class MultisampleState
    {
    public:
        MultisampleState(
            VkSampleCountFlagBits   num_samples                 = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
            VkBool32                sample_shading_enable       = VK_FALSE,
            float                   min_sample_shading          = 1.0f,
            VkBool32                alpha_to_coverage_enable    = VK_FALSE,
            VkBool32                alpha_to_one_enable         = VK_FALSE
        );

        MultisampleState(const MultisampleState& multisample_state) noexcept;

        MultisampleState& operator = (MultisampleState&&)       = delete;
        MultisampleState& operator = (const MultisampleState&)  = delete;

        void setSampleMask(VkSampleMask sample_mask) noexcept;

        /**
         * @brief 
         *      Метод позволяющий установить количество выборок,
         *      используемых при растеризации.
         * 
         * @param num_samples количество выборок.
        */
        void setRasterizationSamples(VkSampleCountFlagBits num_samples) noexcept;

        /**
         * @brief Метод позволяющий установить минимальную долю затенеия образца.
         * @details
         *      Если min_sample_shading = 1.0, то каждый образец внутри пикселя
         *      гарантированно получает свои данные от своего вызова фрагментного 
         *      шейдрера. Любое значение меньшее чем 1.0 привод к тому, что 
         *      устройство будет вычислять цвета как минимум в этом количестве
         *      образцов и затем распределять цвета зависящим от устройства способом.
         *      Например если min_sample_shading = 0.5, то полностью покрытый 
         *      пиксель с 8 образцами выполнит фрагментный шейдер как минимум 4
         *      раза для 4 наборов выходных значений и распределит их среди 8
         *      образцов зависящим от устройства способом.
         * 
         * @param min_sample_shading минимальная доля затенения образца.
         * 
         * @throw В случае если min_sample_shading < 0 или min_sample_shading > 1.
        */
        void setMinSampleShading(float min_sample_shading) noexcept;

        const VkPipelineMultisampleStateCreateInfo& getPipelineMultisampleStateCreateInfo() const noexcept;
        VkPipelineMultisampleStateCreateInfo&       getPipelineMultisampleStateCreateInfo() noexcept;
        
        /**
         * @brief Метод позволяющий включать или выключать закрашивание с частотой образцов.
         * 
         * @param is_enable флаг.
        */
        void sampleShadingEnable(VkBool32 is_enable) noexcept;

        void alphaToCoverageEnable(VkBool32 is_enable) noexcept;

        /**
         * @brief 
         *      Метод позволяющий заменять альфа-канал цветовых буферов
         *      максимальным значением для типов с фиксированной запятой
         *      и 1.0 для типов с плавающей запятой. В противном случае 
         *      альфа-компанент не меняется.
         * 
         * @param is_enable флаг.
        */
        void alphaToOneEnable(VkBool32 is_enable) noexcept;

        VkSampleMask             getSampleMask()             const noexcept;
        VkSampleCountFlagBits    getRasterizationSamples()   const noexcept;
        float                    getMinSampleShading()       const noexcept;
        VkBool32                 sampleShadingEnable()       const noexcept;
        VkBool32                 alphaToCoverageEnable()     const noexcept;
        VkBool32                 alphaToOneEnable()          const noexcept;

    private:
        VkSampleMask                            _sample_mask;
        VkPipelineMultisampleStateCreateInfo    _pipeline_multisample_state_ci;
    };

    /**
     * @class RasterizationState.
     * @brief Класс описывающий состояние растеризации.
    */
    class RasterizationState
    {
    public:
        RasterizationState();

        RasterizationState(RasterizationState&&)        = default;
        RasterizationState(const RasterizationState&)   = default;

        RasterizationState& operator = (RasterizationState&&)       = delete;
        RasterizationState& operator = (const RasterizationState&)  = delete;

        /**
         * @brief Метод позволяющий установить режим отображения треугольника.
         * 
         * @param polygon_mode режим отображения треугольника.
        */
        void setPolygonMode(VkPolygonMode polygon_mode) noexcept;

        /**
         * @brief Метод позволяющий установить режим отбраковки треугольников.
         * 
         * @param cull_mode режим отбраковки треугольников.
        */
        void setCullMode(VkCullModeFlags cull_mode) noexcept;

        /**
         * @brief 
         *      Метод позволяющий установить какое направление обхода вершин 
         *      (по часовой стрелке) является лицевым (в системе координат окна).
         * 
         * @param front_face определяет ориентацию треугольника.
        */
        void setFrontFace(VkFrontFace front_face) noexcept;
        
        /**
         * @brief 
         *      Метод позволяющий установить значение постоянной глубины, 
         *      добавляемый к каждому фрагменту.
         * 
         * @param depth_bias_constant_factor постоянная глубина.
        */
        void setDepthBiasConstantFactor(float depth_bias_constant_factor) noexcept;

        /**
         * @brief Метод позволяющий установить максимальную глубину.
         * 
         * @param depth_bias_clamp максимальная глубина.
        */
        void setDepthBiasClamp(float depth_bias_clamp) noexcept;

        /**
         * @brief 
         *      Метод позволяющий установить коэффициент, применяемый к 
         *      наклону в расчётах смещения глубины.
         * 
         * @param depts_bias_slope_factor коэффициент, применяемый к наклону.
        */
        void setDepthBiasSlopeFactor(float depts_bias_slope_factor) noexcept;

        /**
         * @brief Метод позволяющий установить толщину отрезка в пикселях.
         * 
         * @param line_width толщина отрезка в пикселях.
        */
        void setLineWidth(float line_width) noexcept;

        /**
         * @brief Метод позволяющий включать или отключать смещение глубины.
         * 
         * @param is_enable флаг.
        */
        void depthClampEnable(VkBool32 is_enable) noexcept;

        /**
         * @brief Метод позволяющий отключить или включить растеризацию.
         * 
         * @param is_enable флаг. Если он равен  VK_TRUE, то растеризация отключается.
        */
        void rasterizerDiscardEnable(VkBool32 is_enable) noexcept;

        /**
         * @brief 
         *      Метод позволяющий определить, следует ли смещать значение 
         *      глубины фрагмента.
         * 
         * @details 
         *      Эта возможность позволяет осуществлять смещение глубины
         *      перед тестом глубины. 
         * 
         *      Если is_enable = VK_TRUE, то при помощи методов 
         *      setDepthBiasConstantFactor(...), setDepthBiasClamp(...) и
         *      setDepthBiasSlopeFactor(...) можно инициализировать переменные
         *      depthBiasConstantFactor, depthBiasClamp и depthBiasSlopFactor и
         *      вычислить значение смещения глубины:
         *          o = m × depthBiasSlopFactor + r × depthBiasSlopFactor,
         *      где r - минимально допустимое смещение значения глубины для примитива.
         *      depthBiasClamp является верхней границей для o.
         * 
         * @param is_enable флаг.
        */
        void depthBiasEnable(VkBool32 is_enable) noexcept;

        const VkPipelineRasterizationStateCreateInfo&   getPipelineRasterizationStateCreateInfo()   const noexcept;
        VkPipelineRasterizationStateCreateInfo&         getPipelineRasterizationStateCreateInfo()   noexcept;
        VkPolygonMode                                   getPolygonMode()                            const noexcept;
        VkCullModeFlags                                 getCullMode()                               const noexcept;
        VkFrontFace                                     getFrontFace()                              const noexcept;
        float                                           getDepthBiasConstantFactor()                const noexcept;
        float                                           getDepthBiasClamp()                         const noexcept;
        float                                           getDepthBiasSlopeFactor()                   const noexcept;
        float                                           getLineWidth()                              const noexcept;
        VkBool32                                        depthClampEnable()                          const noexcept;
        VkBool32                                        rasterizerDiscardEnable()                   const noexcept;
        VkBool32                                        depthBiasEnable()                           const noexcept;

    private:
        VkPipelineRasterizationStateCreateInfo _pipeline_rasterization_state_ci;
    };

    /**
     * @class DepthStencilState.
     * @brief Класс, описывающий состояние глубины и трафарета.
    */
    class DepthStencilState
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param depth_test_enable VK_TRUE - включение теста глубины.
         * @param stencil_test_enable VK_TRUE - включение теста трафарета.
        */
        DepthStencilState(
            VkBool32 depth_test_enable      = VK_FALSE,
            VkBool32 stencil_test_enable    = VK_FALSE
        ) noexcept;

        DepthStencilState(DepthStencilState&&)      = default;
        DepthStencilState(const DepthStencilState&) = default;

        DepthStencilState& operator = (DepthStencilState&&)         = delete;
        DepthStencilState& operator = (const DepthStencilState&)    = delete;

        /**
         * @brief 
         *      Метод позволяющий установить операцию для сравнения 
         *      вычисленного значения глубины для фрагмента.
         * 
         * @param depth_compare_op операция сравнения.
        */
        void setDepthCompareOp(VkCompareOp depth_compare_op) noexcept;

        /**
         * @brief 
         *      Метод позволяющий установить состояние теста 
         *      трафарета для лицевых примитивов.
         * 
         * @param front состояние теста трафарета для лицевых примитивов.
        */
        void setFront(VkStencilOpState front) noexcept;

        /**
         * @brief 
         *      Метод позволяющий установить состояние теста 
         *      трафарета для нелицевых примитивов.
         * 
         * @param back состояние теста трафарета для нелицевых примитивов.
        */
        void setBack(VkStencilOpState back) noexcept;
        
        /**
         * @brief 
         *      Метод позволяющий установить минимальную границу 
         *      диапазона глубины. Эта граница будет использована
         *      при тесте диапазона глубины.
         * 
         * @param min_depth_bounds минимальная граница диапазона глубины.
        */
        void setMinDepthBounds(float min_depth_bounds) noexcept;

        /**
         * @brief 
         *      Метод позволяющий установить максимальную границу 
         *      диапазона глубины. Эта граница будет использована
         *      при тесте диапазона глубины.
         * 
         * @param max_depth_bounds максимальная граница диапазона глубины.
        */
        void setMaxDepthBounds(float max_depth_bounds) noexcept;

        /**
         * @brief Метод позволяющий включить или выключить тест глубины.
         * 
         * @param is_enable флаг.
        */
        void depthTestEnable(VkBool32 is_enable) noexcept;

        /**
         * @brief Метод позволяющий включить или выключить запись в буффер глубины.
         * 
         * @param is_enable флаг.
        */
        void depthWriteEnable(VkBool32 is_enable) noexcept;

        /**
         * @brief Метод позволяющий включить или выключить тест диапазона глубины.
         * 
         * @param is_enable флаг.
        */
        void depthBoundsTestEnable(VkBool32 is_enable) noexcept;

        /**
         * @brief Метод позволяющий включить или выключить тест трафарета.
         * 
         * @param is_enable флаг.
        */
        void stencilTestEnable(VkBool32 is_enable) noexcept;

        const VkPipelineDepthStencilStateCreateInfo&    getPipelineDepthStencilStateCreateInfo()    const noexcept;
        VkPipelineDepthStencilStateCreateInfo&          getPipelineDepthStencilStateCreateInfo()    noexcept;
        VkCompareOp                                     getDepthCompareOp()                         const noexcept;
        VkStencilOpState                                getFront()                                  const noexcept;
        VkStencilOpState                                getBack()                                   const noexcept;
        float                                           getMinDepthBounds()                         const noexcept;
        float                                           getMaxDepthBounds()                         const noexcept;
        VkBool32                                        depthTestEnable()                           const noexcept;
        VkBool32                                        depthWriteEnable()                          const noexcept;
        VkBool32                                        depthBoundsTestEnable()                     const noexcept;
        VkBool32                                        stencilTestEnable()                         const noexcept;

    private:
        VkPipelineDepthStencilStateCreateInfo _pipeline_depth_stencil_static_ci;
    };

    /**
     * @class ViewportState.
     * @brief Класс, описывающий состояние области вывода.
    */
    class ViewportState
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param num_viewports число областей вывода.
         * @param num_scissors  число прямоугольников для отсечения.
         * 
         * @throw В случае если num_viewports = 0 или num_scissors = 0.
        */
        ViewportState(size_t num_viewports, size_t num_scissors);

        ViewportState(ViewportState&& viewport_state);
        ViewportState(const ViewportState& viewport_state);

        ~ViewportState();

        ViewportState& operator = (ViewportState&&)         = delete;
        ViewportState& operator = (const ViewportState&)    = delete;

        /**
         * @brief Метод необходимый для добавления области вывода.
         * 
         * @param x         верхний левый угол области просмотра по оси X.
         * @param y         верхний левый угол области просмотра по оси Y.
         * @param width     ширина области просмотра.
         * @param height    высота области просмотра.
         * @param min_depth минимальная глубина для области просмотра.
         * @param max_depth максимальная глубина для области просмотра.
         * 
         * @throw В случае если количество областей вывода больше или равняется capacityViewports().
         * @throw В случае если min_depth < 0 или max_depth > 1.
        */
        void addViewport(float x, float y, float width, float height, float min_depth, float max_depth);

        /**
         * @brief 
         *      Метод необходимый для добавления прямоугольника, с помощью 
         *      которого осуществляется отсечения.
         * 
         * @param x_offset  смещение прямоугольника по оси X.
         * @param y_offset  смещение прямоугольника по оси Y.
         * @param width     ширина прямоугольника.
         * @param height    высота прямоугольника.
         * 
         * @throw В случае если количество прямоугольников для отсечения больше или равняется capacityScissors().
        */
        void addScissor(int32_t x_offset, int32_t y_offset, uint32_t width, uint32_t height);

        const VkPipelineViewportStateCreateInfo&    getPipelineViewportStateCreateInfo()    const noexcept;
        VkPipelineViewportStateCreateInfo&          getPipelineViewportStateCreateInfo()    noexcept;
        const VkViewport*                           getViewports()                          const noexcept;
        const VkRect2D*                             getScissors()                           const noexcept;
        size_t                                      numViewports()                          const noexcept;
        size_t                                      numScissors()                           const noexcept;
        size_t                                      capacityViewports()                     const noexcept;
        size_t                                      capacityScissors()                      const noexcept;

    private:
        VkViewport*                         _ptr_viewports;
        VkRect2D*                           _ptr_scissors;
        size_t                              _current_viewport;
        size_t                              _current_scissor;
        VkPipelineViewportStateCreateInfo   _pipeline_viewport_state_ci;
    };

    /**
     * @class InputAssemblyState.
     * @brief Класс, необходимы для описания входной сборки.
    */
    class InputAssemblyState
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param topology тип примитива.
        */        
        InputAssemblyState(VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

        InputAssemblyState(InputAssemblyState&&)        = default;
        InputAssemblyState(const InputAssemblyState&)   = default;

        InputAssemblyState& operator = (InputAssemblyState&&)       = delete;
        InputAssemblyState& operator = (const InputAssemblyState&)  = delete;

        /**
         * @brief Метод позволяющий устанавливать тип примитива.
         * 
         * @param topology тип примитива.
        */
        void setTopology(VkPrimitiveTopology topology) noexcept;

        /**
         * @brief 
         *      Метод определяющий, будет ли специальной значение индекса вершины
         *      рассматриваться как перезапуск сборки примитива.
         * 
         * @details 
         *      При установке такой возможности много полос или вееров могут 
         *      быть объединены в одну команду вывода.
         *      
         *      Это используется только при индексном выводе.
         * 
         * @param is_enable флаг.
         *      
        */
        void primitiveRestartEnable(VkBool32 is_enable) noexcept;

        const VkPipelineInputAssemblyStateCreateInfo&   getPipelineInputAssemblyStateCreateInfo()   const noexcept;
        VkPipelineInputAssemblyStateCreateInfo&         getPipelineInputAssemblyStateCreateInfo()   noexcept;
        VkPrimitiveTopology                             getTopology()                               const noexcept;
        VkBool32                                        primitiveRestartEnable()                    const noexcept;

    private:
        VkPipelineInputAssemblyStateCreateInfo _pipeline_input_assembly_state_ci;
    };

    /**
     * @class ColorBlendState.
     * @brief Класс, описывающий состояние смешивания цветов.
    */
    class ColorBlendState
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param num_attachments число подключений.
        */
        ColorBlendState(size_t num_attachments) noexcept;

        ColorBlendState(ColorBlendState&& color_blend_state);
        ColorBlendState(const ColorBlendState& color_blend_state);

        ~ColorBlendState();

        ColorBlendState& operator = (ColorBlendState&&)         = delete;
        ColorBlendState& operator = (const ColorBlendState&)    = delete;

        /**
         * @brief Метод добавляющий подключение.
         * @details
         *      Получение окончательного цвета:
         *          color_blend_op(src_color_blend_factor * RGBsrc, dst_color_blend_factor * RGBdst)    - получение исходного цвета;
         *          alpha_blend_op(src_alpha_blend_factor * Asrc, dst_alpha_blend_factor * Adst)        - получение значения альфа канала. 
         * 
         * @param blend_enable              определяюет необходимость включения смешивания для цветового подключения.
         * @param src_color_blend_factor    задаёт множитель на который умножается цветовые каналы источника.
         * @param dst_color_blend_factor    задаёт множитель на который умножается цветовые каналы назначения.
         * @param color_blend_op            операция позволяющая объединять результаты умножений при вычислении каналов RGB.
         * @param src_alpha_blend_factor    задаёт множитель на который умножается альфа канал источника.
         * @param dst_alpha_blend_factor    задаёт множитель на который умножается альфа канал назначения.
         * @param alpha_blend_op            операция позволяющая объединять результаты умножений при вычислении альфа канала.
         * @param color_write_mask          определяет в какие каналы выходного изображения осуществляется запись.
        */
        void addAttchament(
            VkBool32                blend_enable,
            VkBlendFactor           src_color_blend_factor  = VK_BLEND_FACTOR_ZERO,
            VkBlendFactor           dst_color_blend_factor  = VK_BLEND_FACTOR_ZERO,
            VkBlendOp               color_blend_op          = VK_BLEND_OP_ADD,
            VkBlendFactor           src_alpha_blend_factor  = VK_BLEND_FACTOR_ZERO,
            VkBlendFactor           dst_alpha_blend_factor  = VK_BLEND_FACTOR_ZERO,
            VkBlendOp               alpha_blend_op          = VK_BLEND_OP_ADD,
            VkColorComponentFlags   color_write_mask        = VK_COLOR_COMPONENT_A_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_R_BIT
        );

        void logicOpEnable(VkBool32 is_enable) noexcept;

        void setLogicOp(VkLogicOp logic_op) noexcept;

        const VkPipelineColorBlendStateCreateInfo&      getPipelineColorBlendStateCreateInfo()  const noexcept;
        VkPipelineColorBlendStateCreateInfo&            getPipelineColorBlendStateCreateInfo()  noexcept;
        const VkPipelineColorBlendAttachmentState*      getAttachments()                        const noexcept;
        size_t                                          numAttachments()                        const noexcept;
        size_t                                          capacityAttachments()                   const noexcept;
        bool                                            logicOpEnable()                         const noexcept;
        VkLogicOp                                       getLogicOp()                            const noexcept;

    private:
        VkPipelineColorBlendAttachmentState*    _ptr_attachments;
        size_t                                  _current_attachment;
        VkPipelineColorBlendStateCreateInfo     _pipeline_color_blend_state_ci;
    };

    /**
     * @class GraphicsPipelineStates.
     * @brief Класс, хранящий в себе все состояния графического конвейера.
    */
    class GraphicsPipelineStates
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param num_vertex_biding_descriptions    число вершинных привязок используемых конвейером.
         * @param num_vertex_attribute_descriptions число атрибутов вершины.
         * @param num_viewports                     число областей вывода.
         * @param num_scissors                      число прямоугольников для отсечения.
         * @param num_attachments                   число подключений.
        */
        GraphicsPipelineStates(
            size_t num_vertex_biding_descriptions,
            size_t num_vertex_attribute_descriptions,
            size_t num_viewports,
            size_t num_scissors,
            size_t num_attachments
        );

        GraphicsPipelineStates(GraphicsPipelineStates&& graphics_pipeline_states);
        GraphicsPipelineStates(const GraphicsPipelineStates& graphics_pipeline_states);

        GraphicsPipelineStates& operator = (GraphicsPipelineStates&&)         = delete;
        GraphicsPipelineStates& operator = (const GraphicsPipelineStates&)    = delete;

        VertexInputState&            getVertexInputState()   noexcept;
        const VertexInputState&      getVertexInputState()   const noexcept;
        MultisampleState&            getMultisampleState()   noexcept;
        const MultisampleState&      getMultisampleState()   const noexcept;
        RasterizationState&          getRasterizationState() noexcept;
        const RasterizationState&    getRasterizationState() const noexcept;
        DepthStencilState&           getDepthStencilState()  noexcept;
        const DepthStencilState&     getDepthStencilState()  const noexcept;
        ViewportState&               getViewportState()      noexcept;
        const ViewportState&         getViewportState()      const noexcept;
        InputAssemblyState&          getInputAssemblyState() noexcept;
        const InputAssemblyState&    getInputAssemblyState() const noexcept;
        ColorBlendState&             getColorBlendState()    noexcept;
        const ColorBlendState&       getColorBlendState()    const noexcept;

    private:
        VertexInputState    _vertex_input_state;
        MultisampleState    _multisample_state;
        RasterizationState  _rasterization_state;
        DepthStencilState   _depth_stencil_state;
        ViewportState       _viewport_state;
        InputAssemblyState  _input_assembly_state;
        ColorBlendState     _color_blend_state;
    };

    class GraphicsPipeline
    {
    public:
        class Builder :
            public GraphicsPipelineStates
        {
        public:
            /**
             * @brief Конструктор.
             * 
             * @param num_vertex_biding_descriptions    число вершинных привязок используемых конвейером.
             * @param num_vertex_attribute_descriptions число атрибутов вершины.
             * @param num_viewports                     число областей вывода.
             * @param num_scissors                      число прямоугольников для отсечения.
             * @param num_attachments                   число подключений.
            */
            Builder(
                size_t num_vertex_biding_descriptions,
                size_t num_vertex_attribute_descriptions,
                size_t num_viewports,
                size_t num_scissors,
                size_t num_attachments
            );

            Builder(Builder&&)      = delete;
            Builder(const Builder&) = delete;

            Builder& operator = (Builder&&)      = delete;
            Builder& operator = (const Builder&) = delete;

            void setPipleineLayout(const PtrPipelineLayout& ptr_pipeline_layout);
            void setRenderPass(const PtrRenderPass& ptr_render_pass);
            void setSubpassIndex(uint32_t subpass_index) noexcept;
            void setShadersModules(std::vector<ShaderModule>&& shaders);

            void addShader(ShaderModule&& shader_module);

            GraphicsPipeline    build()     const;
            PtrGraphicsPipeline buildPtr()  const;

        private:
            PtrPipelineLayout           _ptr_pipeline_layout;
            PtrRenderPass               _ptr_render_pass;
            uint32_t                    _subpass_index;
            std::vector<ShaderModule>    _shaders;
        };

    public:
        /**
         * @brief Конструктор.
         * 
         * @param graphics_pipeline_states  состояния графического конвейера.
         * @param shaders                   шейдеры.
         * @param ptr_pipeline_layout       указатель на PipelineLayout.
         * @param ptr_render_pass           указатель на проход рендера.
         * @param subpass_index             индекс подпрохода.
        */
        GraphicsPipeline(
            const GraphicsPipelineStates&   graphics_pipeline_states,
            std::span<const ShaderModule>   shaders,
            const PtrPipelineLayout&        ptr_pipeline_layout,
            const PtrRenderPass&            ptr_render_pass,
            uint32_t                        subpass_index
        );

        /**
         * @brief Конструктор.
         * 
         * @param graphics_pipeline_states  состояния графического конвейера.
         * @param shaders                   шейдеры.
         * @param ptr_pipeline_layout       указатель на PipelineLayout.
         * @param ptr_render_pass           указатель на проход рендера.
         * @param subpass_index             индекс подпрохода.
        */
        GraphicsPipeline(
            GraphicsPipelineStates&&        graphics_pipeline_states,
            std::span<const ShaderModule>   shaders,
            const PtrPipelineLayout&        ptr_pipeline_layout,
            const PtrRenderPass&            ptr_render_pass,
            uint32_t                        subpass_index
        );

        GraphicsPipeline(GraphicsPipeline&& graphics_pipeline);
        GraphicsPipeline(const GraphicsPipeline&&)  = delete;

        ~GraphicsPipeline();

        GraphicsPipeline& operator = (GraphicsPipeline&&)       = delete;
        GraphicsPipeline& operator = (const GraphicsPipeline&)  = delete;

        uint32_t                                getSubpassIndex()           const noexcept;
        PtrPipelineLayout&                      getPipelineLayout()         noexcept;
        const PtrPipelineLayout&                getPipelineLayout()         const noexcept;
        GraphicsPipelineStates&                 getGraphicsPipelineStates() noexcept;
        const GraphicsPipelineStates&           getGraphicsPipelineStates() const noexcept;
        VkPipeline                              getPipelineHandle()         const noexcept;
        VkPipelineCache                         getPipelineCacheHandle()    const noexcept;

        /**
         * @brief Статический метод, позволяющий создать указатель на объект типа GraphicsPipeline.
         * 
         * @param graphics_pipeline_states  состояния графического конвейера.
         * @param shaders                   шейдеры.
         * @param ptr_pipeline_layout       указатель на PipelineLayout.
         * @param ptr_render_pass           указатель на проход рендера.
         * @param subpass_index             индекс подпрохода.
        */
        static PtrGraphicsPipeline make(
            const GraphicsPipelineStates&       graphics_pipeline_states,
            const std::vector<ShaderModule>&    shaders,
            const PtrPipelineLayout&            ptr_pipeline_layout,
            const PtrRenderPass&                ptr_render_pass,
            uint32_t                            subpass_index
        );

        /**
         * @brief Статический метод, позволяющий создать указатель на объект типа GraphicsPipeline.
         * 
         * @param graphics_pipeline_states  состояния графического конвейера.
         * @param shaders                   шейдеры.
         * @param ptr_pipeline_layout       указатель на PipelineLayout.
         * @param ptr_render_pass           указатель на проход рендера.
         * @param subpass_index             индекс подпрохода.
        */
        static PtrGraphicsPipeline make(
            GraphicsPipelineStates&&        graphics_pipeline_states,
            std::span<const ShaderModule>    shaders,
            const PtrPipelineLayout&        ptr_pipeline_layout,
            const PtrRenderPass&            ptr_render_pass,
            uint32_t                        subpass_index
        );

    private:
        void _create(std::span<const ShaderModule> shaders);

    private:
        uint32_t                _subpass_index;
        PtrPipelineLayout       _ptr_pipeline_layout;
        PtrRenderPass           _ptr_render_pass;
        GraphicsPipelineStates  _states;
        VkPipeline              _pipeline_handle;
        VkPipelineCache         _pipeline_cache_handle;
    };
}

#endif /* GraphicsPipeline_hpp */
