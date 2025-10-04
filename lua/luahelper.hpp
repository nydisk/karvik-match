#pragma once
#include <sol.hpp>

class LuaHelper {
	inline static sol::state m_luaState;
public:
	inline static void initialize() {
		m_luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table);
		std::cout << "lua: opened libraries" << std::endl;
	}
	inline static sol::state& state() {
		return m_luaState;
	}
};