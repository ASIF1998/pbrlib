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
#include "ShaderModule.hpp"

#include <vector>
#include <memory>

using namespace std;

namespace pbrlib
{
    class GraphicsPipeline;

    using PtrGraphicsPipeline = shared_ptr<GraphicsPipeline>;

    /**
     * @class VertexInputState.
     * @brief Класс описывающий состояние входных данных вершин.
    */
    class VertexInputState :
        private VkPipelineVertexInputStateCreateInfo
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param num_vertex_biding_descriptions    число вершинных привязок используемых конвейером.
         * @param num_vertex_attribute_descriptions число атрибутов вершины.
        */
        inline VertexInputState(
            size_t num_vertex_biding_descriptions       = 0,
            size_t num_vertex_attribute_descriptions    = 0
        );

        inline VertexInputState(VertexInputState&& vertex_input_state);
        inline VertexInputState(const VertexInputState& vertex_input_state);

        inline ~VertexInputState();

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
        inline void addVertexInputBindingDescription(uint32_t binding, uint32_t stride, VkVertexInputRate input_rate);

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
        inline void addVertexInputAttributeDescription(uint32_t location, uint32_t binding, VkFormat format, uint32_t offset);

        inline const VkVertexInputBindingDescription*   getVertexInputBindingDescriptions()         const noexcept;
        inline const VkVertexInputAttributeDescription* getVertexInputAttributeDescriptions()       const noexcept;
        inline size_t                                   numVertexInputBindingDescription()          const noexcept;
        inline size_t                                   numVertexInputAttributeDescription()        const noexcept;
        inline size_t                                   capacityVertexInputBindingDescription()     const noexcept;
        inline size_t                                   capacityVertexInputAttributeDescription()   const noexcept;

