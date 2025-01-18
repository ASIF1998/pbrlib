#include "../utils.hpp"

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/image.hpp>
#include <backend/renderer/vulkan/buffer.hpp>

#include <array>

class VulkanDeviceTests :
    public ::testing::Test
{
public:
    VulkanDeviceTests()
    {
        device.init(nullptr);
    }
    
    pbrlib::vk::Device device;
};

TEST_F(VulkanDeviceTests, Initialize)
{
    ASSERT_NE(device.instance(), VK_NULL_HANDLE);
    ASSERT_NE(device.physicalDevice(), VK_NULL_HANDLE);
    ASSERT_NE(device.device(), VK_NULL_HANDLE);

    constexpr auto invalied_queue_index_value = std::numeric_limits<uint32_t>::max();

    const auto& queue = device.queue();
    EXPECT_NE(queue.index, invalied_queue_index_value);
    EXPECT_NE(queue.family_index, invalied_queue_index_value);
    EXPECT_NE(queue.handle, VK_NULL_HANDLE);
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

    auto image = pbrlib::vk::Image::Builder(&device)
        .size(width, height)
        .format(format)
        .filter(filter)
        .fillColor(fill_color)
        .addQueueFamilyIndex(device.queue().family_index)
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

TEST_F(VulkanDeviceTests, BuildBuffer)
{
    constexpr size_t size = 12342;
    constexpr auto usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    auto buffer = pbrlib::vk::Buffer::Builder(&device)
        .size(size)
        .usage(usage)
        .addQueueFamilyIndex(device.queue().family_index)
        .build();

    ASSERT_NE(buffer.handle, VK_NULL_HANDLE);
    ASSERT_EQ(buffer.size, size);
}

TEST_F(VulkanDeviceTests, CmdBuffer)
{
    const auto& queue  = device.queue();
    ASSERT_NE(queue.handle, VK_NULL_HANDLE);

    auto cmd_buffer = device.oneTimeSubmitCommandBuffer(queue);
    EXPECT_EQ(cmd_buffer.level, VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    cmd_buffer.write([](VkCommandBuffer handle)
    {
        EXPECT_NE(handle, VK_NULL_HANDLE);
    });
}

TEST_F(VulkanDeviceTests, AllocateDescriptorSet)
{
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

    pbrlib::testing::vk::isSuccess(vkCreateDescriptorSetLayout(device.device(), &descriptor_set_layout_info, nullptr, &descriptor_set_layout));

    auto descriptor_set = device.allocate(descriptor_set_layout);
    EXPECT_NE(descriptor_set.handle, VK_NULL_HANDLE);

    vkDestroyDescriptorSetLayout(device.device(), descriptor_set_layout, nullptr);
}