#include <pbrlib/engine.hpp>
#include <pbrlib/config.hpp>

#include <pbrlib/scene/scene.hpp>
#include <pbrlib/transform.hpp>
#include <pbrlib/rendering/camera.hpp>
#include <pbrlib/input/input_stay.hpp>

#include <pbrlib/logger/logger.hpp>

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

        engine.setupCallback([](pbrlib::Engine* ptr_engine, pbrlib::Scene* ptr_scene)
        {
            auto& item = ptr_scene->addItem("rotation-item");
            item.addComponent<RotateComponent>();
            item.updateCallback([](
                pbrlib::SceneItem*          ptr_item, 
                const pbrlib::InputStay*    ptr_input_stay, 
                float                       delta_time, 
                const pbrlib::Transform&    world_transform
            )
            {
                auto& rotate_component = ptr_item->getComponent<RotateComponent>();
                /// some manipulations related to turning
            });

            // ptr_scene->addModel(filename, pbrlib::Transform::translate(pbrlib::math::vec3(0, 0, 0)));

            // auto ptr_light = ptr_scene->addLight(
            //     pbrlib::PointLight::Builder()
            //         .setPosition(pbrlib::math::vec3(0, 10, 10))
            //         .setName("point-light")
            //         .setIntensity(100.0)
            //         .setColor(pbrlib::math::vec3(0.32, 0.43, 0.12))
            //         .build()
            // );

            auto& camera = ptr_engine->camera();
        });

        engine.run();
    }
    catch (const std::exception& ex)
    {
        pbrlib::log::error(ex.what());
    }

    return 0;
}