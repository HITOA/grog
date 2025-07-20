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
    static const char* n[] = {
        "NONE",
        "ERROR",
        "WARNING",
        "Info",
        "Debug"
    };

    if (auto event = handle.Get<LogMessageEvent>()) {
        std::cout << "[" << n[(int)event->message.severity] << "]: " << event->message.message << std::endl;
        return true;
    }
    return false;
}