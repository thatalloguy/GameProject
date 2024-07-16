//
// Created by allos on 6/8/2024.
//

#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#define USING_JOLT
#include "Renderer/VkEngine.h"
#include "Physics/PhysicsEngine.h"

namespace Quack {

    enum class ShapeType : unsigned int {
        Box = 0,
        Sphere = 1,
        Capsule = 2
    };

    struct EntityBlueprint {
        std::string name = "name";
        unsigned int model = 0;
        bool isPhysical = false;


        // Physicscreation
        Math::Vector3 shapeVolume{1, 1, 1}; // for cube its the half extents. for the sphere x = radius, for capsule x = radius y = height.
        ShapeType shapeType = ShapeType::Box;
        int physicsType = 0; // static = 0. dynamic = 2. kinematic = 1
        bool shouldActivate = false;
        unsigned int objectLayer = 0;
    };


    enum class AssetType : unsigned int {
        Model = 0,
        Sound = 1,
        Unknown = 2
    };

    struct AssetInfo {
        AssetType type = AssetType::Unknown;
        char fileName[54] = "Unknown";
        unsigned int Id = 0;
    };


    struct BodyCreationInfo {
        RVec3 position;
        Quat rotation;
        Shape *shape = nullptr;
        EActivation shouldActivate = EActivation::DontActivate;
        EMotionType motionType = EMotionType::Static;
        JPH::ObjectLayer layer = 0; // non moving

        Quack::PhysicsEngine *physicsEngine = nullptr;
    };

    struct EntityCreationInfo {
        Quack::Math::Vector3 position{0, 0, 0};
        Quack::Math::Vector3 size{1, 1, 1};
        unsigned int model = 0;
        bool isDynamic = false;
        bool isPhysical = false;
        BodyCreationInfo bodyCreationInfo{};
    };

    class Entity {

    public:
        explicit Entity(EntityCreationInfo &creationInfo);
        ~Entity();


        void render(VulkanEngine &engine);

        void updatePhysics(Quack::PhysicsEngine &physicsEngine);

        void setPosition(Quack::Math::Vector3 vec);

        bool hasHit(Quack::Math::Vector3 pos) const;

        Quack::Math::Vector3 position{0, 0, 0};
        Quack::Math::Vector3 size{1, 1, 1};
        BodyID physicsID{};
    private:
        unsigned int modelID = 0;
        BodyInterface *body_interface = nullptr;

        void parseInfo(EntityCreationInfo &info);
        glm::mat4 getTransformMatrix() const;
    };
}// Quack

#endif //GAME_ENTITY_H
