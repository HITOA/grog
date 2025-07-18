#pragma once

#include <UI/Module.hpp>
#include <UI/Event.hpp>

#include <filesystem>
#include <thread>


namespace Grog {

    class AsyncResourceManager : public Module, public EventListener {
    public:
        //Event
        struct CheckUpdateEvent {
            std::string resourceURL;
            bool autoLoadAfterUpdate;
        };
        struct UpdateEvent {
            std::string resourceURL;
            bool autoLoadAfterUpdate;
        };
        struct LoadResourcesEvent {};

    public:
        void Initialize() override;
        void Terminate() override;

        bool OnEvent(EventHandle& handle) override;

    private:
        void CheckUpdate(std::string resourceURL, bool autoLoadAfterUpdate);

    private:
        std::filesystem::path resourceDirectoryPath{};
        std::filesystem::path userResourceDirectoryPath{};

        std::jthread runningThread{};
    };

}