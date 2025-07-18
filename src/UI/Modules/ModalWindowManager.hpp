#pragma once

#include <UI/Module.hpp>
#include <UI/Event.hpp>

#include <vector>


namespace Grog {

    class ModalWindow {
    public:
        virtual ~ModalWindow() = default;

        virtual std::string GetName() = 0;
        virtual void Draw() = 0;
    };

    class GenericMessageModalWindow : public ModalWindow {
    public:
        GenericMessageModalWindow(const std::string& name, const std::string& msg);

        std::string GetName() override;
        void Draw() override;

    private:
        std::string name;
        std::string msg;
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

        void Draw() override;

        bool OnEvent(EventHandle& handle) override;

    private:
        std::vector<std::unique_ptr<ModalWindow>> modals{};
    };

}