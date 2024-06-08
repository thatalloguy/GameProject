//
// Created by allos on 6/8/2024.
//

#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#define USING_JOLT
#include "Renderer/VkEngine.h"
#include "Physics/PhysicsEngine.h"

namespace Quack {
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
        Entity(EntityCreationInfo &creationInfo);
        ~Entity();


        void render(VulkanEngine &engine);

        void updatePhysics(Quack::PhysicsEngine &physicsEngine);

        void setPosition(Quack::Math::Vector3 vec);

    private:
        Quack::Math::Vector3 position{0, 0, 0};
        Quack::Math::Vector3 size{1, 1, 1};
        unsigned int modelID = 0;
        BodyID physicsID{};
        BodyInterface *body_interface = nullptr;

        void parseInfo(EntityCreationInfo &info);
        glm::mat4 getTransformMatrix();
    };
}// Quack

#endif //GAME_ENTITY_H
