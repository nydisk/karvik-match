#pragma once
#include <string>
#include <vector>
#include "luahelper.hpp"
#include <SFML/Graphics.hpp>

struct LuaCardDefinition {
	std::string id;
	std::string filename;
	std::unique_ptr<sf::Texture> pTexture;
};

class LuaCard {
	inline static std::vector<LuaCardDefinition> m_definitions{};
public:
	inline static void reload() {
		std::cout << "luacard: reloading card definitions" << std::endl;
		m_definitions.clear();

		for (const auto& entry : LuaHelper::dataMap()) {
			if (entry.second.type != LuaDataType::Card) continue;
			if (!entry.second.entryTable["file"].valid() || !entry.second.entryTable["file"].is<std::string>())
				LuaHelper::panicDie("luacard error:\n\ncard '" + entry.first + "' has no defined 'file' field for filename / filepath");

			std::string file = entry.second.entryTable["file"].get<std::string>();

			std::unique_ptr<sf::Texture> tex = std::make_unique<sf::Texture>();
			if (!tex->loadFromFile("data/cards/" + file))
				LuaHelper::panicDie("luacard error:\n\ncard '" + entry.first + "':\ncould not locate defined image file in data/cards/" + file);

			m_definitions.emplace_back(entry.first, file, std::move(tex));
		}
	}
	inline static const std::vector<LuaCardDefinition>& definitions() {
		return m_definitions;
	}
	inline static sf::Texture* getCardTexture(const std::string& cardId) {
		for (const auto& def : m_definitions) {
			if (def.id == cardId) return def.pTexture.get();
		}
		return nullptr;
	}
};