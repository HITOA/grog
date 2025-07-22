#include "AsyncResourceManager.hpp"

#include <UI/Application.hpp>
#include <UI/Modules/Logger.hpp>
#include <UI/Modules/ModalWindowManager.hpp>
#include <UI/Filesystem.hpp>

#include <VCL/Source.hpp>

#include <iostream>
#include <fstream>
#include <format>

#include <curl/curl.h>
#include <miniz.h>
#include <nlohmann/json.hpp>


class UpdateResourcesModalWindow : public Grog::ModalWindow {
public:
    UpdateResourcesModalWindow(const std::string& msg, const std::string& resource, Grog::EventBus* eventBus) :
        msg{ msg }, resource{ resource }, eventBus{ eventBus } {}

    std::string GetName() override {
        return "Update Resources";
    }

    void Draw() override {
        ImGui::TextUnformatted(msg.c_str());
        ImGui::Separator();
        if (ImGui::Button("Update")) {
            eventBus->Push(std::make_shared<Grog::AsyncResourceManager::UpdateEvent>(resource, true));
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            eventBus->Push(std::make_shared<Grog::AsyncResourceManager::LoadEvent>());
            ImGui::CloseCurrentPopup();
        }
    }

private:
    std::string msg;
    std::string resource;
    Grog::EventBus* eventBus;
};

static size_t WriteToStringStream(void* buffer, size_t size, size_t nmemb, void* data) {
    std::stringstream* ss = (std::stringstream*)data;
    ss->write((const char*)buffer, size * nmemb);
    return size * nmemb;
}

static size_t WriteToBuffer(void* buffer, size_t size, size_t nmemb, void* data) {
    std::vector<uint8_t>* b = (std::vector<uint8_t>*)data;
    size_t i = b->size();
    b->resize(i + size * nmemb);
    memcpy(b->data() + i, buffer, size * nmemb);
    return size * nmemb;
}

void Grog::AsyncResourceManager::Initialize() {
    Config& config = GetApplication()->GetConfig();

    GetApplication()->GetEventBus().AddListener(this);

    GetApplication()->GetEventBus().Push(std::make_shared<CheckUpdateEvent>(config.GetResourcesURL(), true));
}

void Grog::AsyncResourceManager::Terminate() {
    if (runningThread.joinable())
        runningThread.join();

    GetApplication()->GetEventBus().RemoveListener(this);
}

bool Grog::AsyncResourceManager::OnEvent(EventHandle& handle) {
    if (auto event = handle.Get<CheckUpdateEvent>()) {
        runningThread = std::jthread{ &AsyncResourceManager::CheckUpdate, this, event->resourceURL, event->autoLoadAfterUpdate };
        return true;
    }
    if (auto event = handle.Get<UpdateEvent>()) {
        runningThread = std::jthread{ &AsyncResourceManager::Update, this, event->resource, event->autoLoadAfterUpdate };
        return true;
    }
    if (auto event = handle.Get<LoadEvent>()) {
        runningThread = std::jthread{ &AsyncResourceManager::Load, this };
        return true;
    }
    return false;
}

