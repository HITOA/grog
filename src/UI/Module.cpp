#include "Module.hpp"

#include "Application.hpp"



void Grog::Module::Initialize() {}

void Grog::Module::Terminate() {}

void Grog::Module::Draw() {}

void Grog::Module::DrawWidget() {}

Grog::Application* Grog::Module::GetApplication() {
    return app;
}