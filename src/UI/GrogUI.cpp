#include "GrogUI.hpp"

#include <imgui.h>

#include <DSP/GrogPlugin.hpp>


Grog::GrogUI::GrogUI() : ImguiUI{ DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT } {
    setGeometryConstraints(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT, true);
}

void Grog::GrogUI::parameterChanged(uint32_t index, float value) {

}

void Grog::GrogUI::onImguiDisplay() {

    static bool open = true;
    ImGui::ShowDemoWindow(&open);
}

START_NAMESPACE_DISTRHO

UI* createUI() {
    return new Grog::GrogUI{};
}

END_NAMESPACE_DISTRHO