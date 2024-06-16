#include <pbrlib/Rendering/Window.hpp>
#include <pbrlib/PBRLibResources.hpp>
#include <pbrlib/SceneGraph/SceneView.hpp>
#include <pbrlib/SceneGraph/Scene.hpp>

#include <pbrlib/Rendering/Cameras/PerspectiveCamera.hpp>

#include <pbrlib/Rendering/Geometry/Mesh.hpp>
#include <pbrlib/SceneGraph/MeshNode.hpp>

#include <pbrlib/Rendering/Renderers/PBR.hpp>

#include <iostream>
#include <thread>

#include <filesystem>

using namespace pbrlib;
using namespace pbrlib::math;

const static std::filesystem::path mediatestsRoot = "C:\\Dev\\media";

struct ModelResourcesReferences
{
	std::filesystem::path path_to_mesh;
	std::filesystem::path path_to_albedo;
	std::filesystem::path path_to_normal_map;
	std::filesystem::path path_to_metallic;
	std::filesystem::path path_to_roughness;
	std::filesystem::path path_to_backed_ao;
};

class Rotate: 
    public Component<Rotate>
{
public:
    Rotate() 
    {
        _frame = 0;
    }

    void update(SceneItem* ptr_node, float delta_time) override
    {
		_rotate = Transform::rotateY(static_cast<float>(++_frame));
		ptr_node->getLocalTransform() = Transform::scale(0.7f) * _rotate;
    }

    static auto make() 
    {
        return std::make_shared<Rotate>();
    }

private:
    Transform _rotate;
    size_t _frame;
};

class CameraTest
    : public Component<CameraTest>
{
public:
    CameraTest() = default;

    virtual void update(SceneItem* ptr_node, float delta_time)
    {
        auto ptr_camera = ptr_node->getComponent<PerspectiveCamera>();

        // camera_builder.setEye({-0.9f, 1.0f, 0.1f});
    // camera_builder.setEye({0, 0, -1});
    // camera_builder.setPosition({0.0f, 0.0f, 2.1f});
    // camera_builder.setPosition({0, 0, -2.0});

        Vec3<float> pos (0, 0, 2);
        Vec3<float> eye (-0.9f, static_cast<float>(sin(t += 0.1)), 0.1f);
        Vec3<float> up (0, 1, 0);


        ptr_camera.setLookAt(eye, pos, up);
    }

private:
    double t = 0.0;
};

auto getModelResourcesReferences() 
    -> ModelResourcesReferences
{
    ModelResourcesReferences refs;

    refs.path_to_mesh 			= mediatestsRoot / "pbr-sphere/source/sphere_sampleScene_1cm.fbx";
    refs.path_to_albedo 		= mediatestsRoot / "pbr-sphere/textures/sphere_albedo.jpg";
    refs.path_to_normal_map 	= mediatestsRoot / "pbr-sphere/textures/sphere_normal.jpg";
    refs.path_to_metallic 		= mediatestsRoot / "pbr-sphere/textures/sphere_metallic.jpg";
    refs.path_to_roughness 		= mediatestsRoot / "pbr-sphere/textures/sphere_roughness.jpg";
    refs.path_to_backed_ao 		= mediatestsRoot / "pbr-sphere/textures/sphere_AO.jpg";

    return refs;
}

PtrWindow makeWindow()
{
    Window::Builder window_builder;
    window_builder.setTitle("Sand Box");
    window_builder.setExtend(800, 600);
    window_builder.setPosition(Window::WINDOW_POSITION_CENTERED, Window::WINDOW_POSITION_CENTERED);
    window_builder.setResizableWindow(Window::Resizable::STATIC);

    return window_builder.buildPtr();
}

