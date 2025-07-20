#include "GraphEditor.hpp"

#include <UI/Application.hpp>
#include <UI/Modules/AsyncResourceManager.hpp>

#include <iostream>


void Grog::GraphEditor::Initialize() {
    GetApplication()->GetEventBus().AddListener(this);
}

void Grog::GraphEditor::Terminate() {
    GetApplication()->GetEventBus().RemoveListener(this);
}

void Grog::GraphEditor::Draw() {

}

bool Grog::GraphEditor::OnEvent(EventHandle& handle) {
    if (auto event = handle.Get<AsyncResourceManager::ResourcesLoadedEvent>()) {
        nodeRegistry = event->nodeRegistry;
        return false;
    }
    return false;
}