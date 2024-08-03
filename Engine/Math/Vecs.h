//
// Created by allos on 6/1/2024.
//

#ifndef GAME_VECS_H
#define GAME_VECS_H




#ifdef USE_JOLT
#include <Jolt/Jolt.h>

#endif



    namespace Quack::Math {


        // Simple math struct that holds 2 floats.
        struct Vector2 {

            float x, y;

            bool operator==(Vector2 &b) const {
                return (this->x == b.x && this->y == b.y);
            }

            Vector2& operator=(float val) {
                this->x = val;
                this->y = val;
                return *this;
            }

            void operator+(Vector2 &b) {
                this->x += b.x;
                this->y += b.y;
            }

            Vector2& operator-(Vector2 &b) {
                this->x -= b.x;
                this->y -= b.y;
                return *this;
            }

        };

        struct Vector3 {

            float x, y, z;

            bool operator==(Vector3 &b) const {
                return (this->x == b.x && this->y == b.y && this->z == b.z);
            }

            Vector3& operator=(float val) {
                this->x = val;
                this->y = val;
                this->z = val;
                return *this;
            }

            void operator+(Vector3 &b) {
                this->x += b.x;
                this->y += b.y;
                this->z += b.z;
            }

            Vector3& operator*(float b) {
                this->x *= b;
                this->y *= b;
                this->z *= b;

                return *this;
            }




#ifdef USING_JOLT
            Vector3& operator=(JPH::Vec3 b) {
                this->x = b.GetX();
                this->y = b.GetY();
                this->z = b.GetZ();
                return *this;
            }

            Vector3(float x, float y, float z) {
                this->x = x;
                this->y = y;
                this->z = z;
            }

            explicit Vector3(JPH::RVec3 vec) {
                this->x = vec.GetX();
                this->y = vec.GetY();
                this->z = vec.GetZ();
            }

#endif

        };

        struct Vector4 {

            float x, y, z, w;

            bool operator==(Vector4 &b) const {
                return (this->x == b.x && this->y == b.y && this->z == b.z && this->w == b.w);
            }

            Vector4& operator=(float val) {
                this->x = val;
                this->y = val;
                this->z = val;
                this->w = val;
                return *this;
            }

            void operator+(Vector4 &b) {
                this->x += b.x;
                this->y += b.y;
                this->z += b.z;
                this->w += b.z;
            }

            void sIdentity() {
                w = 1;
            }

        };



    }



#endif //GAME_VECS_H
