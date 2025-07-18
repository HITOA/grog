#pragma once

#include <UI/Module.hpp>
#include <UI/Event.hpp>


namespace Grog {

    class ModalWindow {
    public:
        virtual ~ModalWindow() = default;


    };

    class ModalWindowManager : public Module, public EventListener {
    public:
        //Event
        struct ShowModalWindowEvent {
            std::unique_ptr<ModalWindow> modal;
        };
    
    public:
        void Initialize() override;
        void Terminate() override;
        
        bool OnEvent(EventHandle& handle) override;
    };

}