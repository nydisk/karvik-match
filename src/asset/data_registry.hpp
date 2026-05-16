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
    std::vector<std::string> cardVec_;
    AssetRegistry& assets_;
public:
    explicit DataRegistry(AssetRegistry& assets);

    void registerCard(const std::string& id, const std::string& path);
    const CardDefinition& getCard(const std::string& id);
    void unloadCards();
    size_t cardCount() const;
    const std::unordered_map<std::string, CardDefinition>& cards() const;
    const std::vector<std::string>& cardVec() const;
};