PtrMaterial getMaterial(const ModelResourcesReferences& model, GPUTextureManager& texture_manager, MaterialManager& material_manager)
{
    Material::Builder material_builder (texture_manager);
    material_builder.setAlbedo(model.path_to_albedo.string(), "Albedo");
    material_builder.setBakedAO(model.path_to_backed_ao.string(), "AO");
    material_builder.setMetallic(model.path_to_metallic.string(), "Mettalic");
    material_builder.setNormalMap(model.path_to_normal_map.string(), "Normal map");
    material_builder.setRoughness(model.path_to_roughness.string(), "Roughness");
    material_builder.setAnisotropy(0.3f);

    auto material = material_builder.buildPtr();
    material_manager.addMaterial(material, "Material");

    return material;
}

auto makeCamera(Scene& scene)
{
    PerspectiveCamera::Builder camera_builder;
    camera_builder.setName("Camera");
    camera_builder.setAspect(800, 600);
    camera_builder.setEye({-0.9f, 0.5f, 0.5f});
    // camera_builder.setEye({0, 0, -1});
    // camera_builder.setPosition({0.0f, 0.0f, 2.1f});
    camera_builder.setPosition({0, 0, -2.0});
    camera_builder.setFarClipp(10.0f);
    camera_builder.setFovy(60.78f);
    camera_builder.setNearClipp(0.01f);
    camera_builder.setUp({0, 1, 0});
    camera_builder.setViewport(0, 0, 800, 600, 0.01f, 2000.0f);

    auto ptr_camera_node = scene.makeCamera(camera_builder);

    return ptr_camera_node;
}

auto makePointLight(Scene& scene)
{
    PointLight::Builder light_builder;
    light_builder.setName("Point Light");
    light_builder.setColor({1, 1, 1});
    light_builder.setIntensity(10.0f);
    light_builder.setPosition({0, 0, 5});

    return scene.makePointLight(light_builder);
}

bool eventHandling(SDL_Event& event)
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_EVENT_QUIT:
				return false;
			case SDL_EVENT_KEY_DOWN:
			{
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					return false;
			}
		}
	}

	return true;
}

int main(int argc, char* argv[])  
{
#if 0
    auto ptr_resources = pbrlib::PBRLibResources::init();
    std::cout << "Hello world !!!" << std::endl;
    return 0;
#else
	PtrPBRLibResources engine_resources = PBRLibResources::init();
    PtrWindow window = makeWindow();

    SceneView scene_view ("Scene", engine_resources, window);


	auto model = getModelResourcesReferences();
    PtrSceneItem ptr_point_light = nullptr;

    Scene&              scene               = scene_view.getScene();
    MeshManager&        mesh_manager        = scene.getMeshManager();
    MaterialManager&    material_manager    = scene.getMaterialManager();
    GPUTextureManager&  texture_manager     = scene.getTextureManager();

    vector<PtrMesh> model_meshes    = mesh_manager.load(model.path_to_mesh.string()).value();
    PtrMaterial     material        = getMaterial(model, texture_manager, material_manager);
    model_meshes[0]->setMaterial(material);

    auto mesh_node = MeshNode::make("Floor", model_meshes[0]);
    mesh_node->addComponent(Rotate::make());

    // Регестрируем модель в сцене.
    scene.setRoot(mesh_node);

    auto ptr_camer = makeCamera(scene);
    
    auto light    = makePointLight(scene);

    PBRPass::Optionals pbr_optionals;
    pbr_optionals.setDistributionFunction(PBRPass::DistributionFunction::GGX);
    pbr_optionals.setGeometryFunction(PBRPass::GeometryFunction::Kelemen);
    pbr_optionals.setFresnelApproximation(PBRPass::FresnelApproximation::Schlick);

    PtrIRenderer renderer = PBR::make(pbr_optionals);
    scene_view.setRenderer(renderer);

    SDL_Event event;
    float htimer = 0.0;

    std::cout << "Runing !!!" << std::endl;

    while (eventHandling(event)) 
	{
        htimer += 0.01f;

        /*camera->setLookAt(
            { 0, 0, -1 },
            {0, 0, 10.0f * std::cos(htimer)},
            {0, 1, 0}
        );*/

        this_thread::sleep_for(10ms);
        scene_view.drawScene(1323);

    }

    return 0;
#endif
}