void Grog::AsyncResourceManager::CheckUpdate(std::string resourceURL, bool autoLoadAfterUpdate) {
    EventBus& eventBus = GetApplication()->GetEventBus();
    Config& config = GetApplication()->GetConfig();

    CURL* curl = curl_easy_init();
    std::stringstream ss{};

    curl_easy_setopt(curl, CURLOPT_URL, resourceURL.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToStringStream);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ss);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    
    CURLcode res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        ShowError("Update Error", "Could not reach distant repository for update.");
        if (autoLoadAfterUpdate)
            eventBus.Push(std::make_shared<AsyncResourceManager::LoadEvent>());
        return;
    }

    nlohmann::json distantResource = nlohmann::json::parse(ss);
    
    std::filesystem::path localResourcePath{ config.GetResourcesDirectory() }; 
    localResourcePath /= "resources.json";

    if (!std::filesystem::exists(localResourcePath)) {
        eventBus.Push(std::make_shared<ModalWindowManager::ShowModalWindowEvent>(
            std::make_unique<UpdateResourcesModalWindow>(
                "It seems to be the first time you open Grog.\nWill you download the default resources (recommended) ?", 
                ss.str(), &eventBus) 
        ));
        return;
    }

    nlohmann::json localResource = nlohmann::json::parse(std::ifstream{ localResourcePath });

    int localMajor = 0, distantMajor = 0;
    int localMinor = 0, distantMinor = 0;
    int localPatch = 0, distantPatch = 0;

    if (localResource.count("version")) {
        nlohmann::json& localVersion = localResource["version"];
        localMajor = localVersion["major"].get<int>();
        localMinor = localVersion["minor"].get<int>();
        localPatch = localVersion["patch"].get<int>();
    }

    if (distantResource.count("version")) {
        nlohmann::json& distantVersion = distantResource["version"];
        distantMajor = distantVersion["major"].get<int>();
        distantMinor = distantVersion["minor"].get<int>();
        distantPatch = distantVersion["patch"].get<int>();
    }

    bool shouldUpdate = false;
    std::string msg = "";

    if (distantMajor > localMajor) {
        shouldUpdate = true;
        msg = "Major resources update available.\nWill you update ?";
    } else if (distantMinor > localMinor) {
        shouldUpdate = true;
        msg = "Minor resources update available.\nWill you update ?";
    } else if (distantPatch > localPatch) {
        shouldUpdate = true;
        msg = "Patch resources update available.\nWill you update ?";
    }

    if (shouldUpdate) {
        eventBus.Push(std::make_shared<ModalWindowManager::ShowModalWindowEvent>(
            std::make_unique<UpdateResourcesModalWindow>(msg, ss.str(), &eventBus) 
        ));
    } else {
        eventBus.Push(std::make_shared<AsyncResourceManager::LoadEvent>());
    }
}

void Grog::AsyncResourceManager::Update(std::string resource, bool autoLoadAfterUpdate) {
    EventBus& eventBus = GetApplication()->GetEventBus();
    Config& config = GetApplication()->GetConfig();

    nlohmann::json jsonResource = nlohmann::json::parse(resource);

    std::string packURL = "";
    if (jsonResource.count("pack"))
        packURL = jsonResource["pack"].get<std::string>();

    std::vector<uint8_t> buffer{};
    
    CURL* curl = curl_easy_init();
    
    curl_easy_setopt(curl, CURLOPT_URL, packURL.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToBuffer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    
    CURLcode res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);

    /*std::ofstream a{ "test.zip" };
    a.write((const char*)buffer.data(), buffer.size());
    a.close();*/

    if (res != CURLE_OK) {
        ShowError("Update Error", "Could not reach distant repository for update\nor the given pack url is wrong.");
        if (autoLoadAfterUpdate)
            eventBus.Push(std::make_shared<AsyncResourceManager::LoadEvent>());
        return;
    }
    
    // Uncompress the pack inside buffer and write the files to the resource directory of grog

    mz_zip_archive archive{};

    if (!mz_zip_reader_init_mem(&archive, buffer.data(), buffer.size(), 0)) {
        ShowError("Update Error", "Downloaded update is broken.");
        if (autoLoadAfterUpdate)
            eventBus.Push(std::make_shared<AsyncResourceManager::LoadEvent>());
        return;
    }

    mz_uint fileCount = mz_zip_reader_get_num_files(&archive);

    if (fileCount == 0) {
        ShowError("Update Error", "Downloaded update does not contain any file.");
        if (autoLoadAfterUpdate)
            eventBus.Push(std::make_shared<AsyncResourceManager::LoadEvent>());
        mz_zip_reader_end(&archive);
        return;
    }

    std::filesystem::path resourceDirectory{ config.GetResourcesDirectory() };

    std::filesystem::remove_all(resourceDirectory / "Nodes");
    std::filesystem::remove_all(resourceDirectory / "Presets");

    // Will have to fix this code, it work but it's not safe has it doesn't check the path before writing

    mz_zip_archive_file_stat fileStat{};
    for (mz_uint i = 0; i < fileCount; ++i) {
        mz_zip_reader_file_stat(&archive, i, &fileStat);
        std::filesystem::path decompressedEntryFullpath = resourceDirectory / fileStat.m_filename;
        if (mz_zip_reader_is_file_a_directory(&archive, i)) {
            std::filesystem::create_directories(decompressedEntryFullpath);
            continue;
        }
        std::filesystem::create_directories(decompressedEntryFullpath.parent_path());
        if (!mz_zip_reader_extract_to_file(&archive, i, decompressedEntryFullpath.c_str(), 0)) {
            ShowError("Update Error", "File could not be extracted.");
            if (autoLoadAfterUpdate)
                eventBus.Push(std::make_shared<AsyncResourceManager::LoadEvent>());
            mz_zip_reader_end(&archive);
            return;
        }
    }

    mz_zip_reader_end(&archive);

    std::ofstream resourceFile{ resourceDirectory / "resources.json", std::ios::trunc };
    resourceFile << jsonResource.dump(4);
    resourceFile.close();

    eventBus.Push(std::make_shared<ModalWindowManager::ShowModalWindowEvent>(
        std::make_unique<GenericMessageModalWindow>("Update", "Resources updated successfully.")
    ));

    if (autoLoadAfterUpdate)
        eventBus.Push(std::make_shared<AsyncResourceManager::LoadEvent>());
}

