#pragma once

#include <filesystem>

#include <curl/curl.h>


namespace Grog {

    class ResourceManager {
    public:
        ResourceManager() = default;
        ~ResourceManager() = default;

        void Initialize(std::filesystem::path& defaultResourcesDirectory, std::filesystem::path& userResourcesDirectory, const std::string& resourceURL);
        void Shutdown();

        bool CheckForResourcesUpdate();
        void UpdateResources();
        void LoadResources();
        void UnloadResources();
    
    private:
        std::filesystem::path defaultResourcesDirectory{};
        std::filesystem::path userResourcesDirectory{};
        std::string resourceURL{};
    };

}