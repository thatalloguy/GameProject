//
// Created by allos on 6/1/2024.
//

#ifndef GAME_VECS_H
#define GAME_VECS_H


namespace Quack {

    namespace Math {


        // Simple math struct that holds 2 floats.
        struct Vector2 {

            float x, y;

            bool operator==(Vector2 &b) {
                return (this->x == b.x && this->y == b.y);
            }

            void operator=(float val) {
                this->x = val;
                this->y = val;
            }

            void operator+(Vector2 &b) {
                this->x += b.x;
                this->y += b.y;
            }

        };

        struct Vector3 {

            float x, y, z;

            bool operator==(Vector3 &b) {
                return (this->x == b.x && this->y == b.y && this->z == b.z);
            }

            void operator=(float val) {
                this->x = val;
                this->y = val;
                this->z = val;
            }

            void operator=(Vector3& val) {
                this->x = val.x;
                this->y = val.y;
                this->z = val.z;
            }

            void operator+(Vector3 &b) {
                this->x += b.x;
                this->y += b.y;
                this->z += b.z;
            }

        };

        struct Vector4 {

            float x, y, z, w;

            bool operator==(Vector4 &b) {
                return (this->x == b.x && this->y == b.y && this->z == b.z, this->w == b.w);
            }

            void operator=(float val) {
                this->x = val;
                this->y = val;
                this->z = val;
                this->w = val;
            }

            void operator+(Vector4 &b) {
                this->x += b.x;
                this->y += b.y;
                this->z += b.z;
                this->w += b.z;
            }

        };

    }

};


#endif //GAME_VECS_H
