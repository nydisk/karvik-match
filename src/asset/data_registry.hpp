#pragma once
#include <string>
#include <unordered_map>

class AssetRegistry;

struct CardDefinition {
    std::string id;
    std::string path;
};

class DataRegistry {
    std::unordered_map<std::string, CardDefinition> cards_;
    AssetRegistry& assets_;
public:
    explicit DataRegistry(AssetRegistry& assets);

    void registerCard(const std::string& id, const std::string& path);
    const CardDefinition& getCard(const std::string& id);
    void unloadCards();
    size_t cardCount() const;
};