void Grog::AsyncResourceManager::Load() {
    EventBus& eventBus = GetApplication()->GetEventBus();
    Config& config = GetApplication()->GetConfig();

    std::filesystem::path resourceDirectory{ config.GetResourcesDirectory() };
    std::filesystem::path userResourceDirectory{ config.GetUserResourcesDirectory() };
    
    std::filesystem::path nodesDirectory = resourceDirectory / "Nodes";
    std::filesystem::path userNodesDirectory = userResourceDirectory / "User Nodes";

    std::shared_ptr<NodeRegistry> nodeRegistry = std::make_shared<NodeRegistry>();

    int loadedNodes = LoadNodesInRegistry(nodesDirectory, nodeRegistry);
    int loadedUserNodes = LoadNodesInRegistry(userNodesDirectory, nodeRegistry);
    
    eventBus.Push(std::make_shared<ResourcesLoadedEvent>(
        nodeRegistry
    ));
    
    eventBus.Push(std::make_shared<Logger::LogMessageEvent>(
        Message{ Message::MessageSeverity::Info, std::format("Successfully loaded {} node(s).", loadedNodes) }
    ));
    eventBus.Push(std::make_shared<Logger::LogMessageEvent>(
        Message{ Message::MessageSeverity::Info, std::format("Successfully loaded {} user node(s).", loadedUserNodes) }
    ));
}

int Grog::AsyncResourceManager::LoadNodesInRegistry(const std::filesystem::path& nodesDirectory, std::shared_ptr<NodeRegistry> nodeRegistry) {
    if (!std::filesystem::exists(nodesDirectory))
        return 0;
    EventBus& eventBus = GetApplication()->GetEventBus();
    int loadedNodes = 0;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(nodesDirectory)) {
        if (!entry.is_directory() && entry.path().extension() == ".vcl") {
            std::string relativeNodeName = std::filesystem::relative(entry.path(), nodesDirectory).replace_extension("").string();
            if (entry.file_size() <= 0) {
                std::string msg = std::format("Cannot load node `{}` because it does not have any source.", relativeNodeName);
                eventBus.Push(std::make_shared<Logger::LogMessageEvent>(
                    Message{ Message::MessageSeverity::Warning, msg }
                ));
                continue;
            }
            std::filesystem::path fullpath = entry.path();
            if (auto e = VCL::Source::LoadFromDisk(fullpath); e.has_value()) {
                std::shared_ptr<VCL::Source> source = *e;
                nodeRegistry->emplace(relativeNodeName, source);
                ++loadedNodes;
            } else {
                std::string msg = std::format("Cannot load node `{}` because of error:\n{}", relativeNodeName, e.error());
                eventBus.Push(std::make_shared<Logger::LogMessageEvent>(
                    Message{ Message::MessageSeverity::Warning, msg }
                ));
                continue;
            }
        }
    }
    return loadedNodes;
}

void Grog::AsyncResourceManager::ShowError(const std::string& name, const std::string& msg) {
    EventBus& eventBus = GetApplication()->GetEventBus();
    eventBus.Push(std::make_shared<Logger::LogMessageEvent>(
        Message{ Message::MessageSeverity::Error, msg }
    ));
    eventBus.Push(std::make_shared<ModalWindowManager::ShowModalWindowEvent>(
        std::make_unique<GenericMessageModalWindow>(name, msg)
    ));
}