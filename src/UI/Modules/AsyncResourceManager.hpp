#pragma once

#include <UI/Module.hpp>
#include <UI/Event.hpp>
#include <UI/NodeRegistry.hpp>

#include <filesystem>
#include <thread>
#include <memory>


namespace Grog {

    class AsyncResourceManager : public Module, public EventListener {
    public:
        //Event
        struct CheckUpdateEvent {
            std::string resourceURL;
            bool autoLoadAfterUpdate;
        };
        struct UpdateEvent {
            std::string resource;
            bool autoLoadAfterUpdate;
        };
        struct LoadEvent {};

        struct ResourcesLoadedEvent {
            std::shared_ptr<NodeRegistry> nodeRegistry;
        };

    public:
        void Initialize() override;
        void Terminate() override;

        bool OnEvent(EventHandle& handle) override;

    private:
        void CheckUpdate(std::string resourceURL, bool autoLoadAfterUpdate);
        void Update(std::string resource, bool autoLoadAfterUpdate);
        void Load();

        int LoadNodesInRegistry(const std::filesystem::path& nodesDirectory, std::shared_ptr<NodeRegistry> nodeRegistry);

        void ShowError(const std::string& name, const std::string& msg);

    private:
        std::jthread runningThread{};
    };

}