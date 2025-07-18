#pragma once

#include <UI/Module.hpp>
#include <UI/Event.hpp>

#include <string>


namespace Grog {

    struct Message {
        enum class MessageSeverity {
            None = 0,
            Error = 1,
            Warning = 2,
            Info = 3,
            Debug = 4
        } severity = MessageSeverity::None;
        std::string message;
    };
    
    class Logger : public Module, public EventListener {
    public:
        //Event
        struct LogMessageEvent {
            Message message;
        };
        
    public:
        void Initialize() override;
        void Terminate() override;
        
        bool OnEvent(EventHandle& handle) override;
    };

}