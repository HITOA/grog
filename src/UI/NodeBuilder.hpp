#pragma once

#include <imgui_node_editor.h>


namespace Grog {

    class NodeBuilder {
    public:
        NodeBuilder() = delete;
        NodeBuilder(ax::NodeEditor::NodeId id);
        ~NodeBuilder() = default;

        void Begin();
        void End();

    private:
        ax::NodeEditor::NodeId id;
    };

}