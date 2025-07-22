#pragma once

#include <UI/Module.hpp>
#include <UI/Event.hpp>
#include <UI/NodeRegistry.hpp>

#include <VCLG/Graph.hpp>

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
        void DrawAddNodePopup();
        void DrawAddNodeMenu(std::string::const_iterator&& begin, std::string::const_iterator&& end, NodeSource& source, const ImVec2& clickPos);

        void InstantiateNode(NodeSource& nodeSource, const ImVec2& position);
        void DrawNode(VCLG::Graph::NodeHandle handle);

    private:
        ax::NodeEditor::EditorContext* context = nullptr;
        std::string nodeAddPopupFilter{};

        std::shared_ptr<NodeRegistry> nodeRegistry = nullptr;
    };

}