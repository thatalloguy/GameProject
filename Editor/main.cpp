//
// Created by allos on 7/1/2024.
//

#define LAKE_EDITOR_VERSION_DATA
#define LAKE_EDITOR_VERSION 1
#define LAKE_EDITOR_MIN_COMPATIBLE_VERSION 1


#include <fstream>
#include "spdlog/spdlog.h"
#include "src/App.h"
#include "Animations/AnimationManager.h"

int main() {

/*
    Lake::App::Init();


    Lake::App::Run();



    Lake::App::CleanUp();*/

/*
    FILE* f_out = fopen("../../animations.lake", "w");

    Quack::AnimationDataBase dataBase;

    Quack::Animation animation {
            .frames ={{0, 0, 0}, {1, 5, 6}},
            .loop = true
    };

    dataBase.registerAnimation(&animation);

    int size = dataBase._animations.size();

    fwrite(&size, sizeof(int), 1, f_out);
    for (auto pair : dataBase._animations) {
        //first write the ID to the file
        fwrite(&pair.first, sizeof(Quack::AnimationID), 1, f_out);

        //then write the animation data.
        fwrite(&pair.second->loop, sizeof(bool), 1, f_out);

        // then all the keyframes
        for (auto frame : pair.second->frames) {
            fwrite(&frame, sizeof(Quack::Keyframe), 1, f_out);
        }
    }

    fclose(f_out);
*/


    Quack::AnimationDataBase dataBase1;

    FILE* f_in = fopen("../../animations.lake", "r");

    int readSize = 0;
    // get the size of the saved hashmap
    fread(&readSize, sizeof(int), 1, f_in);

    for (int i=0; i<readSize; i++) {

        auto tempAnimation = new Quack::Animation{{}, 0, 0.0f, {}, false};
        Quack::Keyframe tempKeyframe{};
        //set it to i as a fallback.
        Quack::AnimationID tempID = i;

        fread(&tempID, sizeof(Quack::AnimationID), 1, f_in);
        fread(&tempAnimation->loop, sizeof(bool), 1, f_in);

        while (fread(&tempKeyframe, sizeof(Quack::Keyframe), 1, f_in) == 1) {
            tempAnimation->frames.push_back(tempKeyframe);
        }
        dataBase1._animations[tempID] = tempAnimation;
    }

    dataBase1.cleanUp();

    return 0;
}

