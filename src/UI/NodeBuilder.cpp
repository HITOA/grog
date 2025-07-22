#include "NodeBuilder.hpp"

#include <imgui.h>


Grog::NodeBuilder::NodeBuilder(ax::NodeEditor::NodeId id) : id{ id } {

}

void Grog::NodeBuilder::Begin() {
    ax::NodeEditor::BeginNode(id);
    ImGui::PushID(id.AsPointer());
}

void Grog::NodeBuilder::End() {
    ImGui::PopID();
    ax::NodeEditor::EndNode();
}