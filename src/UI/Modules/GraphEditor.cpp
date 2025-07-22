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

    ax::NodeEditor::Style& style = ax::NodeEditor::GetStyle();
    style.Colors[ax::NodeEditor::StyleColor_Bg] = ImColor(16, 16, 17, 255);
    style.Colors[ax::NodeEditor::StyleColor_Grid] = ImColor(36, 36, 37, 255);
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
        InstantiateNode(source, clickPos);
    }
}

void Grog::GraphEditor::InstantiateNode(NodeSource& nodeSource, const ImVec2& position) {
    std::unique_ptr<Node> node = std::make_unique<Node>(nodeSource.source, nullptr);
    VCLG::Graph::NodeHandle handle = GetApplication()->GetGraph().AddNode(std::move(node));
    ax::NodeEditor::SetNodePosition(handle.GetNodeIdx() + 1, ax::NodeEditor::ScreenToCanvas(position));
}

void Grog::GraphEditor::DrawNode(VCLG::Graph::NodeHandle handle) {
    NodeBuilder builder{ handle.GetNodeIdx() + 1 };
    builder.Begin();
    ImGui::TextUnformatted("UWU");
    builder.End();
}