#pragma once

#include <UI/Module.hpp>
#include <UI/Event.hpp>
#include <UI/NodeRegistry.hpp>

#include <imgui_node_editor.h>

#include <memory>


namespace Grog {
    
    class GraphEditor : public Module, public EventListener {
    public:
        void Initialize() override;
        void Terminate() override;

        void DrawWidget() override;
        
        bool OnEvent(EventHandle& handle) override;

    private:
        ax::NodeEditor::EditorContext* context = nullptr;

        std::shared_ptr<NodeRegistry> nodeRegistry = nullptr;
    };

}