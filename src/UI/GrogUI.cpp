#include "GrogUI.hpp"

#include <imgui.h>

#include <DSP/GrogPlugin.hpp>


Grog::GrogUI::GrogUI() : ImguiUI{ DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT } {
    setGeometryConstraints(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT, true);
}

void Grog::GrogUI::parameterChanged(uint32_t index, float value) {

}

void Grog::GrogUI::OnImguiDisplay() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    ImGui::Begin("Grog", nullptr, 
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);

    DrawMainMenuBar();

    ImGui::End();
}

void Grog::GrogUI::DrawMainMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window")) {

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

START_NAMESPACE_DISTRHO

UI* createUI() {
    return new Grog::GrogUI{};
}

END_NAMESPACE_DISTRHO