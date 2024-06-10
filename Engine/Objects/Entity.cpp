//
// Created by allos on 6/8/2024.
//

#include "Entity.h"

Quack::Entity::Entity(Quack::EntityCreationInfo &creationInfo)  {
    parseInfo(creationInfo);
}

Quack::Entity::~Entity()  {
    if (body_interface) {
        body_interface->RemoveBody(physicsID);

        body_interface->DestroyBody(physicsID);
    }
}

void Quack::Entity::render(VulkanEngine &engine)  {
    // only render if we have a model attached.
    if (modelID > 0) {
        engine.loadedScenes[modelID]->Draw(getTransformMatrix(), engine.mainDrawContext);
    }
}

void Quack::Entity::updatePhysics(Quack::PhysicsEngine &physicsEngine)  {
    if (body_interface) {
        position = physicsEngine.getInterface().GetCenterOfMassPosition(physicsID);
    }
}

void Quack::Entity::setPosition(Quack::Math::Vector3 vec)  {
    position = vec;
}

void Quack::Entity::parseInfo(Quack::EntityCreationInfo &info)  {

    this->modelID = info.model;
    this->position = info.position;
    size = info.size;

    if (info.isPhysical && info.bodyCreationInfo.physicsEngine != nullptr) {
        BodyCreationSettings settings(info.bodyCreationInfo.shape, info.bodyCreationInfo.position,
                                      info.bodyCreationInfo.rotation, info.bodyCreationInfo.motionType,
                                      info.bodyCreationInfo.layer);

        physicsID = info.bodyCreationInfo.physicsEngine->addNewBody(settings,
                                                                    info.bodyCreationInfo.shouldActivate);
        body_interface = &info.bodyCreationInfo.physicsEngine->getInterface();
    }
}

glm::mat4 Quack::Entity::getTransformMatrix()  {
    glm::mat4 model{1.f};

    model =  glm::translate(model, glm::vec3{position.x, position.y, position.z});
    model =  glm::scale(model, glm::vec3{size.x, size.y, size.z});

    return model;
}

bool Quack::Entity::hasHit(Quack::Math::Vector3 pos) {
    return (pos.x > position.x - size.x && pos.x < position.x + size.x) && (pos.y > position.y - size.y && pos.y < position.y + size.y) && (pos.z > position.z - size.z && pos.z < position.z + size.z);
}
