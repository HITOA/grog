#pragma once

#include <UI/Module.hpp>
#include <UI/Event.hpp>
#include <UI/NodeRegistry.hpp>

#include <memory>


namespace Grog {
    
    class GraphEditor : public Module, public EventListener {
    public:
        void Initialize() override;
        void Terminate() override;

        void Draw() override;
        
        bool OnEvent(EventHandle& handle) override;

    private:
        std::shared_ptr<NodeRegistry> nodeRegistry = nullptr;
    };

}