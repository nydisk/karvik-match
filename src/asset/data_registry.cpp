#include "data_registry.hpp"

#include <ranges>

#include "asset_definitions.hpp"
#include "asset_registry.hpp"
#include "spdlog/spdlog.h"

DataRegistry::DataRegistry(AssetRegistry& assets): assets_(assets) {}

void DataRegistry::registerCard(const std::string& id, const std::string& path) {
    assets_.load<TextureAsset>("card_" + id, "cards/" + path);
    cards_[id] = CardDefinition{id, path};
    spdlog::debug("registered card {}", id);
}

const CardDefinition& DataRegistry::getCard(const std::string& id) {
    return cards_.at(id);
}

void DataRegistry::unloadCards() {
    for (const auto& key: cards_ | std::views::keys) {
        assets_.unload("card_" + key);
    }
    cards_.clear();
}

size_t DataRegistry::cardCount() const { return cards_.size(); }
