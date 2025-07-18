#include "ModalWindowManager.hpp"

#include <UI/Application.hpp>

#include <imgui.h>



Grog::GenericMessageModalWindow::GenericMessageModalWindow(const std::string& name, const std::string& msg) : 
    name{ name }, msg{ msg } {

}

std::string Grog::GenericMessageModalWindow::GetName() {
    return name;
}

void Grog::GenericMessageModalWindow::Draw() {
    ImGui::TextUnformatted(msg.c_str());
    ImGui::Separator();
    if (ImGui::Button("Ok"))
        ImGui::CloseCurrentPopup();
}

void Grog::ModalWindowManager::Initialize() {
    GetApplication()->GetEventBus().AddListener(this);
}

void Grog::ModalWindowManager::Terminate() {
    GetApplication()->GetEventBus().RemoveListener(this);
}

void Grog::ModalWindowManager::Draw() {
    for (size_t i = 0; i < modals.size();) {
        std::unique_ptr<ModalWindow>& modal = modals[i];
        if (!ImGui::IsPopupOpen(modal->GetName().c_str())) {
            modals.erase(modals.begin() + i);
            continue;
        }
        if (ImGui::BeginPopupModal(modal->GetName().c_str(), nullptr, ImGuiWindowFlags_NoResize)) {
            modal->Draw();
            ImGui::EndPopup();
        }
        ++i;
    }
}

bool Grog::ModalWindowManager::OnEvent(EventHandle& handle) {
    if (auto event = handle.Get<ShowModalWindowEvent>()) {
        ImGui::OpenPopup(event->modal->GetName().c_str(), 0);
        modals.push_back(std::move(event->modal));
        return true;
    }
    return false;
}