#include "asset_definitions.hpp"

TextureAsset::TextureAsset(const std::string& path): tex_(LoadTexture(path.c_str())) { SetTextureFilter(tex_, TEXTURE_FILTER_POINT); }
TextureAsset::~TextureAsset() { UnloadTexture(tex_); }
Texture2D& TextureAsset::tex() { return tex_; }
