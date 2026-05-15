#include "wren_importer.hpp"

#include <fstream>

#include "asset_registry.hpp"
#include "data_registry.hpp"
#include "spdlog/spdlog.h"

WrenImporter::WrenImporter(): config_() {
    vm_ = initializeWren(config_);
    interpretWrenFile("core", "core.wren");
}

void WrenImporter::interpretWrenFile(const std::string& module, const std::string& path) const {
    switch (wrenInterpret(vm_, module.c_str(), loadWrenFile(path).c_str())) {
        case WREN_RESULT_SUCCESS:
            spdlog::info("interpreted {} ({})", module, path);
            break;
        case WREN_RESULT_COMPILE_ERROR:
            spdlog::error("compile error {} ({})", module, path);
            break;
        case WREN_RESULT_RUNTIME_ERROR:
            spdlog::error("runtime error {} ({})", module, path);
            break;
    }
}

void WrenImporter::free() const {
    wrenFreeVM(vm_);
}

WrenVM* WrenImporter::vm() const { return vm_; }

void WrenImporter::wrenRegisterCard(WrenVM* vm) {
    const char* id = wrenGetSlotString(vm, 1);
    const char* file = wrenGetSlotString(vm, 2);
    const auto data = static_cast<DataRegistry*>(wrenGetUserData(vm));
    data->registerCard(id, file);
}

WrenVM* WrenImporter::initializeWren(WrenConfiguration& config) {
    wrenInitConfiguration(&config);

    config.writeFn = [](WrenVM*, const char* text) { spdlog::info("[wren] {}", text); };
    config.errorFn = [](WrenVM*, WrenErrorType, const char* module, int line, const char* msg) { spdlog::error("[wren] {}:{} {}", module ? module : "?", line, msg); };
    config.bindForeignMethodFn = [](WrenVM*, const char*, const char* className, bool, const char* signature) -> WrenForeignMethodFn {
        if (strcmp(className, "Core") == 0 && strcmp(signature, "f_registerCard(_,_)") == 0)
            return wrenRegisterCard;
        return nullptr;
    };

    WrenVM* vm = wrenNewVM(&config);
    spdlog::info("finished initializing wren");
    return vm;
}

std::string WrenImporter::loadWrenFile(const std::string& path) {
    const std::ifstream fs(AssetRegistry::DataPrefix + path);
    if (!fs.is_open()) {
        spdlog::error("failed to open wren file {}", path);
        return {};
    }

    std::stringstream ss;
    ss << fs.rdbuf();
    return ss.str();
}
