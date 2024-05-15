#include "config.hpp"

static Config* configInstance = nullptr;

Config* Config::Instance() {
    if (!configInstance)
        configInstance = new Config();

    return configInstance;
}

void Config::Uninitialize() {
    configInstance = nullptr;
    delete this;
}