    private:
        VkVertexInputBindingDescription*    _ptr_vertex_biding_descriptions;
        VkVertexInputAttributeDescription*  _ptr_vertex_attribute_descriptions;
        size_t                              _curent_vertex_biding_description;
        size_t                              _curent_vertex_attribute_description;
    };

    /**
     * @class MultisampleState.
     * @brief Класс описывающий состояние мультисэмплинга.
    */
    class MultisampleState :
        private VkPipelineMultisampleStateCreateInfo
    {
    public:
        inline MultisampleState(
            VkSampleCountFlagBits   num_samples                 = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
            VkBool32                sample_shading_enable       = VK_FALSE,
            float                   min_sample_shading          = 1.0f,
            VkBool32                alpha_to_coverage_enable    = VK_FALSE,
            VkBool32                alpha_to_one_enable         = VK_FALSE
        );

        inline MultisampleState(const MultisampleState& multisample_state) noexcept;

        MultisampleState& operator = (MultisampleState&&)       = delete;
        MultisampleState& operator = (const MultisampleState&)  = delete;

        inline void setSampleMask(VkSampleMask sample_mask) noexcept;

        /**
         * @brief 
         *      Метод позволяющий установить количество выборок,
         *      используемых при растеризации.
         * 
         * @param num_samples количество выборок.
        */
        inline void setRasterizationSamples(VkSampleCountFlagBits num_samples) noexcept;

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
        inline void setMinSampleShading(float min_sample_shading) noexcept;
        
        /**
         * @brief Метод позволяющий включать или выключать закрашивание с частотой образцов.
         * 
         * @param is_enable флаг.
        */
        inline void sampleShadingEnable(VkBool32 is_enable) noexcept;

        inline void alphaToCoverageEnable(VkBool32 is_enable) noexcept;

        /**
         * @brief 
         *      Метод позволяющий заменять альфа-канал цветовых буферов
         *      максимальным значением для типов с фиксированной запятой
         *      и 1.0 для типов с плавающей запятой. В противном случае 
         *      альфа-компанент не меняется.
         * 
         * @param is_enable флаг.
        */
        inline void alphaToOneEnable(VkBool32 is_enable) noexcept;

        inline VkSampleMask             getSampleMask()             const noexcept;
        inline VkSampleCountFlagBits    getRasterizationSamples()   const noexcept;
        inline float                    getMinSampleShading()       const noexcept;
        inline VkBool32                 sampleShadingEnable()       const noexcept;
        inline VkBool32                 alphaToCoverageEnable()     const noexcept;
        inline VkBool32                 alphaToOneEnable()          const noexcept;

    private:
        VkSampleMask _sample_mask;
    };

    /**
     * @class RasterizationState.
     * @brief Класс описывающий состояние растеризации.
    */
    class RasterizationState :
        private VkPipelineRasterizationStateCreateInfo
    {
    public:
        inline RasterizationState();

        RasterizationState(RasterizationState&&)        = default;
        RasterizationState(const RasterizationState&)   = default;

        RasterizationState& operator = (RasterizationState&&)       = delete;
        RasterizationState& operator = (const RasterizationState&)  = delete;

        /**
         * @brief Метод позволяющий установить режим отображения треугольника.
         * 
         * @param polygon_mode режим отображения треугольника.
        */
        inline void setPolygonMode(VkPolygonMode polygon_mode) noexcept;

        /**
         * @brief Метод позволяющий установить режим отбраковки треугольников.
         * 
         * @param cull_mode режим отбраковки треугольников.
        */
        inline void setCullMode(VkCullModeFlags cull_mode) noexcept;

        /**
         * @brief 
         *      Метод позволяющий установить какое направление обхода вершин 
         *      (по часовой стрелке) является лицевым (в системе координат окна).
         * 
         * @param front_face определяет ориентацию треугольника.
        */
        inline void setFrontFace(VkFrontFace front_face) noexcept;
        
        /**
         * @brief 
         *      Метод позволяющий установить значение постоянной глубины, 
         *      добавляемый к каждому фрагменту.
         * 
         * @param depth_bias_constant_factor постоянная глубина.
        */
        inline void setDepthBiasConstantFactor(float depth_bias_constant_factor) noexcept;

        /**
         * @brief Метод позволяющий установить максимальную глубину.
         * 
         * @param depth_bias_clamp максимальная глубина.
        */
        inline void setDepthBiasClamp(float depth_bias_clamp) noexcept;

        /**
         * @brief 
         *      Метод позволяющий установить коэффициент, применяемый к 
         *      наклону в расчётах смещения глубины.
         * 
         * @param depts_bias_slope_factor коэффициент, применяемый к наклону.
        */
        inline void setDepthBiasSlopeFactor(float depts_bias_slope_factor) noexcept;

        /**
         * @brief Метод позволяющий установить толщину отрезка в пикселях.
         * 
         * @param line_width толщина отрезка в пикселях.
        */
        inline void setLineWidth(float line_width) noexcept;

        /**
         * @brief Метод позволяющий включать или отключать смещение глубины.
         * 
         * @param is_enable флаг.
        */
        inline void depthClampEnable(VkBool32 is_enable) noexcept;

        /**
         * @brief Метод позволяющий отключить или включить растеризацию.
         * 
         * @param is_enable флаг. Если он равен  VK_TRUE, то растеризация отключается.
        */
        inline void rasterizerDiscardEnable(VkBool32 is_enable) noexcept;

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
        inline void depthBiasEnable(VkBool32 is_enable) noexcept;

        inline VkPolygonMode    getPolygonMode()                const noexcept;
        inline VkCullModeFlags  getCullMode()                   const noexcept;
        inline VkFrontFace      getFrontFace()                  const noexcept;
        inline float            getDepthBiasConstantFactor()    const noexcept;
        inline float            getDepthBiasClamp()             const noexcept;
        inline float            getDepthBiasSlopeFactor()       const noexcept;
        inline float            getLineWidth()                  const noexcept;
        inline VkBool32         depthClampEnable()              const noexcept;
        inline VkBool32         rasterizerDiscardEnable()       const noexcept;
        inline VkBool32         depthBiasEnable()               const noexcept;
    };

    /**
     * @class DepthStencilState.
     * @brief Класс, описывающий состояние глубины и трафарета.
    */
    class DepthStencilState :
        private VkPipelineDepthStencilStateCreateInfo
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param depth_test_enable VK_TRUE - включение теста глубины.
         * @param stencil_test_enable VK_TRUE - включение теста трафарета.
        */
        inline DepthStencilState(
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
        inline void setDepthCompareOp(VkCompareOp depth_compare_op) noexcept;

        /**
         * @brief 
         *      Метод позволяющий установить состояние теста 
         *      трафарета для лицевых примитивов.
         * 
         * @param front состояние теста трафарета для лицевых примитивов.
        */
        inline void setFront(VkStencilOpState front) noexcept;

        /**
         * @brief 
         *      Метод позволяющий установить состояние теста 
         *      трафарета для нелицевых примитивов.
         * 
         * @param back состояние теста трафарета для нелицевых примитивов.
        */
        inline void setBack(VkStencilOpState back) noexcept;
        
        /**
         * @brief 
         *      Метод позволяющий установить минимальную границу 
         *      диапазона глубины. Эта граница будет использована
         *      при тесте диапазона глубины.
         * 
         * @param min_depth_bounds минимальная граница диапазона глубины.
        */
        inline void setMinDepthBounds(float min_depth_bounds) noexcept;

        /**
         * @brief 
         *      Метод позволяющий установить максимальную границу 
         *      диапазона глубины. Эта граница будет использована
         *      при тесте диапазона глубины.
         * 
         * @param max_depth_bounds максимальная граница диапазона глубины.
        */
        inline void setMaxDepthBounds(float max_depth_bounds) noexcept;

        /**
         * @brief Метод позволяющий включить или выключить тест глубины.
         * 
         * @param is_enable флаг.
        */
        inline void depthTestEnable(VkBool32 is_enable) noexcept;

        /**
         * @brief Метод позволяющий включить или выключить запись в буффер глубины.
         * 
         * @param is_enable флаг.
        */
        inline void depthWriteEnable(VkBool32 is_enable) noexcept;

        /**
         * @brief Метод позволяющий включить или выключить тест диапазона глубины.
         * 
         * @param is_enable флаг.
        */
        inline void depthBoundsTestEnable(VkBool32 is_enable) noexcept;

        /**
         * @brief Метод позволяющий включить или выключить тест трафарета.
         * 
         * @param is_enable флаг.
        */
        inline void stencilTestEnable(VkBool32 is_enable) noexcept;

        inline VkCompareOp      getDepthCompareOp()     noexcept;
        inline VkStencilOpState getFront()              noexcept;
        inline VkStencilOpState getBack()               noexcept;
        inline float            getMinDepthBounds()     noexcept;
        inline float            getMaxDepthBounds()     noexcept;
        inline VkBool32         depthTestEnable()       noexcept;
        inline VkBool32         depthWriteEnable()      noexcept;
        inline VkBool32         depthBoundsTestEnable() noexcept;
        inline VkBool32         stencilTestEnable()     noexcept;
    };

    /**
     * @class ViewportState.
     * @brief Класс, описывающий состояние области вывода.
    */
    class ViewportState :
        private VkPipelineViewportStateCreateInfo
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
        inline ViewportState(size_t num_viewports, size_t num_scissors);

        inline ViewportState(ViewportState&& viewport_state);
        inline ViewportState(const ViewportState& viewport_state);

        inline ~ViewportState();

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
        inline void addViewport(float x, float y, float width, float height, float min_depth, float max_depth);

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
        inline void addScissor(int32_t x_offset, int32_t y_offset, uint32_t width, uint32_t height);

        inline const VkViewport*    getViewports()      const noexcept;
        inline const VkRect2D*      getScissors()       const noexcept;
        inline size_t               numViewports()      const noexcept;
        inline size_t               numScissors()       const noexcept;
        inline size_t               capacityViewports() const noexcept;
        inline size_t               capacityScissors()  const noexcept;

    private:
        VkViewport* _ptr_viewports;
        VkRect2D*   _ptr_scissors;
        size_t      _current_viewport;
        size_t      _current_scissor;
    };

    /**
     * @class InputAssemblyState.
     * @brief Класс, необходимы для описания входной сборки.
    */
    class InputAssemblyState :
        private VkPipelineInputAssemblyStateCreateInfo
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param topology тип примитива.
        */        
        inline InputAssemblyState(VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

        InputAssemblyState(InputAssemblyState&&)        = default;
        InputAssemblyState(const InputAssemblyState&)   = default;

        InputAssemblyState& operator = (InputAssemblyState&&)       = delete;
        InputAssemblyState& operator = (const InputAssemblyState&)  = delete;

        /**
         * @brief Метод позволяющий устанавливать тип примитива.
         * 
         * @param topology тип примитива.
        */
        inline void setTopology(VkPrimitiveTopology topology) noexcept;

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
        inline void primitiveRestartEnable(VkBool32 is_enable) noexcept;

        inline VkPrimitiveTopology  getTopology()               const noexcept;
        inline VkBool32             primitiveRestartEnable()    const noexcept;
    };

    /**
     * @class ColorBlendState.
     * @brief Класс, описывающий состояние смешивания цветов.
    */
    class ColorBlendState :
        private VkPipelineColorBlendStateCreateInfo
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param num_attachments число подключений.
        */
        inline ColorBlendState(size_t num_attachments) noexcept;

        inline ColorBlendState(ColorBlendState&& color_blend_state);
        inline ColorBlendState(const ColorBlendState& color_blend_state);

        inline ~ColorBlendState();

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
        inline void addAttchament(
            VkBool32                blend_enable,
            VkBlendFactor           src_color_blend_factor,
            VkBlendFactor           dst_color_blend_factor,
            VkBlendOp               color_blend_op,
            VkBlendFactor           src_alpha_blend_factor,
            VkBlendFactor           dst_alpha_blend_factor,
            VkBlendOp               alpha_blend_op,
            VkColorComponentFlags   color_write_mask
        );

        inline void logicOpEnable(VkBool32 is_enable) noexcept;

        inline void setLogicOp(VkLogicOp logic_op) noexcept;

        inline const VkPipelineColorBlendAttachmentState*   getAttachments()        const noexcept;
        inline size_t                                       numAttachments()        const noexcept;
        inline size_t                                       capacityAttachments()   const noexcept;
        inline bool                                         logicOpEnable()         const noexcept;
        inline VkLogicOp                                    getLogicOp()            const noexcept;

    private:
        VkPipelineColorBlendAttachmentState*    _ptr_attachments;
        size_t                                  _current_attachment;
    };

    /**
     * @class GraphicsPipelineState.
     * @brief Класс, хранящий в себе все состояния графического конвейера.
    */
    class GraphicsPipelineState
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
        inline GraphicsPipelineState(
            size_t num_vertex_biding_descriptions,
            size_t num_vertex_attribute_descriptions,
            size_t num_viewports,
            size_t num_scissors,
            size_t num_attachments
        );

        inline GraphicsPipelineState(GraphicsPipelineState&& graphics_pipeline_state);
        inline GraphicsPipelineState(const GraphicsPipelineState& graphics_pipeline_state);

        GraphicsPipelineState& operator = (GraphicsPipelineState&&)         = delete;
        GraphicsPipelineState& operator = (const GraphicsPipelineState&)    = delete;

        inline VertexInputState&            getVertexInputState()   noexcept;
        inline const VertexInputState&      getVertexInputState()   const noexcept;
        inline MultisampleState&            getMultisampleState()   noexcept;
        inline const MultisampleState&      getMultisampleState()   const noexcept;
        inline RasterizationState&          getRasterizationState() noexcept;
        inline const RasterizationState&    getRasterizationState() const noexcept;
        inline DepthStencilState&           getDepthStencilState()  noexcept;
        inline const DepthStencilState&     getDepthStencilState()  const noexcept;
        inline ViewportState&               getViewportState()      noexcept;
        inline const ViewportState&         getViewportState()      const noexcept;
        inline InputAssemblyState&          getInputAssemblyState() noexcept;
        inline const InputAssemblyState&    getInputAssemblyState() const noexcept;
        inline ColorBlendState&             getColorBlendState()    noexcept;
        inline const ColorBlendState&       getColorBlendState()    const noexcept;

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
        /**
         * @brief Конструктор.
         * 
         * @param graphics_pipeline_state   состояния графического конвейера.
         * @param shaders                   шейдеры.
         * @param ptr_pipeline_layout       указатель на PipelineLayout.
         * @param ptr_render_pass           указатель на проход рендера.
         * @param subpass_index             индекс подпрохода.
        */
        inline GraphicsPipeline(
            const GraphicsPipelineState&            graphics_pipeline_state,
            const vector<ShaderModule>&             shaders,
            const PtrPipelineLayout&                ptr_pipeline_layout,
            const PtrRenderPass&                    ptr_render_pass,
            uint32_t                                subpass_index
        );

        /**
         * @brief Конструктор.
         * 
         * @param graphics_pipeline_state   состояния графического конвейера.
         * @param shaders                   шейдеры.
         * @param ptr_pipeline_layout       указатель на PipelineLayout.
         * @param ptr_render_pass           указатель на проход рендера.
         * @param subpass_index             индекс подпрохода.
        */
        inline GraphicsPipeline(
            GraphicsPipelineState&&                 graphics_pipeline_state,
            const vector<ShaderModule>&             shaders,
            const PtrPipelineLayout&                ptr_pipeline_layout,
            const PtrRenderPass&                    ptr_render_pass,
            uint32_t                                subpass_index
        );

        inline  GraphicsPipeline(GraphicsPipeline&& graphics_pipeline);
                GraphicsPipeline(const GraphicsPipeline&&)  = delete;

        inline ~GraphicsPipeline();

        GraphicsPipeline& operator = (GraphicsPipeline&&)       = delete;
        GraphicsPipeline& operator = (const GraphicsPipeline&)  = delete;

        inline uint32_t                                 getSubpassIndex()           const noexcept;
        inline PtrPipelineLayout&                       getPipelineLayout()         noexcept;
        inline const PtrPipelineLayout&                 getPipelineLayout()         const noexcept;
        inline GraphicsPipelineState&                   getGraphicsPipelineState()  noexcept;
        inline const GraphicsPipelineState&             getGraphicsPipelineState()  const noexcept;
        inline VkPipeline                               getPipelineHandle()         const noexcept;
        inline VkPipelineCache                          getPipelineCacheHandle()    const noexcept;

        /**
         * @brief Статический метод, позволяющий создать указатель на объект типа GraphicsPipeline.
         * 
         * @param graphics_pipeline_state   состояния графического конвейера.
         * @param shaders                   шейдеры.
         * @param ptr_pipeline_layout       указатель на PipelineLayout.
         * @param ptr_render_pass           указатель на проход рендера.
         * @param subpass_index             индекс подпрохода.
        */
        inline static PtrGraphicsPipeline make(
            const GraphicsPipelineState&            graphics_pipeline_state,
            const vector<ShaderModule>&             shaders,
            const PtrPipelineLayout&                ptr_pipeline_layout,
            const PtrRenderPass&                    ptr_render_pass,
            uint32_t                                subpass_index
        );

        /**
         * @brief Статический метод, позволяющий создать указатель на объект типа GraphicsPipeline.
         * 
         * @param graphics_pipeline_state   состояния графического конвейера.
         * @param shaders                   шейдеры.
         * @param ptr_pipeline_layout       указатель на PipelineLayout.
         * @param ptr_render_pass           указатель на проход рендера.
         * @param subpass_index             индекс подпрохода.
        */
        inline static PtrGraphicsPipeline make(
            GraphicsPipelineState&&     graphics_pipeline_state,
            const vector<ShaderModule>& shaders,
            const PtrPipelineLayout&    ptr_pipeline_layout,
            const PtrRenderPass&        ptr_render_pass,
            uint32_t                    subpass_index
        );

    private:
        inline void _create(const vector<ShaderModule>& shaders);

    private:
        uint32_t                _subpass_index;
        PtrPipelineLayout       _ptr_pipeline_layout;
        PtrRenderPass           _ptr_render_pass;
        GraphicsPipelineState   _state;
        VkPipeline              _pipeline_handle;
        VkPipelineCache         _pipeline_cache_handle;
    };
}

#include "GraphicsPipeline.inl"

#endif /* GraphicsPipeline_hpp */
