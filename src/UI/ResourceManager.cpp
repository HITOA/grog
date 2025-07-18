#include "ResourceManager.hpp"

#include <curl/curl.h>

#include <iostream>



void Grog::ResourceManager::Initialize(std::filesystem::path& defaultResourcesDirectory, 
    std::filesystem::path& userResourcesDirectory, 
    const std::string& resourceURL) {
    
    this->resourceURL = resourceURL;
    this->defaultResourcesDirectory = defaultResourcesDirectory;
    this->userResourcesDirectory = userResourcesDirectory;

    CheckForResourcesUpdate();
}

void Grog::ResourceManager::Shutdown() {

}

bool Grog::ResourceManager::CheckForResourcesUpdate() {
    std::filesystem::path resourceFilePath = defaultResourcesDirectory / "resource.json";
    
    CURL* curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, resourceURL.c_str());
    CURLcode res = curl_easy_perform(curl);

    std::cout << (int)res << std::endl;

    curl_easy_cleanup(curl);

    return true;
}

void Grog::ResourceManager::UpdateResources() {

}

void Grog::ResourceManager::LoadResources() {

}

void Grog::ResourceManager::UnloadResources() {

}