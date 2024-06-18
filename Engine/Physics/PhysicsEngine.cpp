//
// Created by allos on 6/5/2024.
//

#include "PhysicsEngine.h"

using namespace Quack::PhysicsHelpers;

void Quack::PhysicsEngine::Initialize(Quack::PhysicsEngineCreationInfo &creationInfo) {


    RegisterDefaultAllocator();

    Trace = PhysicsHelpers::TraceImpl;

    Factory::sInstance = new Factory();

    RegisterTypes();

    temp_allocator = new TempAllocatorImpl(creationInfo.preallocation_size);

    job_system = new JobSystemThreadPool(1024, 100, (int) thread::hardware_concurrency() - 1);


    physicsSystem = new PhysicsSystem();

    broad_phase_layer_interface = new PhysicsHelpers::BPlayerInterfaceImpl();
    object_vs_broadphase_layer_filter = new ObjectVsBroadPhaseLayerFilter();
    object_vs_object_layer_filter = new ObjectLayerPairFilterImpl();

    physicsSystem->Init(creationInfo.cMaxBodies, creationInfo.cNumBodyMutexes, creationInfo.cMaxBodyPairs, creationInfo.cMaxContactConstraints, *broad_phase_layer_interface, *object_vs_broadphase_layer_filter, *object_vs_object_layer_filter);

    body_interface = &physicsSystem->GetBodyInterface();

    cDeltaTime = creationInfo.cDeltaTime;
}

BodyID Quack::PhysicsEngine::addNewBody(BodyCreationSettings &settings, EActivation mode) {
    return body_interface->CreateAndAddBody(settings, mode);
}



void Quack::PhysicsEngine::update() {
        physicsSystem->Update(cDeltaTime, 1, temp_allocator, job_system);
}

Quack::PhysicsEngine::~PhysicsEngine() {
    UnregisterTypes();

    delete temp_allocator;
    delete job_system;
    delete physicsSystem;

    delete broad_phase_layer_interface;
    delete object_vs_broadphase_layer_filter;
    delete object_vs_object_layer_filter;

    delete Factory::sInstance;
    Factory::sInstance = nullptr;

}


void Quack::PhysicsEngine::optimizeBP() {
    physicsSystem->OptimizeBroadPhase();
}

BodyInterface &Quack::PhysicsEngine::getInterface() {
    return *body_interface;
}

PhysicsSystem &Quack::PhysicsEngine::getSystem() {
    return *physicsSystem;
}


