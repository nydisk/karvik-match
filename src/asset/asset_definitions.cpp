#include "asset_definitions.hpp"

TextureAsset::TextureAsset(const std::string& path): tex_(LoadTexture(path.c_str())) {}
TextureAsset::~TextureAsset() { UnloadTexture(tex_); }
Texture2D& TextureAsset::tex() { return tex_; }
