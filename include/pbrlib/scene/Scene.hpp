//
//  Scene.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 15/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <pbrlib/moving/transform.hpp>
#include <pbrlib/scene/component.hpp>

#include <memory>
#include <vector>
#include <unordered_map>

#include <string>
#include <string_view>

#include <typeindex>

#include <span>

namespace pbrlib
{
    class   SceneItem;
    struct  InputStay;

    using VisibleList = std::vector<std::shared_ptr<const SceneItem>>;

    class SceneItem
    {
        friend class Scene;

    public:
        SceneItem(
            const std::string_view  name    = "No name",
            SceneItem*              parent  = nullptr
        );

        virtual ~SceneItem();

        AABB&                               getWorldAABB()      noexcept;
        const AABB&                         getWorldAABB()      const noexcept;
        SceneItem*                          getParent()         noexcept;
        const SceneItem*                    getParent()         const noexcept;
        std::shared_ptr<const SceneItem>    getChild(size_t i)  const;
        Transform&                          getLocalTransform() noexcept;
        const Transform&                    getLocalTransform() const noexcept;
        Transform&                          getWorldTransform() noexcept;
        const Transform&                    getWorldTransform() const noexcept;
        std::string&                        getName()           noexcept;
        const std::string&                  getName()           const noexcept;

        size_t getChildrenCount() const;

        template<typename TComponent>
        inline TComponent& getComponent();

        template<typename TComponent>
        inline const TComponent& getComponent() const;

        template<typename TComponent>
        bool hasComponent() const;

        void setParent(SceneItem* ptr_parent) noexcept;
        void setChildren(std::span<std::shared_ptr<SceneItem>> children);
        void setLocalTransform(const Transform& transform);
        void setWorldTransform(const Transform& transform);
        void setWorldAABB(const AABB& bbox);
        void setName(const std::string_view name);

        bool worldTransformIsCurrent()              const noexcept;
        void worldTransformIsCurrent(bool current)  noexcept;
        bool worldAABBIsCurrent()                   const noexcept;
        void worldAABBIsCurrent(bool current)       noexcept;

        void addComponent(std::shared_ptr<ComponentBase> ptr_component);

        /**
         * @brief Метод необходимый для добавления дочернего узла.
         * @details
         *      При добавлении дочернего узла указатель на родителя
         *      устанавливается автоматически.
         *
         * @param child ссылка на дочерний узел.
        */
        void addChild(std::shared_ptr<SceneItem> child);

        /**
         * @brief Метод необходимый для добавления дочернего узла.
         * @details
         *      При добавлении дочернего узла указатель на родителя
         *      устанавливается автоматически.
         *
         * @param name название дочернего узла.
         * @return Указатель на дочерний узел.
        */
        std::shared_ptr<SceneItem> addChild(const std::string_view name);

        void detachChild(std::shared_ptr<const SceneItem> ptr_item);
        void detachChild(const std::string_view name);

        virtual void update(const InputStay* ptr_input_stay, float delta_time, const Transform& world_transform);

        static std::unique_ptr<SceneItem> make(
            const std::string_view  name        = "No name",
            SceneItem*              ptr_parent  = nullptr
        );

        private:
            void _getVisibleList(VisibleList& out_visible_list);

        protected:
            SceneItem*                                                          _ptr_parent;
            std::vector<std::shared_ptr<SceneItem>>                             _children;
            Transform                                                           _local_transform;
            Transform                                                           _world_transform;
            bool                                                                _world_transform_is_current;
            bool                                                                _world_aabb_is_current;
            // AABB                                                                _world_bbox;
            std::unordered_map<std::type_index, std::shared_ptr<ComponentBase>> _components;
            std::string                                                         _name;
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
        // Scene(
        //     const std::string_view  name, 
        //     const Device*           ptr_device,
        //     const DeviceQueue*      ptr_device_queue,
        //     const CommandPool*      ptr_command_pool,    
        //     uint32_t                device_local_memory_type_index,
        //     uint32_t                host_local_memory_type_index
        // );
        
        void setRoot(std::shared_ptr<SceneItem> ptr_root);
        void setName(const std::string_view name);

        std::shared_ptr<SceneItem>          getRoot()               noexcept;
        std::shared_ptr<const SceneItem>    getRoot()               const noexcept;
        std::string&                        getName()               noexcept;
        const std::string&                  getName()               const noexcept;
        // MaterialManager&                    getMaterialManager()    noexcept;
        // const MaterialManager&              getMaterialManager()    const noexcept;
        // GPUTextureManager&                  getTextureManager()     noexcept;
        // const GPUTextureManager&            getTextureManager()     const noexcept;
        // MeshManager&                        getMeshManager()        noexcept;
        // const MeshManager&                  getMeshManager()        const noexcept;
        VisibleList                         getVisibleList();
        const VisibleList                   getVisibleList()        const;

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
        // std::shared_ptr<SceneItem> makePointLight(
        //     const PointLight::Builder&  light_builder,
        //     const std::string_view      name = "Point Light"
        // );

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
        // std::shared_ptr<SceneItem> makeSpotLight(
        //     const SpotLight::Builder&   light_builder,
        //     const std::string_view      name = "Spot Light"
        // );

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
        // std::shared_ptr<SceneItem> makeDirectionLight(
        //     const DirectionLight::Builder&  light_builder,
        //     const std::string_view          name = "Spot Light"
        // );

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
        // std::shared_ptr<SceneItem> makeCamera(
        //     const PerspectiveCamera::Builder&   camera_builder,
        //     const std::string_view              name = "Camera"
        // );
        
        void update(const InputStay* ptr_input_stay, float delta_time);

    private:
        std::shared_ptr<SceneItem>              _ptr_root;          //!< Корневой узел сцены.
        std::shared_ptr<SceneItem>              _ptr_camera;        //!< Камера.
        std::vector<std::shared_ptr<SceneItem>> _dir_lights;        //!< Направленные источники света.
        std::vector<std::shared_ptr<SceneItem>> _spot_lights;       //!< Прожекторные источники света.
        std::vector<std::shared_ptr<SceneItem>> _point_lights;      //!< Точечные источники света.
        // MaterialManager                         _material_manager;  //!< Менеджер материалов.
        // GPUTextureManager                       _texture_manager;   //!< Менеджер текстур.
        // MeshManager                             _mesh_manager;      //!< Менеджер сеток.
        std::string                             _name;              //!< Имя сцены.
    };
}

#include "Scene.inl"

#endif /* Scene_hpp */
