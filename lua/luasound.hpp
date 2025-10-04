#pragma once
#include "luahelper.hpp"
#include "core/registry.hpp"

class LuaSound {
public:
	inline static void reload() {
		std::cout << "luasound: reloading music definitions" << std::endl;
		Registry::clearMusic();

		for (const auto& entry : LuaHelper::dataMap()) {
			if (entry.second.type != LuaDataType::BGM) continue;
			if (!entry.second.entryTable["file"].valid() || !entry.second.entryTable["file"].is<std::string>())
				LuaHelper::panicDie("luasound error:\n\nmusic '" + entry.first + "' has no defined 'file' field for filename / filepath");
			if (!entry.second.entryTable["author"].valid() || !entry.second.entryTable["author"].is<std::string>())
				LuaHelper::panicDie("luasound error:\n\nmusic '" + entry.first + "' has no defined 'author' field");
			if (!entry.second.entryTable["title"].valid() || !entry.second.entryTable["title"].is<std::string>())
				LuaHelper::panicDie("luasound error:\n\nmusic '" + entry.first + "' has no defined 'title' field");
			if (!entry.second.entryTable["playsIn"].valid() || !entry.second.entryTable["playsIn"].is<sol::table>())
				LuaHelper::panicDie("luasound error:\n\nmusic '" + entry.first + "' has no defined 'playsIn' field (table) for scenes that play this music");

			std::string file = entry.second.entryTable["file"].get<std::string>();
			std::string title = entry.second.entryTable["title"].get<std::string>();
			std::string author = entry.second.entryTable["author"].get<std::string>();
			sol::table playsIn = entry.second.entryTable["playsIn"].get<sol::table>();

			std::vector<SceneId> scenesToPlayIn{};
			for (const auto& sc : playsIn) {
				if (!sc.second.valid() || !sc.second.is<std::string>())
					LuaHelper::panicDie("luasound error:\n\nmusic '" + entry.first + "' has an illdefined 'playsIn' field (table)");
				std::string plin = sc.second.as<std::string>();
				try {
					scenesToPlayIn.push_back(Scene::getSceneIdFromName(plin));
				}
				catch (...) {
					LuaHelper::panicDie("luasound error:\n\nmusic '" + entry.first + "' has set plays in to '" + plin + "', which isn't an existing scene");
				}
			}

			if (!Registry::loadMusic("data/sound/" + file, entry.first, author, title, scenesToPlayIn)) {
				LuaHelper::panicDie("luasound error:\n\nmusic '" + entry.first + "' has failed to load. are you sure the defined source file exists?");
			}
		}
	}
};