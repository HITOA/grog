#include "GraphEditor.hpp"

#include <UI/Application.hpp>
#include <UI/Modules/AsyncResourceManager.hpp>

#include <iostream>


void Grog::GraphEditor::Initialize() {
    GetApplication()->GetEventBus().AddListener(this);
    context = ax::NodeEditor::CreateEditor();
    ax::NodeEditor::SetCurrentEditor(context);
}

void Grog::GraphEditor::Terminate() {
    ax::NodeEditor::SetCurrentEditor(nullptr);
    ax::NodeEditor::DestroyEditor(context);
    GetApplication()->GetEventBus().RemoveListener(this);
}

void Grog::GraphEditor::DrawWidget() {
    ax::NodeEditor::Begin("Editor");
    ax::NodeEditor::End();
}

bool Grog::GraphEditor::OnEvent(EventHandle& handle) {
    if (auto event = handle.Get<AsyncResourceManager::ResourcesLoadedEvent>()) {
        nodeRegistry = event->nodeRegistry;
        return false;
    }
    return false;
}