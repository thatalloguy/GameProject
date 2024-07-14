//
// Created by allos on 7/2/2024.
//

#ifndef GAME_ASSETMANAGER_H
#define GAME_ASSETMANAGER_H

#include <Objects/Entity.h>
#include <vector>

namespace Lake {

    /*
     * Task of the assetManager is to generate the LevelAssetInfo struct.
     * NOTE: the asset buffer is DYNAMIC in the Map Editor, BUT is STATIC in the RUNTIME.
     */
    class AssetManager {
    public:

        void Initialize(const char* assetDataFile);

        void newAsset(const char* fileName, Quack::AssetType type);
        void deleteAsset(unsigned int assetID) {};

        void exportAssetData();

        void renderAssetUI(float width, float height);

        void renderAssetSelectionUI(Quack::EntityBlueprint& entity);

    private:
        unsigned int _assetCount = 0;
        unsigned int _idCount = 1;
        std::vector<Quack::AssetInfo> _assets;
        const char* _assetDataFile;

        void loadAssetDataFromFile(const char* assetDataFile);
    };
}




#endif //GAME_ASSETMANAGER_H
