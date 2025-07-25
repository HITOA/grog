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
        void BeginHeader();
        void EndHeader();
        void BeginContent();
        void EndContent();
        void BeginInput();
        void EndInput();
        void BeginOutput();
        void EndOutput();
        void BeginFooter();
        void EndFooter();

    private:
        ax::NodeEditor::NodeId id;

        ImVec2 headerMin;
        ImVec2 headerMax;
        ImVec2 inputMin;
        ImVec2 inputMax;
        ImVec2 outputMin;
        ImVec2 outputMax;
    };

}