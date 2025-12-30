#include "render_test.hpp"

#include <backend/renderer/frame_graph/filters/fxaa.hpp>

class FXAATests :
    public pbrlib::testing::RenderTest
{ 
public:
    FXAATests() :
        pbrlib::testing::RenderTest("fxaa-tests", 1000, 1000)
    { }
};

TEST_F(FXAATests, JunkShopAttachments)
{
    constexpr pbrlib::testing::Settings settings
    {
        .up     = pbrlib::math::vec3(0, -1, 0),
        .pos    = pbrlib::math::vec3(-3, 5, 16.0),
        .eye    = pbrlib::math::vec3(0)
    };

    config().aa = pbrlib::settings::AA::eFXAA;

    constexpr auto attahment_name = pbrlib::backend::AttachmentsTraits<pbrlib::backend::FXAA>::result;
    setup("Blender 2.glb", settings);
    check("fxaa/junk-shop-fxaa-result.png", attahment_name);
}