#pragma once
#define WIN32_LEAN_AND_MEAN
#include <sol.hpp>
#include <filesystem>
#include <unordered_map>
#include <Windows.h>

enum class LuaDataType {
	Card,
	Other
};

struct LuaDataObject {
	LuaDataType type;
	sol::table entryTable;
};

class LuaHelper {
	inline static sol::state m_luaState{};
	inline static std::unordered_map<std::string, LuaDataObject> m_dataMap{};

	inline static const std::unordered_map<std::string, LuaDataType> m_dataTypeDict{
		{"card", LuaDataType::Card},
		{"other", LuaDataType::Other}
	};
	inline static const std::unordered_map<LuaDataType, std::string> m_reverseDataTypeDict{
		{LuaDataType::Card, "card"},
		{LuaDataType::Other, "other"}
	};
public:
	[[noreturn]] inline static void panicDie(const std::string& msg) {
		MessageBoxA(nullptr, msg.c_str(), "karvik-match lua_exec", MB_OK);
		exit(EXIT_FAILURE);
	}
	inline static void initialize() {
		m_luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table);
		std::cout << "lua: opened libraries" << std::endl;

		try {
			m_luaState.script_file("data/core.lua");
		}
		catch (const sol::error& e) {
			panicDie("failed to load core:\n\n" + std::string(e.what()));
		}
		std::cout << "lua: loaded core.lua" << std::endl;

		for (const auto& entry : std::filesystem::directory_iterator("./data/")) {
			if (entry.path().extension() != ".lua") continue;
			if (entry.path().filename() == "core.lua") continue;
			
			std::cout << "lua: loading " << entry.path().filename().string() << std::endl;
			try {
				m_luaState.script_file(entry.path().string());
			}
			catch (const sol::error& e) {
				panicDie("failed to load " + entry.path().string() + ":\n\n" + std::string(e.what()));
			}
		}

		if (!m_luaState["Core"]["data"].valid()) {
			panicDie("failed to initialize Lua:\n\ncould not locate data table in Core namespace.");
			exit(EXIT_FAILURE);
		}

		std::cout << "lua: seems ok" << std::endl;
	}
	inline static void reloadData() {
		std::cout << "lua: reloading datamap" << std::endl;
		sol::table data = m_luaState["Core"]["data"];
		for (const auto& entry : data) {
			if (!entry.first.is<std::string>())
				LuaHelper::panicDie("lua error:\n\ndata table key is not a string");
			if (!entry.second.is<sol::table>())
				LuaHelper::panicDie("lua error:\n\ndata table entry is not a table");

			std::string dataId = entry.first.as<std::string>();
			sol::table dataEntry = entry.second.as<sol::table>();

			if (!dataEntry.valid())
				LuaHelper::panicDie("lua error:\n\ndata table entry is invalid");
			if (!dataEntry["type"].valid() || !dataEntry["type"].is<std::string>())
				LuaHelper::panicDie("lua error:\n\ndata table entry type is nonexistent / not a string");
			if (!m_dataTypeDict.contains(dataEntry["type"].get<std::string>()))
				LuaHelper::panicDie("lua error:\n\ndata table entry type is invalid. datatype '" + dataEntry["type"].get<std::string>() + "' does not exist");

			LuaDataType dataType = m_dataTypeDict.at(dataEntry["type"].get<std::string>());
			m_dataMap[dataId] = {
				dataType,
				dataEntry
			};

			std::cout << "lua-data: loaded " << dataId << " (" << m_reverseDataTypeDict.at(dataType) << ")" << std::endl;
		}
		std::cout << "lua: reloaded data map" << std::endl;
	}
	inline static const std::unordered_map<std::string, LuaDataObject>& dataMap() {
		return m_dataMap;
	}
	inline static sol::state& state() {
		return m_luaState;
	}
};