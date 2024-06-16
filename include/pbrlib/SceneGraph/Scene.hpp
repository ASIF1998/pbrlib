//
//  Scene.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 15/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <memory>
#include <vector>
#include <unordered_map>

#include <string>
#include <string_view>

#include <typeindex>

#include <pbrlib/Moving/Transform.hpp>
#include "../Rendering/Geometry/AABB.hpp"

#include <pbrlib/Rendering/Lights/PointLight.hpp>
#include <pbrlib/Rendering/Lights/SpotLight.hpp>
#include <pbrlib/Rendering/Lights/DirectionLight.hpp>

#include <pbrlib/Rendering/Material/MaterialManager.hpp>
#include <pbrlib/Rendering/VulkanWrapper/GPUTextureManager.hpp>
#include <pbrlib/Rendering/Geometry/MeshManager.hpp>

#include <pbrlib/Rendering/Cameras/PerspectiveCamera.hpp>

using namespace std;

namespace pbrlib
{
    class ComponentBase;
    
    using PtrComponent = shared_ptr<ComponentBase>;
}

namespace pbrlib
{
    class SceneItem;

    using PtrSceneItem = shared_ptr<SceneItem>;
    using VisibleList = vector<PtrSceneItem>;

    class SceneItem
    {
        friend class Scene;

    public:
        SceneItem(
            const string_view   name = "No name",
            SceneItem* parent = nullptr
        );

        virtual ~SceneItem();

        AABB&                       getWorldAABB()      noexcept;
        const AABB&                 getWorldAABB()      const noexcept;
        SceneItem*                  getParent()         noexcept;
        const SceneItem*            getParent()         const noexcept;
        PtrSceneItem&               getChild(size_t i);
        const PtrSceneItem&         getChild(size_t i)  const;
        vector<PtrSceneItem>&       getChildren()       noexcept;
        const vector<PtrSceneItem>& getChildren()       const noexcept;
        Transform&                  getLocalTransform() noexcept;
        const Transform&            getLocalTransform() const noexcept;
        Transform&                  getWorldTransform() noexcept;
        const Transform&            getWorldTransform() const noexcept;
        string&                     getName()           noexcept;
        const string&               getName()           const noexcept;

        template<typename TComponent>
        inline TComponent& getComponent();

        template<typename TComponent>
        inline const TComponent& getComponent() const;

        template<typename TComponent>
        bool hasComponent() const;

        void setParent(SceneItem* ptr_parent) noexcept;
        void setChildren(vector<PtrSceneItem>&& children);
        void setChildren(const vector<PtrSceneItem>& children);
        void setLocalTransform(const Transform& transform);
        void setWorldTransform(const Transform& transform);
        void setWorldAABB(const AABB& bbox);
        void setName(const string_view name);

        bool worldTransformIsCurrent()              const noexcept;
        void worldTransformIsCurrent(bool current)  noexcept;
        bool worldAABBIsCurrent()                   const noexcept;
        void worldAABBIsCurrent(bool current)       noexcept;

        void addComponent(const PtrComponent& ptr_component);

        /**
         * @brief Метод необходимый для добавления дочернего узла.
         * @details
         *      При добавлении дочернего узла указатель на родителя
         *      устанавливается автоматически.
         *
         * @param child ссылка на дочерний узел.
        */
        void addChild(PtrSceneItem&& child);

        /**
         * @brief Метод необходимый для добавления дочернего узла.
         * @details
         *      При добавлении дочернего узла указатель на родителя
         *      устанавливается автоматически.
         *
         * @param child ссылка на дочерний узел.
        */
        void addChild(const PtrSceneItem& child);

        /**
         * @brief Метод необходимый для добавления дочернего узла.
         * @details
         *      При добавлении дочернего узла указатель на родителя
         *      устанавливается автоматически.
         *
         * @param SceneItem_name название дочернего узла.
         * @return Указатель на дочерний узел.
        */
        PtrSceneItem& addChild(const string_view name);

        void detachChild(const PtrSceneItem& ptr_item);
        void detachChild(const string_view name);

        virtual void update(float delta_time, const Transform& world_transform);

        static PtrSceneItem make(
            const string_view   name        = "No name",
            SceneItem*          ptr_parent  = nullptr
        );

        private:
            void _getVisibleList(VisibleList& out_visible_list);

        protected:
            SceneItem*                              _ptr_parent;
            vector<PtrSceneItem>                    _ptr_children;
            Transform                               _local_transform;
            Transform                               _world_transform;
            bool                                    _world_transform_is_current;
            bool                                    _world_aabb_is_current;
            AABB                                    _world_bbox;
            unordered_map<type_index, PtrComponent> _components;
            string                                  _name;
        };

