#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

class Registry {
	inline static std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_tex{};
	inline static std::unordered_map<std::string, std::unique_ptr<sf::Sound>> m_snd{};
	inline static std::vector<std::unique_ptr<sf::SoundBuffer>> m_sndbf{};
public:
	[[nodiscard]] inline static const sf::Texture& getTexture(const std::string& id) {
		auto it = m_tex.find(id);
		if (it == m_tex.end()) throw std::out_of_range("tex not found: " + id);
		return *(it->second);
	}
	[[nodiscard]] inline static bool loadTexture(const std::string& filename, const std::string& id) {
		std::unique_ptr<sf::Texture> tex = std::make_unique<sf::Texture>();
		if (!tex->loadFromFile(filename)) return false;
		m_tex[id] = std::move(tex);
		std::cout << "loaded texture: '" << id << "' @ " << filename << std::endl;
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

		std::cout << "loaded sound: '" << id << "' @ " << filename << std::endl;
		return true;
	}
};