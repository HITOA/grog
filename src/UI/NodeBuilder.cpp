#include "NodeBuilder.hpp"

#include <imgui.h>


Grog::NodeBuilder::NodeBuilder(ax::NodeEditor::NodeId id) : id{ id } {

}

void Grog::NodeBuilder::Begin() {
    ax::NodeEditor::BeginNode(id);
    ImGui::PushID(id.AsPointer());
    ImGui::BeginVertical("Node");
}

void Grog::NodeBuilder::End() {
    ImGui::EndVertical();
    ImGui::PopID();
    ax::NodeEditor::EndNode();

    if (ImGui::IsItemVisible()) {
        ImDrawList* drawList = ax::NodeEditor::GetNodeBackgroundDrawList(id);
        ax::NodeEditor::Style& style = ax::NodeEditor::GetStyle();

        // Draw darker background on output
        drawList->AddRectFilled(
            ImVec2{ outputMin.x - style.NodePadding.x / 2.0f, outputMin.y - style.NodePadding.y / 2.0f }, 
            ImVec2{ outputMax.x + style.NodePadding.x - style.NodeBorderWidth, outputMax.y + style.NodePadding.y - style.NodeBorderWidth }, 
            ImColor{ 60, 60, 62, 255 },
            style.NodeRounding,
            ImDrawFlags_RoundCornersBottomRight);
        
        // Draw separator line
        float lineWidth = 0.35f;
        drawList->AddLine(
            ImVec2{ outputMin.x - style.NodePadding.x / 2.0f - lineWidth / 2.0f, outputMin.y - style.NodePadding.y / 2.0f },
            ImVec2{ outputMin.x - style.NodePadding.x / 2.0f - lineWidth / 2.0f, outputMax.y + style.NodePadding.y - style.NodeBorderWidth - 0.5f },
            ImColor{ 48, 48, 49, 255 },
            lineWidth
        );
        drawList->AddLine(
            ImVec2{ headerMin.x - style.NodePadding.x + style.NodeBorderWidth, outputMin.y - style.NodePadding.y / 2.0f - lineWidth / 2.0f },
            ImVec2{ headerMax.x + style.NodePadding.x - style.NodeBorderWidth - 0.5f, outputMin.y - style.NodePadding.y / 2.0f - lineWidth / 2.0f },
            ImColor{ 48, 48, 49, 255 },
            lineWidth
        );

        // Redraw node border
        ImVec2 nodePosition = ax::NodeEditor::GetNodePosition(id);
        ImVec2 nodeSize = ax::NodeEditor::GetNodeSize(id);
        drawList->AddRect(nodePosition,
            ImVec2{ nodePosition.x + nodeSize.x, nodePosition.y + nodeSize.y },
            ImColor{ style.Colors[ax::NodeEditor::StyleColor_NodeBorder] },
            style.NodeRounding,
            0,
            style.NodeBorderWidth);
    }
}

void Grog::NodeBuilder::BeginHeader() {
    ImGui::BeginHorizontal("Header");
}

void Grog::NodeBuilder::EndHeader() {
    ImGui::Dummy(ImVec2{ 0.0f, 24.0f });
    ImGui::EndHorizontal();
    headerMin = ImGui::GetItemRectMin();
    headerMax = ImGui::GetItemRectMax();
}

void Grog::NodeBuilder::BeginContent() {
    ImGui::BeginHorizontal("Content");
    ImGui::Spring(0.0f, 0.0f);
}

void Grog::NodeBuilder::EndContent() {
    ImGui::EndHorizontal();
}

void Grog::NodeBuilder::BeginInput() {
    ImGui::BeginVertical("Input", ImVec2{ 0.0f, 0.0f }, 0.0f);
}

void Grog::NodeBuilder::EndInput() {
    ImGui::Spring(1.0f, 0.0f);
    ImGui::EndVertical();
    inputMin = ImGui::GetItemRectMin();
    inputMax = ImGui::GetItemRectMax();
}

void Grog::NodeBuilder::BeginOutput() {
    ImGui::Spring(1.0f);
    ImGui::BeginVertical("Output", ImVec2{ 0.0f, 0.0f }, 0.0f);
}

void Grog::NodeBuilder::EndOutput() {
    ImGui::Spring(1.0f, 0.0f);
    ImGui::EndVertical();
    outputMin = ImGui::GetItemRectMin();
    outputMax = ImGui::GetItemRectMax();
}

void Grog::NodeBuilder::BeginFooter() {
    ImGui::BeginHorizontal("Footer");
}

void Grog::NodeBuilder::EndFooter() {
    ImGui::EndHorizontal();
}