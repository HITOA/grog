#include "AsyncResourceManager.hpp"

#include <UI/Application.hpp>
#include <UI/Modules/Logger.hpp>
#include <UI/Modules/ModalWindowManager.hpp>

#include <iostream>

#include <curl/curl.h>



void Grog::AsyncResourceManager::Initialize() {
    Config& config = GetApplication()->GetConfig();

    resourceDirectoryPath = config.GetResourcesDirectory();
    userResourceDirectoryPath = config.GetUserResourcesDirectory();

    GetApplication()->GetEventBus().AddListener(this);

    GetApplication()->GetEventBus().Push(std::make_shared<CheckUpdateEvent>(config.GetResourcesURL(), true));
}

void Grog::AsyncResourceManager::Terminate() {
    GetApplication()->GetEventBus().RemoveListener(this);
}

bool Grog::AsyncResourceManager::OnEvent(EventHandle& handle) {
    if (auto event = handle.Get<CheckUpdateEvent>()) {
        runningThread = std::jthread{ &AsyncResourceManager::CheckUpdate, this, event->resourceURL, event->autoLoadAfterUpdate };
        return true;
    }
    if (auto event = handle.Get<UpdateEvent>()) {
        return true;
    }
    return false;
}

void Grog::AsyncResourceManager::CheckUpdate(std::string resourceURL, bool autoLoadAfterUpdate) {
    EventBus& eventBus = GetApplication()->GetEventBus();

    CURL* curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, resourceURL.c_str());
    CURLcode res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);

    if (res == CURLE_OK) {
        eventBus.Push(std::make_shared<Logger::LogMessageEvent>(
            Message{ Message::MessageSeverity::Error, "Could not reach distant repository for update." }
        ));
        eventBus.Push(std::make_shared<ModalWindowManager::ShowModalWindowEvent>(

        ));
        if (autoLoadAfterUpdate) {
            // Still load even if it failed
            eventBus.Push(std::make_shared<AsyncResourceManager::LoadResourcesEvent>());
        }
        return;
    }

}