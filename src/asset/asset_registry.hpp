#pragma once
#include <functional>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

#include "asset_definitions.hpp"
#include "spdlog/spdlog.h"

typedef std::function<std::unique_ptr<Asset>(const std::string&)> asset_factory_t;

class AssetRegistry {
    std::unordered_map<std::string, std::unique_ptr<Asset>> assets_{};
    std::unordered_map<std::type_index, asset_factory_t> factories_{};
public:
    static constexpr auto DataPrefix = "../data/";
    template<typename T> requires std::is_base_of_v<Asset, T>
    void registerFactory(asset_factory_t factory) {
        factories_[typeid(T)] = std::move(factory);
        spdlog::debug("factory {} registered", typeid(T).name());
    }

    template<typename T> requires std::is_base_of_v<Asset, T>
    void registerSimpleFactory() {
        factories_[typeid(T)] = [](const std::string& path) {
            return std::make_unique<T>(path);
        };
        spdlog::debug("(simple) factory {} registered", typeid(T).name());
    }

    template<typename T> requires std::is_base_of_v<Asset, T>
    T* load(const std::string& id, const std::string& path, const bool ignoreDataPath = false) {
        const auto it = factories_.find(typeid(T));
        if (it == factories_.end()) {
            spdlog::error("no factory for type {}", typeid(T).name());
            return nullptr;
        }
        assets_[id] = it->second(ignoreDataPath ? path : DataPrefix + path);
        spdlog::debug("asset {} loaded", id);
        return static_cast<T*>(assets_[id].get());
    }

    template<typename T> requires std::is_base_of_v<Asset, T>
    T* get(const std::string& id) {
        return static_cast<T*>(assets_.at(id).get());
    }

    void unload(const std::string& id) {
        const auto it = assets_.find(id);
        if (it == assets_.end()) {
            spdlog::error("no such asset {}", id);
            return;
        }
        assets_.erase(it);
        spdlog::debug("asset {} unloaded", id);
    }
};