#include <pbrlib/engine.hpp>
#include <pbrlib/config.hpp>

#include <pbrlib/scene/scene.hpp>

#include <pbrlib/transform.hpp>
#include <pbrlib/rendering/camera.hpp>
#include <pbrlib/input/input_stay.hpp>

#include <pbrlib/logger/logger.hpp>

#include <backend/utils/paths.hpp>

struct RotateComponent
{
    float angle = 0.0f;
};

int main()
{
    try 
    {
        pbrlib::Config config;
        config.title = "test-console";
        
        pbrlib::Engine engine (config);

        engine.setupCallback([&config](pbrlib::Engine& engine, pbrlib::Scene& scene)
        {
            auto& camera = engine.camera();
            camera.up(pbrlib::math::vec3(0, -1, 0));
            camera.eye(pbrlib::math::vec3(-5, 5, 16.0));
            camera.fovY(60.0f);
            camera.range(0.01f, 1000.0f);
            camera.width(config.width);
            camera.height(config.height);

            if (!scene.import(engine, pbrlib::backend::utils::projectRoot() / "pbrlib-tests/content/Blender 2.glb")) [[unlikely]] 
                throw std::runtime_error("[test-console] failed load content");

            if (auto* ptr_root_item = scene.item("Extended")) [[likely]]
            {
                ptr_root_item->addComponent<RotateComponent>();
                ptr_root_item->updateCallback([](
                    pbrlib::SceneItem&          item, 
                    const pbrlib::InputStay&    input_stay, 
                    float                       delta_time, 
                    const pbrlib::math::mat4&   world_transform
                )
                {
                    auto& rotate_component = item.getComponent<RotateComponent>();
                    rotate_component.angle += 0.5;

                    auto& transform = item.getComponent<pbrlib::component::Transform>();
                    transform.transform = pbrlib::transform::rotateZ(rotate_component.angle);
                });
            }


            // auto ptr_light = ptr_scene->addLight(
            //     pbrlib::PointLight::Builder()
            //         .setPosition(pbrlib::math::vec3(0, 10, 10))
            //         .setName("point-light")
            //         .setIntensity(100.0)
            //         .setColor(pbrlib::math::vec3(0.32, 0.43, 0.12))
            //         .build()
            // );
        });

        engine.run();
    }
    catch (const std::exception& ex)
    {
        pbrlib::log::error(ex.what());
    }

    return 0;
}