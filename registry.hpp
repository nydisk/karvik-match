#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

struct RegistryMusicInfo {
	std::unique_ptr<sf::Music> pMusic;
	std::string author;
	std::string title;
};

class Registry {
public:
	inline static constexpr int POSTERIZATION_LEVELS = 96;
private:
	inline static std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_tex{};
	inline static std::unordered_map<std::string, std::unique_ptr<sf::Sound>> m_snd{};
	inline static std::vector<std::unique_ptr<sf::SoundBuffer>> m_sndbf{};
	inline static std::unordered_map<std::string, RegistryMusicInfo> m_msc{};
	inline static std::unique_ptr<sf::Shader> m_globalPosterizationShader{};
	inline static std::unique_ptr<sf::Font> m_globalFont{};
public:
	[[nodiscard]] inline static const sf::Font& getFont() {
		return *m_globalFont;
	}
	[[nodiscard]] inline static bool loadFont(const std::string& path) {
		m_globalFont = std::make_unique<sf::Font>();
		if (!m_globalFont->openFromFile(path)) return false;
		return true;
	}

	[[nodiscard]] inline static const sf::Shader& getGlobalPosterizationShader() {
		return *m_globalPosterizationShader;
	}
	[[nodiscard]] inline static const bool loadGlobalPosterizationShader(const std::string& path) {
		m_globalPosterizationShader = std::make_unique<sf::Shader>();
		if (!m_globalPosterizationShader->loadFromFile(path, sf::Shader::Type::Fragment)) return false;
		m_globalPosterizationShader->setUniform("levels", POSTERIZATION_LEVELS);
		return true;
	}

	[[nodiscard]] inline static const sf::Texture& getTexture(const std::string& id) {
		auto it = m_tex.find(id);
		if (it == m_tex.end()) throw std::out_of_range("tex not found: " + id);
		return *(it->second);
	}
	[[nodiscard]] inline static bool loadTexture(const std::string& filename, const std::string& id) {
		std::unique_ptr<sf::Texture> tex = std::make_unique<sf::Texture>();
		if (!tex->loadFromFile(filename)) return false;
		m_tex[id] = std::move(tex);
		std::cout << "registry: loaded texture '" << id << "' @ " << filename << std::endl;
		return true;
	}

	[[nodiscard]] inline static RegistryMusicInfo& getMusic(const std::string& id) {
		auto it = m_msc.find(id);
		if (it == m_msc.end()) throw std::out_of_range("msc not found: " + id);
		return it->second;
	}
	[[nodiscard]] inline static bool loadMusic(const std::string& filename, const std::string& id, const std::string& author, const std::string& title) {
		std::unique_ptr<sf::Music> msc = std::make_unique<sf::Music>();
		if (!msc->openFromFile(filename)) return false;
		m_msc[id] = { std::move(msc), author, title };
		std::cout << "registry: loaded music '" << id << "' @ " << filename << " by " << author << " titled '" << title << "'" << std::endl;
		return true;
	}

	[[nodiscard]] inline static sf::Sound& getSound(const std::string& id) {
		auto it = m_snd.find(id);
		if (it == m_snd.end()) throw std::out_of_range("snd not found: " + id);
		return *(it->second);
	}
	[[nodiscard]] inline static bool loadSound(const std::string& filename, const std::string& id) {
		auto buf = std::make_unique<sf::SoundBuffer>();
		if (!buf->loadFromFile(filename)) return false;

		m_sndbf.push_back(std::move(buf));
		std::unique_ptr<sf::Sound> snd = std::make_unique<sf::Sound>(*m_sndbf.back());
		m_snd[id] = std::move(snd);

		std::cout << "registry: loaded sound '" << id << "' @ " << filename << std::endl;
		return true;
	}
	inline static void deathAndDestruction() {
		m_tex.clear();
		m_snd.clear();
		m_sndbf.clear();
		m_msc.clear();
		m_globalPosterizationShader.reset();
	}
};