    class Scene
    {
        friend class SceneView;
        
    public:
        /**
         * @brief Конструктор.
         * 
         * @param name                              название сцены.
         * @param ptr_device                        указатель на устройство. 
         * @param ptr_device_queue                  указатель на очередь устройства.
         * @param ptr_command_pool                  указатель на команлный пул.
         * @param device_local_memory_type_index    тип памяти, являющаяся локальной для устройства (GPU).
         * @param host_local_memory_type_index      тип памяти, которая может быть отображена и читаться или записываться CPU.
        */
        Scene(
            const string_view       name, 
            const PtrDevice&        ptr_device,
            const PtrDeviceQueue&   ptr_device_queue,
            const PtrCommandPool&   ptr_command_pool,    
            uint32_t                device_local_memory_type_index,
            uint32_t                host_local_memory_type_index
        );
        
        void setRoot(const PtrSceneItem& ptr_root);
        void setRoot(PtrSceneItem&& ptr_root);
        void setName(const string_view name);

        PtrSceneItem&               getRoot()               noexcept;
        const PtrSceneItem&         getRoot()               const noexcept;
        string&                     getName()               noexcept;
        const string&               getName()               const noexcept;
        MaterialManager&            getMaterialManager()    noexcept;
        const MaterialManager&      getMaterialManager()    const noexcept;
        GPUTextureManager&          getTextureManager()     noexcept;
        const GPUTextureManager&    getTextureManager()     const noexcept;
        MeshManager&                getMeshManager()        noexcept;
        const MeshManager&          getMeshManager()        const noexcept;
        VisibleList                 getVisibleList();
        const VisibleList           getVisibleList()        const;

        /**
         * @brief Метод создающий узел для точечного источника света.
         * @details 
         *      Регистрирует источник света в сцене.
         *      Созданный узел не имеет родителя и дочерних узлов.
         * 
         * @param light_builder объект создающий источник света.
         * @param name          название узла.
         * @return Указатель на узел.
        */
        PtrSceneItem& makePointLight(
            const PointLight::Builder&   light_builder,
            const string_view            name = "Point Light"
        );

        /**
         * @brief Метод создающий узел для прожекторного источника света.
         * @details 
         *      Регистрирует источник света в сцене.
         *      Созданный узел не имеет родителя и дочерних узлов.
         * 
         * @param light_builder объект создающий источник света.
         * @param name          название узла.
         * @return Указатель на узел.
        */
        PtrSceneItem& makeSpotLight(
            const SpotLight::Builder&   light_builder,
            const string_view           name = "Spot Light"
        );

        /**
         * @brief Метод создающий узел для направленного источника света.
         * @details 
         *      Регистрирует источник света в сцене.
         *      Созданный узел не имеет родителя и дочерних узлов.
         * 
         * @param light_builder объект создающий источник света.
         * @param name          название узла.
         * @return Указатель на узел.
        */
        PtrSceneItem& makeDirectionLight(
            const DirectionLight::Builder&  light_builder,
            const string_view               name = "Spot Light"
        );

        /**
         * @brief Метод создающий узел для камеры.
         *  @details 
         *      Регистрирует камеру в сцене.
         *      Созданный узел не имеет родителя и дочерних узлов.
         *      Камера в сцене может быть только одно, поэтому при 
         *      вызове данного метод, предыдущая камера, если она была,
         *      будет заменена на новую.
         * @param camera_builder    объект создающий камеру.
         * @param name              название узла.
         * @return Указатель на узел.
        */
        PtrSceneItem& makeCamera(
            const PerspectiveCamera::Builder&   camera_builder,
            const string_view                   name = "Camera"
        );
        
        void update(float delta_time);

    private:
        PtrSceneItem            _ptr_root;          //!< Корневой узел сцены.
        PtrSceneItem            _ptr_camera;        //!< Камера.
        vector<PtrSceneItem>    _dir_lights;        //!< Направленные источники света.
        vector<PtrSceneItem>    _spot_lights;       //!< Прожекторные источники света.
        vector<PtrSceneItem>    _point_lights;      //!< Точечные источники света.
        MaterialManager         _material_manager;  //!< Менеджер материалов.
        GPUTextureManager       _texture_manager;   //!< Менеджер текстур.
        MeshManager             _mesh_manager;      //!< Менеджер сеток.
        string                  _name;              //!< Имя сцены.
    };
}

#include "Scene.inl"

#endif /* Scene_hpp */
