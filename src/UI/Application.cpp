#include "Application.hpp"

#include <imgui.h>

#include <DSP/GrogPlugin.hpp>

#include <UI/Filesystem.hpp>

#include <UI/Modules/AsyncResourceManager.hpp>
#include <UI/Modules/Logger.hpp>
#include <UI/Modules/ModalWindowManager.hpp>


Grog::Application::Application() : ImguiUI{ DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT } {
    setGeometryConstraints(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT, true);
}

Grog::Application::~Application() {
    for (auto& module : modules)
        module->Terminate();
}

void Grog::Application::Initialize() {
    std::filesystem::path configDirectory = Filesystem::GetUserConfigDirectoryPath() / "Grog";
    std::filesystem::create_directories(configDirectory);

    std::filesystem::path configFilePath = configDirectory / "config.json";
    if (std::filesystem::exists(configFilePath))
        config.LoadFromDisk(configFilePath);
    
    config.SaveToDisk(configFilePath);
}

void Grog::Application::AddModule(std::unique_ptr<Module> module) {
    module->app = this;
    module->Initialize();
    modules.push_back(std::move(module));
}

Grog::Config& Grog::Application::GetConfig() {
    return config;
}

Grog::EventBus& Grog::Application::GetEventBus() {
    return eventBus;
}

void Grog::Application::parameterChanged(uint32_t index, float value) {

}

void Grog::Application::OnImguiDisplay() {
    eventBus.Process();
    
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    ImGui::Begin("Grog", nullptr, 
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);

    ImGui::End();

    for (auto& module : modules)
        module->Draw();
}

START_NAMESPACE_DISTRHO

UI* createUI() {
    Grog::Application* ui = new Grog::Application{};
    ui->Initialize();
    ui->AddModule<Grog::Logger>();
    ui->AddModule<Grog::AsyncResourceManager>();
    ui->AddModule<Grog::ModalWindowManager>();
    return ui;
}

END_NAMESPACE_DISTRHO