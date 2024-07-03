//
// Created by allos on 7/2/2024.
//

#include <fstream>
#include "AssetManager.h"

#include "LakeUtils.h"
#include "spdlog/spdlog.h"

void Lake::AssetManager::Initialize(const char *assetDataFile) {
    // Do this check so that we dont try to load a struct that doesnt exist
    if (fopen(assetDataFile, "r") != NULL) {
        loadAssetDataFromFile(assetDataFile);
    }

    _assetDataFile = assetDataFile;
}

void Lake::AssetManager::newAsset(const char* fileName, Quack::AssetType type) {

    _idCount++;
    Quack::AssetInfo newAsset{};

    newAsset.type = type;
    newAsset.Id = _idCount;

    // ;-; im sure this is fine.
    memcpy(newAsset.fileName, fileName, strlen(fileName));

    _assets.push_back(newAsset);

}

void Lake::AssetManager::exportAssetData() {
    FILE* file = fopen(_assetDataFile, "w");

    for (auto item : _assets) {
        fwrite(&item, sizeof(Quack::AssetInfo), 1, file);
    }

    fclose(file);

}


void Lake::AssetManager::loadAssetDataFromFile(const char *assetDataFile) {

    FILE* file = fopen(assetDataFile, "r");

    Quack::AssetInfo tempObject{};
    // will this work?
    //Lake::Utils::getStructArrayFromFile(assetDataFile, static_array, 1);

    while (fread(&tempObject, sizeof(Quack::AssetInfo), 1, file) == 1) {
        _assets.push_back(tempObject);

        _assetCount = _assets.size();
        _idCount = _assets.back().Id;
    }

    fclose(file);


}

void Lake::AssetManager::renderAssetUI() {

}