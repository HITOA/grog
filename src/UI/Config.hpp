#pragma once

#include <filesystem>


namespace Grog {

    // I should probably make this thread safe at some point
    class Config {
    public:
        Config();
        ~Config() = default;

        std::string GetResourcesURL();
        void SetResourcesURL(const std::string& url);

        std::string GetResourcesDirectory();
        void SetResourcesDirectory(const std::string& path);

        std::string GetUserResourcesDirectory();
        void SetUserResourcesDirectory(const std::string& path);

        void SaveToDisk(std::filesystem::path& path);
        void LoadFromDisk(std::filesystem::path& path);

    private:
        std::string resourcesURL = "";
        std::string resourcesDirectory = "";
        std::string userResourcesDirectory = "";
    };

}