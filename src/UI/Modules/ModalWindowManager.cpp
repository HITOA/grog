#include "ModalWindowManager.hpp"

#include <UI/Application.hpp>


void Grog::ModalWindowManager::Initialize() {
    GetApplication()->GetEventBus().AddListener(this);
}

void Grog::ModalWindowManager::Terminate() {
    GetApplication()->GetEventBus().RemoveListener(this);
}

bool Grog::ModalWindowManager::OnEvent(EventHandle& handle) {
    return false;
}