#include "render_test.hpp"

#include <backend/renderer/frame_graph/gbuffer_generator.hpp>

#include <array>

class GBufferGeneratorTests :
    public pbrlib::testing::RenderTest
{
public:
    GBufferGeneratorTests() :
        pbrlib::testing::RenderTest("gbuffer-generator-tests", 1000, 1000)
    { }
};

TEST_F(GBufferGeneratorTests, JunkShopAttachments)
{
    /// @todo add check material_index

    constexpr std::array attachments
    {
        std::make_pair("gbuffer_generator/junk-shop-attachment-normal-tangent.png", pbrlib::backend::AttachmentsTraits<pbrlib::backend::GBufferGenerator>::normal_tangent),
        std::make_pair("gbuffer_generator/junk-shop-attachment-pos-uv.png", pbrlib::backend::AttachmentsTraits<pbrlib::backend::GBufferGenerator>::pos_uv)
    };

    constexpr pbrlib::testing::Settings settings
    {
        .up     = pbrlib::math::vec3(0, -1, 0),
        .pos    = pbrlib::math::vec3(-3, 5, 16.0),
        .eye    = pbrlib::math::vec3(0)
    };

    setup("Blender 2.glb", settings);
    for (const auto [filename, attachment_name]: attachments)
        check(filename, attachment_name);
}
