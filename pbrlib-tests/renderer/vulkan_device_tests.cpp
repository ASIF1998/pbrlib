#include "../utils.hpp"

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/image.hpp>
#include <backend/renderer/vulkan/buffer.hpp>

#include <array>

TEST(VulkanDeviceTests, Initialize)
{
    pbrlib::vk::Device device;

    ASSERT_NO_THROW(device.init(nullptr));

    ASSERT_NE(device.instance(), VK_NULL_HANDLE);
    ASSERT_NE(device.physicalDevice(), VK_NULL_HANDLE);
    ASSERT_NE(device.device(), VK_NULL_HANDLE);

    constexpr auto invalied_queue_index_value = std::numeric_limits<uint32_t>::max();

    const auto& graphics_queue = device.graphicsQueue();
    EXPECT_NE(graphics_queue.index, invalied_queue_index_value);
    EXPECT_NE(graphics_queue.family_index, invalied_queue_index_value);
    EXPECT_NE(graphics_queue.handle, VK_NULL_HANDLE);
    
    const auto& compute_queue = device.computeQueue();
    EXPECT_NE(compute_queue.index, invalied_queue_index_value);
    EXPECT_NE(compute_queue.family_index, invalied_queue_index_value);
    EXPECT_NE(compute_queue.handle, VK_NULL_HANDLE);
    
    const auto& transfer_queue = device.transferQueue();
    EXPECT_NE(transfer_queue.index, invalied_queue_index_value);
    EXPECT_NE(transfer_queue.family_index, invalied_queue_index_value);
    EXPECT_NE(transfer_queue.handle, VK_NULL_HANDLE);
}

TEST(VulkanDeviceTests, BuildImage)
{
    constexpr uint32_t width = 300;
    constexpr uint32_t height = 100;

    constexpr auto format = VK_FORMAT_R32_SFLOAT;
    constexpr auto filter = VK_FILTER_LINEAR;

    constexpr auto fill_color = pbrlib::math::vec3(0.54, 0.12, 0.43);
    
    constexpr auto usage = VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    pbrlib::vk::Device device;
    device.init(nullptr);

    auto image = pbrlib::vk::Image::Builder(&device)
        .size(width, height)
        .format(format)
        .filter(filter)
        .fillColor(fill_color)
        .addQueueFamilyIndex(device.graphicsQueue().family_index)
        .addQueueFamilyIndex(device.transferQueue().family_index)
        .addQueueFamilyIndex(device.computeQueue().family_index)
        .usage(usage)
        .build();

    ASSERT_NE(image.handle, VK_NULL_HANDLE);
    ASSERT_NE(image.view_handle, VK_NULL_HANDLE);

    EXPECT_EQ(image.width, width);
    EXPECT_EQ(image.height, height);
    EXPECT_EQ(image.format, format);

    EXPECT_EQ(image.level_count, 1);
    EXPECT_EQ(image.layer_count, 1);
}

TEST(VulkanDeviceTests, BuildBuffer)
{
    constexpr size_t size = 12342;
    constexpr auto usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    pbrlib::vk::Device device;
    device.init(nullptr);

    auto buffer = pbrlib::vk::Buffer::Builder(&device)
        .size(size)
        .usage(usage)
        .addQueueFamilyIndex(device.graphicsQueue().family_index)
        .addQueueFamilyIndex(device.transferQueue().family_index)
        .build();

    ASSERT_NE(buffer.handle, VK_NULL_HANDLE);
    ASSERT_EQ(buffer.size, size);
}

TEST(VulkanDeviceTests, CmdBuffer)
{
    pbrlib::vk::Device device;
    device.init(nullptr);

    const auto& graphics_queue  = device.graphicsQueue();
    const auto& compute_queue   = device.computeQueue();
    const auto& transfer_queue  = device.transferQueue();

    ASSERT_NE(graphics_queue.handle, VK_NULL_HANDLE);
    ASSERT_NE(compute_queue.handle, VK_NULL_HANDLE);
    ASSERT_NE(transfer_queue.handle, VK_NULL_HANDLE);

    auto cmd_buffer_1 = device.oneTimeSubmitCommandBuffer(graphics_queue);
    auto cmd_buffer_2 = device.oneTimeSubmitCommandBuffer(compute_queue);
    auto cmd_buffer_3 = device.oneTimeSubmitCommandBuffer(transfer_queue);

    EXPECT_EQ(cmd_buffer_1.level(), VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    EXPECT_EQ(cmd_buffer_2.level(), VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    EXPECT_EQ(cmd_buffer_3.level(), VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    cmd_buffer_1.write([](VkCommandBuffer handle)
    {
        EXPECT_NE(handle, VK_NULL_HANDLE);
    });
    
    cmd_buffer_2.write([](VkCommandBuffer handle)
    {
        EXPECT_NE(handle, VK_NULL_HANDLE);
    });
    
    cmd_buffer_3.write([](VkCommandBuffer handle)
    {
        EXPECT_NE(handle, VK_NULL_HANDLE);
    });
}

TEST(VulkanDeviceTests, AllocateDescriptorSet)
{
    pbrlib::vk::Device device;
    device.init(nullptr);

    constexpr uint32_t descriptor_count = 100;

    constexpr std::array descriptors_type
    {
        VK_DESCRIPTOR_TYPE_SAMPLER,
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
        VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
        VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
        VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
    };

    std::array<VkDescriptorSetLayoutBinding, descriptors_type.size()> bindings = { };
    for (size_t i = 0; i < bindings.size(); ++i)
    {
        bindings[i].binding         = static_cast<uint32_t>(i);
        bindings[i].descriptorCount = descriptor_count;
        bindings[i].descriptorType  = descriptors_type[i];
        bindings[i].stageFlags      = VK_SHADER_STAGE_ALL;
    }

    VkDescriptorSetLayout descriptor_set_layout = VK_NULL_HANDLE;
    VkDescriptorSetLayoutCreateInfo descriptor_set_layout_info = { };
    descriptor_set_layout_info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptor_set_layout_info.pBindings    = bindings.data();
    descriptor_set_layout_info.bindingCount = static_cast<uint32_t>(bindings.size());

    pbrlib::testing::vk::utils::isSuccess(vkCreateDescriptorSetLayout(device.device(), &descriptor_set_layout_info, nullptr, &descriptor_set_layout));

    auto descriptor_set = device.allocate(descriptor_set_layout);
    EXPECT_NE(descriptor_set.handle, VK_NULL_HANDLE);

    vkDestroyDescriptorSetLayout(device.device(), descriptor_set_layout, nullptr);
}