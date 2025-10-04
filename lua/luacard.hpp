#pragma once
#include <string>
#include <vector>
#include "luahelper.hpp"

struct LuaCardDefinition {
	std::string id;
	std::string filename;
};

class LuaCard {
	inline static std::vector<LuaCardDefinition> m_definitions;
public:
	inline static void reload() {
		std::cout << "luacard: reloading card definitions" << std::endl;

	}
	inline static const std::vector<LuaCardDefinition>& definitions() {
		return m_definitions;
	}
};