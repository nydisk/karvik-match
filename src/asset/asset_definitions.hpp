#pragma once
#include <string>
#include "raylib.h"

class Asset {
public:
    virtual ~Asset() = default;
};

class TextureAsset : public Asset {
    Texture2D tex_;
public:
    explicit TextureAsset(const std::string& path);
    ~TextureAsset() override;
    Texture2D& tex();
};