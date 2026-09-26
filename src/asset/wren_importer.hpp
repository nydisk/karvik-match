#pragma once
#include <string>
#include "wren.hpp"

class WrenImporter {
    WrenVM* vm_;
    WrenConfiguration config_;
public:
    explicit WrenImporter();

    void interpretWrenFile(const std::string& module, const std::string& path) const;

    void free() const;
    [[nodiscard]] WrenVM* vm() const;

    static void wrenRegisterCard(WrenVM* vm);

private:
    static WrenVM* initializeWren(WrenConfiguration& config);
    static std::string loadWrenFile(const std::string& path);
};
