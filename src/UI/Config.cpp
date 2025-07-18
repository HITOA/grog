#include "Config.hpp"

#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>

#include <UI/Filesystem.hpp>



Grog::Config::Config() {
    resourcesURL = "https://raw.githubusercontent.com/HITOA/grog-resources/refs/heads/main/resources.json";

    std::filesystem::path resourcesPath = Filesystem::GetUserDataDirectoryPath() / "Grog";
    resourcesDirectory = resourcesPath.string();
    userResourcesDirectory = resourcesPath.string();
}

std::string Grog::Config::GetResourcesURL() {
    return resourcesURL;
}

void Grog::Config::SetResourcesURL(const std::string& url) {
    resourcesURL = url;
}

std::string Grog::Config::GetResourcesDirectory() {
    return resourcesDirectory;
}

void Grog::Config::SetResourcesDirectory(const std::string& path) {
    resourcesDirectory = path;
}

std::string Grog::Config::GetUserResourcesDirectory() {
    return userResourcesDirectory;
}

void Grog::Config::SetUserResourcesDirectory(const std::string& path) {
    userResourcesDirectory = path;
}

void Grog::Config::SaveToDisk(std::filesystem::path& path) {
    nlohmann::json obj{};

    obj["resourcesURL"] = resourcesURL;
    obj["resourcesDirectory"] = resourcesDirectory;
    obj["userResourcesDirectory"] = userResourcesDirectory;

    std::ofstream outputFile{ path };
    if (outputFile.is_open()) {
        outputFile << obj.dump(4);
        outputFile.close();
    }
}

void Grog::Config::LoadFromDisk(std::filesystem::path& path) {
    std::ifstream inputFile{ path };
    if (!inputFile.is_open())
        return;
    
    nlohmann::json obj = nlohmann::json::parse(inputFile);

    if (obj.count("resourcesURL"))
        if (obj["resourcesURL"].is_string())
            resourcesURL = obj["resourcesURL"].get<std::string>();

    if (obj.count("resourcesDirectory"))
        if (obj["resourcesDirectory"].is_string())
            resourcesDirectory = obj["resourcesDirectory"].get<std::string>();

    if (obj.count("userResourcesDirectory"))
        if (obj["userResourcesDirectory"].is_string())
            userResourcesDirectory = obj["userResourcesDirectory"].get<std::string>();
}
