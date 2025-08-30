#include "render_test.hpp"

#include <backend/renderer/frame_graph/ssao.hpp>

#include <array>

class SSAOTests :
    public pbrlib::testing::RenderTest
{ 
public:
    SSAOTests() :
        pbrlib::testing::RenderTest("ssao-tests", 1000, 1000)
    { }
};

TEST_F(SSAOTests, JunkShopAttachments)
{
    constexpr pbrlib::testing::Settings settings
    {
        .up     = pbrlib::math::vec3(0, -1, 0),
        .pos    = pbrlib::math::vec3(-3, 5, 16.0),
        .eye    = pbrlib::math::vec3(0)
    };

    constexpr auto attahment_name = pbrlib::backend::AttachmentsTraits<pbrlib::backend::SSAO>::blur;

    setup("Blender 2.glb", settings);
    for (uint32_t sample_count = 1; sample_count < 1 << 4; sample_count <<= 1)
    {
        config().ssao.blur_samples_count = sample_count;
        for (float spatial_sigma = 0.1; spatial_sigma < 3.0; spatial_sigma += 0.7)
        {
            config().ssao.spatial_sigma = spatial_sigma;
            for (float luminance_sigma = 0.1; luminance_sigma < 3.0; luminance_sigma += 0.7)
            {
                config().ssao.luminance_sigma = luminance_sigma;

                const auto filename = std::format(
                    "ssao/junk-shop_sample-count={},spatial-sigma={},luminance-sigma={}.png", 
                    sample_count, 
                    spatial_sigma, 
                    luminance_sigma
                );

                engine().update(config());
                check(filename, attahment_name);
            }
        }
    }
}