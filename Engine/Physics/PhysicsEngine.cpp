//
// Created by allos on 6/5/2024.
//

#include "PhysicsEngine.h"
#include "Jolt/Core/Factory.h"
#include "Jolt/RegisterTypes.h"
#include "Jolt/Core/TempAllocator.h"
#include "Jolt/Core/JobSystemThreadPool.h"
#include "Jolt/Physics/PhysicsSystem.h"
#include "Jolt/Physics/Collision/Shape/BoxShape.h"
#include "Jolt/Physics/Body/BodyCreationSettings.h"
#include "Jolt/Physics/Collision/Shape/SphereShape.h"

using namespace Quack::PhysicsHelpers;

void Quack::PhysicsEngine::Initialize(Quack::PhysicsEngineCreationInfo &creationInfo) {


    RegisterDefaultAllocator();

    Trace = PhysicsHelpers::TraceImpl;

    Factory::sInstance = new Factory();

    RegisterTypes();

    TempAllocatorImpl temp_allocator(creationInfo.preallocation_size);

    JobSystemThreadPool job_system(1024, 100, thread::hardware_concurrency() - 1);

    BPlayerInterfaceImpl broad_phase_layer_interface;
    ObjectVsBroadPhaseLayerFilter object_vs_broadphase_layer_filter;
    ObjectLayerPairFilterImpl object_vs_object_layer_filter;

    PhysicsSystem physicsSystem;

    physicsSystem.Init(creationInfo.cMaxBodies, creationInfo.cNumBodyMutexes, creationInfo.cMaxBodyPairs, creationInfo.cMaxContactConstraints, broad_phase_layer_interface, object_vs_broadphase_layer_filter, object_vs_object_layer_filter);


    MyBodyActivationListener body_activation_listener;
    physicsSystem.SetBodyActivationListener(&body_activation_listener);

    MyContactListener contact_listener;
    physicsSystem.SetContactListener(&contact_listener);

    BodyInterface &body_interface = physicsSystem.GetBodyInterface();

    BoxShapeSettings floor_shape_settings(Vec3(100.f, 1.0f, 100.0f));
    floor_shape_settings.SetEmbedded();

    ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
    ShapeRefC floor_shape = floor_shape_result.Get();

    BodyCreationSettings floor_settings(floor_shape, RVec3(0.0f, -1.0f, 0.0f), Quat::sIdentity(), EMotionType::Static, Layers::NON_MOVING);

    Body *floor = body_interface.CreateBody(floor_settings);

    body_interface.AddBody(floor->GetID(), EActivation::DontActivate);

    BodyCreationSettings sphere_settings(new SphereShape(0.5f), RVec3(0.0f, 3.0f, 0.0f), Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING);
    BodyID sphere_id = body_interface.CreateAndAddBody(sphere_settings, EActivation::Activate);

    body_interface.SetLinearVelocity(sphere_id, Vec3(0.0f, -5.0f, 0.0f));

    const float cDeltaTime = 1.0f / 60.0f;

    physicsSystem.OptimizeBroadPhase();

    while (body_interface.IsActive(sphere_id)) {

        RVec3 position = body_interface.GetCenterOfMassPosition(sphere_id);
        spdlog::info("Sphere position: {} {} {}", position.GetX(),position.GetY(),position.GetZ());

        physicsSystem.Update(cDeltaTime, 1, &temp_allocator, &job_system);
    }

    body_interface.RemoveBody(sphere_id);

    body_interface.DestroyBody(sphere_id);


    body_interface.RemoveBody(floor->GetID());
    body_interface.DestroyBody(floor->GetID());

    UnregisterTypes();

    delete Factory::sInstance;
    Factory::sInstance = nullptr;

}
