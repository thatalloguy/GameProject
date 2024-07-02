//
// Created by allos on 7/2/2024.
//

#ifndef GAME_LAKEUTILS_H
#define GAME_LAKEUTILS_H


#include <cstdio>

namespace Lake::Utils {

    template<typename T>
    constexpr void saveStructToFile(const char * file, T& instance) {

        FILE* f_out = fopen(file, "w");

        fwrite(&instance, sizeof(T), 1, f_out);

        fclose(f_out);
    }

    template<typename T>
    constexpr void getStructFromFile(const char * file, T& instance) {
        FILE* f_in = fopen(file, "r");

        fread(&instance, sizeof(T ), 1, f_in);
        fclose(f_in);
    }

};


#endif //GAME_LAKEUTILS_H
