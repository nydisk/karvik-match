#pragma once

struct ManifestDefinition {
	std::string id;
	std::string path;
};
class Manifest {
public:
	inline static std::vector<ManifestDefinition> readManifest(const std::string& manifestPath) {
		std::ifstream manifest(manifestPath, std::ios::in);
		if (!manifest.is_open()) {
			MessageBoxA(nullptr, "failed to load manifest:\ncouldn't locate data manifest file", "fatal error", MB_OK | MB_ICONERROR);
			exit(EXIT_FAILURE);
		}

		std::vector<ManifestDefinition> definitions;
		std::string line;
		while (std::getline(manifest, line)) {
			if (!line.starts_with("tx ")) continue;
			std::istringstream ss(line.substr(3));

			std::string id, path;
			ss >> id >> path;

			definitions.emplace_back(id, path);
		}

		manifest.close();
		return definitions;
	}
};