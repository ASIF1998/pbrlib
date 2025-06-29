#include "../utils.hpp"

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/image.hpp>
#include <backend/renderer/vulkan/buffer.hpp>

#include <backend/renderer/vulkan/pipeline_layout.hpp>

#include <array>

class VulkanDeviceTests :
    public ::testing::Test
{
public:
    void SetUp() override
    {
        if constexpr (!pbrlib::testing::vk::isSupport())
            GTEST_SKIP();

        device.init();
    }
    
    pbrlib::backend::vk::Device device;
};

TEST_F(VulkanDeviceTests, Initialize)
{
    pbrlib::testing::notEquality<VkInstance>(device.instance(), VK_NULL_HANDLE);
    pbrlib::testing::notEquality<VkPhysicalDevice>(device.physicalDevice(), VK_NULL_HANDLE);
    pbrlib::testing::notEquality<VkDevice>(device.device(), VK_NULL_HANDLE);

    constexpr auto invalied_queue_index_value = std::numeric_limits<uint32_t>::max();

    const auto& queue = device.queue();
    pbrlib::testing::notEquality(queue.index, invalied_queue_index_value);
    pbrlib::testing::notEquality(queue.family_index, invalied_queue_index_value);
    pbrlib::testing::notEquality<VkQueue>(queue.handle, VK_NULL_HANDLE);
}

TEST_F(VulkanDeviceTests, BuildImage)
{
    constexpr uint32_t width    = 300;
    constexpr uint32_t height   = 100;

    constexpr auto format = VK_FORMAT_R32_SFLOAT;
    constexpr auto filter = VK_FILTER_LINEAR;

    constexpr auto fill_color = pbrlib::math::vec3(0.54, 0.12, 0.43);
    
    constexpr auto usage =  VK_IMAGE_USAGE_STORAGE_BIT          | 
                            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | 
                            VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    auto image = pbrlib::backend::vk::builders::Image(device)
        .size(width, height)
        .format(format)
        .filter(filter)
        .fillColor(fill_color)
        .addQueueFamilyIndex(device.queue().family_index)
        .usage(usage)
        .build();

    pbrlib::testing::notEquality<VkImage>(image.handle, VK_NULL_HANDLE);
    pbrlib::testing::notEquality<VkImageView>(image.view_handle, VK_NULL_HANDLE);

    pbrlib::testing::equality(image.width, width);
    pbrlib::testing::equality(image.height, height);
    pbrlib::testing::equality(image.format, format);

    pbrlib::testing::equality(image.level_count, 1u);
    pbrlib::testing::equality(image.layer_count, 1u);
}

TEST_F(VulkanDeviceTests, BuildBuffer)
{
    constexpr size_t size = 12342;
    constexpr auto usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    auto buffer = pbrlib::backend::vk::builders::Buffer(device)
        .size(size)
        .usage(usage)
        .addQueueFamilyIndex(device.queue().family_index)
        .build();

    pbrlib::testing::notEquality<VkBuffer>(buffer.handle, VK_NULL_HANDLE);
    pbrlib::testing::equality(buffer.size, size);
}

TEST_F(VulkanDeviceTests, CmdBuffer)
{
    auto cmd_buffer = device.oneTimeSubmitCommandBuffer();
    pbrlib::testing::equality(cmd_buffer.level, VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    cmd_buffer.write([](VkCommandBuffer handle)
    {
        pbrlib::testing::notEquality<VkCommandBuffer>(handle, VK_NULL_HANDLE);
    });
}

TEST_F(VulkanDeviceTests, AllocateDescriptorSet)
{
    constexpr uint32_t descriptor_count = 100;

    auto pipeline_layout = pbrlib::backend::vk::builders::PipelineLayout(device)
        .addSet()
            .addBinding(0, VK_DESCRIPTOR_TYPE_SAMPLER, descriptor_count, VK_SHADER_STAGE_ALL)   
            .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, descriptor_count, VK_SHADER_STAGE_ALL)
            .addBinding(2, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, descriptor_count, VK_SHADER_STAGE_ALL)
            .addBinding(3, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, descriptor_count, VK_SHADER_STAGE_ALL)
            .addBinding(4, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, descriptor_count, VK_SHADER_STAGE_ALL)
            .addBinding(5, VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, descriptor_count, VK_SHADER_STAGE_ALL)
            .addBinding(6, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, descriptor_count, VK_SHADER_STAGE_ALL)
            .addBinding(7, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, descriptor_count, VK_SHADER_STAGE_ALL)
            .addBinding(8, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, descriptor_count, VK_SHADER_STAGE_ALL)
            .addBinding(9, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, descriptor_count, VK_SHADER_STAGE_ALL)
            .addBinding(10, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, descriptor_count, VK_SHADER_STAGE_FRAGMENT_BIT)
        .build();

    for (const auto set_layout_handle: pipeline_layout.sets_layout)
    {
        auto descriptor_set_handle = device.allocateDescriptorSet(set_layout_handle);

        pbrlib::testing::notEquality<VkDescriptorSetLayout>(set_layout_handle, VK_NULL_HANDLE);
        pbrlib::testing::notEquality<VkDescriptorSet>(descriptor_set_handle, VK_NULL_HANDLE);

        vkFreeDescriptorSets(device.device(), device.descriptorPool(), 1, &descriptor_set_handle);
    }
}