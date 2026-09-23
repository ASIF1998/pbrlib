#pragma once

#include <backend/renderer/vulkan/unique_handler.hpp>

#include <span>
#include <vector>

#include <filesystem>

namespace pbrlib::backend::vk
{
    class Device;
}

namespace pbrlib::backend::vk::shader
{
    struct Define final
    {
        std::string name;
        std::string value;
    };

    class SpecializationInfoBase
    {
    public:
        SpecializationInfoBase() = default;

        SpecializationInfoBase(SpecializationInfoBase&& specialization_info)        = delete;
        SpecializationInfoBase(const SpecializationInfoBase& specialization_info)   = delete;

        SpecializationInfoBase& operator = (SpecializationInfoBase&& specialization_info)       = delete;
        SpecializationInfoBase& operator = (const SpecializationInfoBase& specialization_info)  = delete;

        [[nodiscard]] virtual std::span<const uint8_t>          data()      const noexcept = 0;
        [[nodiscard]] std::span<const VkSpecializationMapEntry> entries()   const noexcept;

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

        [[nodiscard]] std::span<const uint8_t> data() const noexcept override
        {
            return std::span(reinterpret_cast<const uint8_t*>(&_data), sizeof(T));
        }

    private:
        const T& _data;
    };

    void init();
    void finalize();

    [[nodiscard]] vk::ShaderModuleHandle compile(
         Device&                         device,
        const std::filesystem::path&    filename,
        const std::filesystem::path&    root_directory,
        std::span<const Define>         defines,
        bool                            dump = false
    );
}
