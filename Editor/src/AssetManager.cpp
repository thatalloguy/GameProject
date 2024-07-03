//
// Created by allos on 7/2/2024.
//

#include <fstream>
#include "AssetManager.h"

#include "LakeUtils.h"
#include "spdlog/spdlog.h"
#include "imgui.h"
#include "IconsFontAwesome6.h"

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

void Lake::AssetManager::renderAssetUI(float width, float height) {


    ImGui::BeginTable("assets", 2);

    for (auto asset : _assets) {

        ImGui::TableNextColumn();

        ImVec2 rectMin = ImGui::GetCursorScreenPos();

        rectMin.y *= 1 + ImGui::TableGetRowIndex() / 0.6f;

        ImVec2 rectMax = ImVec2(rectMin.x + ImGui::GetColumnWidth(), rectMin.y + (height * 0.13f) );

        ImVec4 color = ImVec4(0.125980454683304f, 0.1250980454683304f, 0.1290196138620377f, 1.0f);
        ImGui::GetWindowDrawList()->AddRectFilled(rectMin, rectMax, IM_COL32(color.x * 255, color.y * 255, color.z * 255, color.w * 255), 10.0f);
        ImGui::SetWindowFontScale((ImGui::GetColumnWidth() / 10) * 0.25f);
        ImGui::GetWindowDrawList()->AddText(ImVec2(rectMin.x, rectMin.y + (height * 0.005f)), ImColor(255, 255, 255), ICON_FA_CUBE);
        ImGui::SetWindowFontScale(1.0f);
        // Draw Seperator line
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(rectMin.x, rectMin.y + height * 0.075f), ImVec2(rectMax.x, rectMin.y + height * 0.075f), IM_COL32(0.4f * 255, 0.322f * 255, 0.89f * 255, 255), 10.0f);
        ImGui::GetWindowDrawList()->AddText(ImVec2(rectMin.x, rectMin.y + height * 0.075f), ImColor(255, 255, 255), asset.fileName);


        //ImGui::TableNextRow();
    }

    ImGui::EndTable();
}