#include <pbrlib/engine.hpp>
#include <pbrlib/config.hpp>

#include <pbrlib/moving/transform.hpp>

#include <pbrlib/scene/scene.hpp>
#include <pbrlib/scene/component.hpp>

#include <pbrlib/Rendering/Lights/PointLight.hpp>
// #include <pbrlib/Rendering/Cameras/PerspectiveCamera.hpp>

#include <pbrlib/new-renderer/camera.hpp>

#include <pbrlib/logger/logger.hpp>

#include <pbrlib/input/input_stay.hpp>

class RotateComponent :
    public pbrlib::Component<RotateComponent>
{
    void update(const pbrlib::InputStay* ptr_input_stay, pbrlib::SceneItem* ptr_node, float delta_time) override
    {
        if (ptr_input_stay->keyboard.isDown(pbrlib::Keycode::Enter)) 
            _rotate_angle += 0.1f;
    }

public:
    RotateComponent() :
        Component("rotate-compnent")
    { }

private:
    float _rotate_angle = 0.0f;
};

int main()
{
    try 
    {
        pbrlib::Config config;
        config.title = "test-console";
        
        pbrlib::Engine engine (config);

        /// @todo Написать ещё часть, где будет работа с компанентами.

        engine.setupCallback([](pbrlib::Engine* ptr_engine, pbrlib::Scene* ptr_scene)
        {
            const auto filename = pbrlib::path_to_root / "pbrlib-tests/content/Blender 2.glb";
            pbrlib::log::info("Path to content: {}", filename.string());

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