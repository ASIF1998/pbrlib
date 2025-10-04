#pragma once

#include <vulkan/vulkan.h>

#include <filesystem>

#include <span>
#include <vector>

namespace pbrlib::backend::vk
{
    class Device;
}

namespace pbrlib::backend::vk::shader
{
    class SpecializationInfoBase
    {
    public:
        SpecializationInfoBase() = default;

        SpecializationInfoBase(SpecializationInfoBase&& specialization_info)        = delete;
        SpecializationInfoBase(const SpecializationInfoBase& specialization_info)   = delete;

        SpecializationInfoBase& operator = (SpecializationInfoBase&& specialization_info)       = delete;
        SpecializationInfoBase& operator = (const SpecializationInfoBase& specialization_info)  = delete;

        virtual std::span<const uint8_t>            data()      const noexcept = 0;
        std::span<const VkSpecializationMapEntry>   entries()   const noexcept;

        SpecializationInfoBase& addEntry(uint32_t constant_id, uint32_t offset, size_t size);

    private:
        std::vector<VkSpecializationMapEntry> _entries;
    };

    template<typename T>
    class SpecializationInfo :
        public SpecializationInfoBase
    {
    public:
        explicit SpecializationInfo(const T& data) noexcept :
            _data(data)
        { }

        std::span<const uint8_t> data() const noexcept override
        {
            return std::span(reinterpret_cast<const uint8_t*>(&_data), sizeof(T));
        }
        
    private:
        const T& _data;
    };
}

namespace pbrlib::backend::vk::shader
{
    void initCompiler();
    void finalizeCompiler();

    [[nodiscard]]
    VkShaderModule compile(const Device& device, const std::filesystem::path& filename);
}