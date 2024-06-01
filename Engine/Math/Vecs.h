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

            bool operator==(Vector2& b) {
                return (this->x == b.x && this->y == b.y);
            }

            void operator=(float val) {
                this->x = val;
                this->y = val;
            }

            void operator+(Vector2& b) {
                this->x += b.x;
                this->y += b.y;
            }

        };

    }

};


#endif //GAME_VECS_H
