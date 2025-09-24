#pragma once
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <vector>
#include "registry.hpp"
#include "rng.hpp"
class BGM {
	inline static std::vector<RegistryMusicInfo*> m_playlist;
	inline static size_t m_nowPlaying = 0;
	inline static bool m_playing = false;
	inline static void playCurrent() {
		sf::Music* nowPlaying = m_playlist[m_nowPlaying]->pMusic.get();

		nowPlaying->setLooping(false); // just in case
		nowPlaying->play();
		
		std::cout << "bgm: now playing: '" + m_playlist[m_nowPlaying]->title + "' by " + m_playlist[m_nowPlaying]->author + " at 0x" << std::hex << std::showbase << nowPlaying << std::dec << std::noshowbase << std::endl;
	}
public:
	inline static RegistryMusicInfo* currentlyPlaying(){
		if (!m_playing) return nullptr;
		return m_playlist[m_nowPlaying];
	}
	inline static void queue(const std::string& id) {
		m_playlist.push_back(&Registry::getMusic(id));
		std::cout << "bgm: queued: " << id << std::endl;
	}
	inline static void shuffle() {
		std::shuffle(m_playlist.begin(), m_playlist.end(), RNG::gen());
		std::cout << "bgm: shuffled the bgm list" << std::endl;
	}
	inline static void play() {
		if (m_playlist.empty()) {
			std::cout << "bgm: no tracks to play :3" << std::endl;
			return;
		}
		m_playing = true;

		shuffle();
		playCurrent();

		std::cout << "bgm: playing " << m_playlist.size() << " tracks on loop + shuffle" << std::endl;
	}
	inline static void next() {
		if (m_playlist.empty() || !m_playing) return;

		std::cout << "bgm: playing next track" << std::endl;
		m_nowPlaying++;
		if (m_nowPlaying == m_playlist.size()) {
			replay();
		}


		playCurrent();
	}
	inline static void replay() {
		std::cout << "bgm: playlist finished, replaying" << std::endl;
		m_nowPlaying = 0;
		shuffle();
		play();
	}
	inline static void resetState() { 
		std::cout << "bgm: resetting state" << std::endl;
		if (m_playing)
			m_playlist[m_nowPlaying]->pMusic->stop();
		m_playlist.clear();
		m_nowPlaying = 0;
		m_playing = false;
	}
	inline static void update() {
		if (m_playlist.empty() || !m_playing) return;
		if (m_playlist[m_nowPlaying]->pMusic->getStatus() == sf::SoundStream::Status::Stopped)
			next();
	}
};