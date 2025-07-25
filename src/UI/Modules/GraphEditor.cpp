#include "GraphEditor.hpp"

#include <UI/Application.hpp>
#include <UI/Node.hpp>
#include <UI/NodeBuilder.hpp>
#include <UI/Modules/AsyncResourceManager.hpp>

#include <misc/cpp/imgui_stdlib.h>

#include <iostream>


void Grog::GraphEditor::Initialize() {
    GetApplication()->GetEventBus().AddListener(this);
    ax::NodeEditor::Config config{};
    config.SettingsFile = nullptr;
    config.NavigateButtonIndex = 2;
    config.EnableSmoothZoom = true;
    
    context = ax::NodeEditor::CreateEditor(&config);
    ax::NodeEditor::SetCurrentEditor(context);
}

void Grog::GraphEditor::Terminate() {
    ax::NodeEditor::SetCurrentEditor(nullptr);
    ax::NodeEditor::DestroyEditor(context);
    GetApplication()->GetEventBus().RemoveListener(this);
}

void Grog::GraphEditor::DrawWidget() {
    VCLG::Graph& graph = GetApplication()->GetGraph();

    ax::NodeEditor::Begin("Editor");
    
    if (nodeRegistry) {
        ax::NodeEditor::Suspend();
        if ((ImGui::IsKeyPressed(ImGuiKey_Space) || ImGui::IsMouseClicked(ImGuiMouseButton_Right)) &&
            ImGui::IsItemHovered()) {
            ImGui::OpenPopup("Add Node");
        }
        DrawAddNodePopup();
        ax::NodeEditor::Resume();
    }

    for (VCLG::Graph::NodeHandle handle : graph)
        DrawNode(handle);

    ax::NodeEditor::End();
}

bool Grog::GraphEditor::OnEvent(EventHandle& handle) {
    if (auto event = handle.Get<AsyncResourceManager::ResourcesLoadedEvent>()) {
        nodeRegistry = event->nodeRegistry;
        return false;
    }
    return false;
}

void Grog::GraphEditor::DrawAddNodePopup() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
    if (ImGui::BeginPopup("Add Node")) {
        const ImVec2 clickPos = ImGui::GetMousePosOnOpeningCurrentPopup();
        if (ImGui::IsWindowAppearing())
            ImGui::SetKeyboardFocusHere(0);
        ImGui::InputText("##filter", &nodeAddPopupFilter);
        ImGui::Separator();
        for (auto& entry : *nodeRegistry) {
            if (entry.first.find(nodeAddPopupFilter) != std::string::npos)
                DrawAddNodeMenu(entry.first.cbegin(), entry.first.cend(), entry.second, clickPos);
        }
        ImGui::EndPopup();
    } else {
        nodeAddPopupFilter.clear();
    }
    ImGui::PopStyleVar();
}

void Grog::GraphEditor::DrawAddNodeMenu(std::string::const_iterator&& begin, std::string::const_iterator&& end, NodeSource& source, const ImVec2& clickPos) {
    for (auto it = begin; it < end; ++it) {
        if (*it == '/' || *it == '\\') {
            std::string name{ begin, it };
            if (ImGui::BeginMenu(name.c_str())) {
                DrawAddNodeMenu(it + 1, std::move(end), source, clickPos);
                ImGui::EndMenu();
            }
            return;
        }
    }
    std::string name{ begin, end };
    if (ImGui::MenuItem(name.c_str())) {
        InstantiateNode(name, source, clickPos);
    }
}

void Grog::GraphEditor::InstantiateNode(const std::string& name, NodeSource& nodeSource, const ImVec2& position) {
    std::unique_ptr<Node> node = std::make_unique<Node>(nodeSource.source, nullptr);
    node->SetName(name);
    VCLG::Graph::NodeHandle handle = GetApplication()->GetGraph().AddNode(std::move(node));
    ax::NodeEditor::SetNodePosition(GetAxNodeEditorNodeId(handle), ax::NodeEditor::ScreenToCanvas(position));
}

void Grog::GraphEditor::DrawNode(VCLG::Graph::NodeHandle handle) {
    Node* node = (Node*)handle.Get();
    NodeBuilder builder{ GetAxNodeEditorNodeId(handle) };
    builder.Begin();

    builder.BeginHeader();
    ImGui::TextUnformatted(node->GetName().c_str());
    builder.EndHeader();

    builder.BeginContent();

    builder.BeginInput();
    for (uint32_t i = 0; i < node->GetInputs().size(); ++i)
        DrawPort(handle.GetInput(i));
    builder.EndInput();

    builder.BeginOutput();
    for (uint32_t i = 0; i < node->GetOutputs().size(); ++i)
        DrawPort(handle.GetOutput(i));
    builder.EndOutput();

    builder.EndContent();

    builder.End();
}

void Grog::GraphEditor::DrawPort(VCLG::Graph::PortHandle portHandle) {
    ax::NodeEditor::BeginPin(
        GetAxNodeEditorPinId(portHandle),
        portHandle.IsPortInput() ? ax::NodeEditor::PinKind::Input : ax::NodeEditor::PinKind::Output
    );

    if (portHandle.IsPortInput()) {
        DrawPortIcon(4.0f, ImColor{ 204, 65, 65, 255 }, false);
        ImGui::SameLine();
    }

    ImGui::TextUnformatted(portHandle->GetName().c_str());

    if (!portHandle.IsPortInput()) {
        ImGui::SameLine();
        DrawPortIcon(4.0f, ImColor{ 204, 65, 65, 255 }, false);
    }

    ax::NodeEditor::EndPin();
}

void Grog::GraphEditor::DrawPortIcon(float size, ImU32 color, bool filled) {
    ImGui::Dummy(ImVec2{ 10.0f, 20.0f });
    if (ImGui::IsRectVisible(ImVec2{ size, size })) {
        ImDrawList *drawList = ImGui::GetWindowDrawList();
        
        ImVec2 rectMin = ImGui::GetItemRectMin();
        ImVec2 rectSize = ImGui::GetItemRectSize();

        ImVec2 c = ImVec2{ rectMin.x + rectSize.x / 2.0f, rectMin.y + rectSize.y / 2.0f };

        if (filled)
            drawList->AddCircleFilled(c, size, color);
        else
            drawList->AddCircle(c, size, color);
    }
}

ax::NodeEditor::NodeId Grog::GraphEditor::GetAxNodeEditorNodeId(VCLG::Graph::NodeHandle nodeHandle) {
    return nodeHandle.GetNodeIdx() + 1;
}

ax::NodeEditor::PinId Grog::GraphEditor::GetAxNodeEditorPinId(VCLG::Graph::PortHandle portHandle) {
    const uint32_t maxNodeCountPerGraph = 16384;
    const uint32_t maxPortCountPerNode = 128;
    uint32_t offset = portHandle.GetNodeHandle().GetNodeIdx() * maxPortCountPerNode;
    return (portHandle.GetPortIdx() + offset + (portHandle.IsPortInput() ? 0 : maxPortCountPerNode / 2)) + maxNodeCountPerGraph + 1;
}