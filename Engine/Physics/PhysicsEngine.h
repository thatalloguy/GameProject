//
// Created by allos on 6/5/2024.
//

#ifndef GAME_PHYSICSENGINE_H
#define GAME_PHYSICSENGINE_H

#include <cstdarg>
#include <cstdio>
#include "spdlog/spdlog.h"
#include <Jolt/Jolt.h>


#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

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

        private:
            BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
        };

    }

    struct PhysicsEngineCreationInfo {

    };

    class PhysicsEngine {

    public:

    private:

    };


}




#endif //GAME_PHYSICSENGINE_H
