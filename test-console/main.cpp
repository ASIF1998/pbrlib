#include <pbrlib/engine.hpp>
#include <pbrlib/config.hpp>

#include <pbrlib/scene/scene.hpp>

#include <pbrlib/transform.hpp>
#include <pbrlib/camera.hpp>
#include <pbrlib/input/input_stay.hpp>

#include <pbrlib/logger/logger.hpp>

#include <pbrlib/math/quat.hpp>
#include <pbrlib/math/vec3.hpp>
#include <pbrlib/math/vec4.hpp>

#include <backend/utils/paths.hpp>

struct RotateComponent
{
    float angle = 0.0f;
};

auto angleAxis(float angle, const pbrlib::math::vec3& axis)
{
    return pbrlib::math::quat(pbrlib::math::normalize(axis) * sin(angle * 0.5), cos(angle * 0.5));
}

auto mouseMotionPorcess(const pbrlib::Config& config, const pbrlib::MouseMotionStay& motion, const pbrlib::math::vec3& dir) 
    -> pbrlib::math::vec3
{
    const auto move_dir = motion.getRelativeMotion();

    const auto dx = move_dir.x / static_cast<float>(config.width);
    const auto dy = move_dir.y / static_cast<float>(config.height);

    const auto rotation = angleAxis(dx, pbrlib::math::vec3(0, 1, 0)) * angleAxis(dy, pbrlib::math::vec3(1, 0, 0));

    const auto rdir = pbrlib::math::normalize(rotation.toMatrix() * pbrlib::math::vec4(dir, 0.0));

    return pbrlib::math::vec3(rdir.x, rdir.y, rdir.z);
}

auto keyboardProcess(const pbrlib::KeyboardStay& keyboard, const pbrlib::math::vec3& dir, const pbrlib::math::vec3& up) noexcept
    -> pbrlib::math::vec3
{
    if (keyboard.isDown(pbrlib::Keycode::W))
        return dir;

    if (keyboard.isDown(pbrlib::Keycode::S))
        return dir * -1.0;

    if (keyboard.isDown(pbrlib::Keycode::A))
        return pbrlib::math::cross(up, dir);
    
    if (keyboard.isDown(pbrlib::Keycode::D))
        return pbrlib::math::cross(dir, up);

    if (keyboard.isDown(pbrlib::Keycode::Space))
        return up * -1.0;

    if (keyboard.isDown(pbrlib::Keycode::ShiftLeft))
        return up;

    return pbrlib::math::vec3(0);
}

int main()
{
    try 
    {
        pbrlib::Config config;
        config.title    = "test-console";
        config.width    = 2000;
        config.height   = 1100;

        config.ssao = 
        {
            .blur_samples_count = 8
        };
        
        pbrlib::Engine engine (config);

        engine.setup([&engine, &config] (pbrlib::Scene& scene)
        {
            auto& camera = engine.camera();
            camera.up(pbrlib::math::vec3(0, -1, 0));
            camera.pos(pbrlib::math::vec3(-5, 5, 16.0));
            camera.fovY(60.0f);
            camera.range(0.01f, 100.0f);
            camera.width(config.width);
            camera.height(config.height);

            if (!scene.import(engine, pbrlib::backend::utils::projectRoot() / "pbrlib-tests/content/Blender 2.glb")) [[unlikely]] 
                throw std::runtime_error("[test-console] failed load content");

            if (auto ptr_root_item = scene.item("Extended")) [[likely]]
            {
                ptr_root_item->addComponent<RotateComponent>();
                ptr_root_item->update([](
                    pbrlib::SceneItem&          item, 
                    const pbrlib::InputStay&    input_stay, 
                    float                       delta_time, 
                    const pbrlib::math::mat4&   world_transform
                )
                {
                    auto& rotate_component = item.getComponent<RotateComponent>();
                    rotate_component.angle += 0.5;

                    auto& transform = item.getComponent<pbrlib::components::Transform>();
                    transform.transform = pbrlib::transform::rotateZ(rotate_component.angle);
                });

                const auto instance_transform = pbrlib::transform::translate(pbrlib::math::vec3(-2, 2, 3));

                scene.createInstance("Extended", "Extended - 2", instance_transform);
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

        engine.update([&engine, &config] (const pbrlib::InputStay& input_stay, float delta_time)
        {
            auto& camera = engine.camera();

            const auto dir = pbrlib::math::normalize(camera.eye() - camera.pos());

            if (input_stay.mouse_buttons.isDown(pbrlib::MouseButton::eLeft) && input_stay.mouse_motion.isMotion())
                camera.moveEye(mouseMotionPorcess(config, input_stay.mouse_motion, dir));

            camera.translate(keyboardProcess(input_stay.keyboard, dir, camera.up()));
        });

        engine.run();
    }
    catch (const std::exception& ex)
    {
        pbrlib::log::error(ex.what());
    }

    return 0;
}