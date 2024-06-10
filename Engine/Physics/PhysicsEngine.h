//
// Created by allos on 6/5/2024.
//

#ifndef GAME_PHYSICSENGINE_H
#define GAME_PHYSICSENGINE_H

#include <cstdarg>
#include <cstdio>
#include "spdlog/spdlog.h"

#define JPH_ENABLE_ASSERT
#include <Jolt/Jolt.h>


#include <Jolt/Physics/Collision/ObjectLayer.h>
#include "Jolt/Physics/Collision/ContactListener.h"
#include "Jolt/Physics/Body/BodyActivationListener.h"
#include "Math/Vecs.h"
#include "Jolt/Physics/Body/BodyInterface.h"
#include "Jolt/Physics/PhysicsSystem.h"
#include "Jolt/Core/JobSystemThreadPool.h"
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

#include "Jolt/Core/Factory.h"
#include "Jolt/RegisterTypes.h"
#include "Jolt/Core/TempAllocator.h"
#include "Jolt/Core/JobSystemThreadPool.h"
#include "Jolt/Physics/Collision/Shape/BoxShape.h"
#include "Jolt/Physics/Body/BodyCreationSettings.h"
#include "Jolt/Physics/Collision/Shape/SphereShape.h"

using namespace JPH;

namespace Quack {


    namespace Layers {
        static constexpr JPH::ObjectLayer NON_MOVING = 0;
        static constexpr JPH::ObjectLayer MOVING = 1;
        static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
    }

    namespace PhysicsHelpers {

        static void TraceImpl(const char* inFMT, ...) {
            va_list list;
            va_start(list, inFMT);
            char buffer[1024];
            vsnprintf(buffer, sizeof(buffer), inFMT, list);
            va_end(list);

            // Print to the TTY
            spdlog::info("Trace: {}", buffer);
        }

#ifdef JPH_ENABLE_ASSERT
        static bool AssertFailedImpl(const char *inExpression, const char *inMessage, const char *inFile, unsigned int inLine) {
            spdlog::error("Assertion: {}: {}: ( {} ) {}", inFile, inLine, inExpression, inMessage);

            return true;
        }
#endif

        class ObjectLayerPairFilterImpl : public ObjectLayerPairFilter {
        public:
            virtual bool ShouldCollide(ObjectLayer inObject1, ObjectLayer inObject2) {
                switch (inObject1)
                {
                    case Layers::NON_MOVING:
                        return inObject2 == Layers::MOVING; // Non moving only collides with moving
                    case Layers::MOVING:
                        return true; // Moving collides with everything
                    default:
                        JPH_ASSERT(false);
                        return false;
                }
            }
        };

        namespace BroadPhaseLayers {
            static constexpr BroadPhaseLayer NON_MOVING(0);
            static constexpr BroadPhaseLayer MOVING(1);
            static constexpr uint NUM_LAYERS(2);
        }

        class BPlayerInterfaceImpl final : public BroadPhaseLayerInterface {

        public:
            BPlayerInterfaceImpl() {
                    mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
                    mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
            }

            virtual unsigned int GetNumBroadPhaseLayers() const override {
                return BroadPhaseLayers::NUM_LAYERS;
            }

            virtual BroadPhaseLayer GetBroadPhaseLayer(ObjectLayer inLayer) const override {
                JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
                return mObjectToBroadPhase[inLayer];
            }

            virtual const char *			GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override
            {
                switch ((BroadPhaseLayer::Type)inLayer)
                {
                    case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
                    case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
                    default:													JPH_ASSERT(false); return "INVALID";
                }
            }


        private:
            BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
        };

/*        class ObjectVsBroadPhaseLayerFilterImpl : public ObjectVsBroadPhaseLayerFilter {
        public:
            virtual bool ShouldCollide(ObjectLayer inLayer1, BroadPhaseLayer inLayer2) const override {
                switch (inLayer1)
                {
                    case Layers::NON_MOVING:
                        return inLayer2 == BroadPhaseLayers::MOVING;
                    case Layers::MOVING:
                        return true;
                    default:
                        JPH_ASSERT(false);
                        return false;
                }
            }
        };
*/
        class MyContactListener : public ContactListener {
        public:

            ValidateResult OnContactValidate(const Body &inBody1, const Body &inBody2, RVec3Arg inBaseOffset, const CollideShapeResult& inCollisionResult) override {
                spdlog::debug("Contact Validate Callback");

                return ValidateResult::AcceptAllContactsForThisBodyPair;
            }

            void OnContactAdded(const Body& inBody1, const Body& inBody2, const ContactManifold& inManifold, ContactSettings& ioSettings) override {
                spdlog::debug("A contact was added");
            }

            void OnContactPersisted(const Body& inBody1, const Body& inBody2, const ContactManifold& inManifold, ContactSettings& ioSettings) override {
                spdlog::debug("A contact was persisted");
            }

            void OnContactRemoved(const SubShapeIDPair& inSubShapePair) override {
                spdlog::debug("A contact was removed");
            }

        };

        class MyBodyActivationListener : public BodyActivationListener {
        public:
            virtual void OnBodyActivated(const BodyID& inBodyId, uint64 inBodyUserData) override {
                //spdlog::debug("A body got activated");
            }

            virtual void OnBodyDeactivated(const BodyID& inBodyID, uint64 inBodyUserData) override {
                //spdlog::debug("A body went to sleep");
            }
        };

    }

    struct PhysicsEngineCreationInfo {
        int preallocation_size = 10 * 1024 * 1024;
        uint cMaxBodies = 1024;
        uint cNumBodyMutexes = 0;
        uint cMaxBodyPairs = 1024;
        uint cMaxContactConstraints = 1024;
        float cDeltaTime = 1.0f / 60.0f;
    };

    class PhysicsEngine {

    public:


        PhysicsEngine(PhysicsEngineCreationInfo& physicsEngineCreationInfo) {
            Initialize(physicsEngineCreationInfo);
        }


        ~PhysicsEngine();

        void optimizeBP();

        BodyID addNewBody(BodyCreationSettings& settings, EActivation mode);

        //temp
        BodyInterface& getInterface();

        PhysicsSystem& getSystem();
        void update();


    private:
        void Initialize(PhysicsEngineCreationInfo& creationInfo);

        BodyInterface* body_interface = nullptr;
        PhysicsSystem* physicsSystem= nullptr;
        TempAllocator* temp_allocator= nullptr;
        JobSystemThreadPool* job_system= nullptr;

        PhysicsHelpers::BPlayerInterfaceImpl* broad_phase_layer_interface = nullptr;
        ObjectVsBroadPhaseLayerFilter* object_vs_broadphase_layer_filter = nullptr;
        PhysicsHelpers::ObjectLayerPairFilterImpl* object_vs_object_layer_filter = nullptr;

        float cDeltaTime = 1.0f / 60.0f;

    };


}




#endif //GAME_PHYSICSENGINE_H
