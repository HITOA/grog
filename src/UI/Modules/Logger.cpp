#include "Logger.hpp"

#include <UI/Application.hpp>

#include <iostream>


void Grog::Logger::Initialize() {
    GetApplication()->GetEventBus().AddListener(this);
}

void Grog::Logger::Terminate() {
    GetApplication()->GetEventBus().RemoveListener(this);
}

bool Grog::Logger::OnEvent(EventHandle& handle) {
    if (auto event = handle.Get<LogMessageEvent>()) {
        return true;
    }
    return false;
}