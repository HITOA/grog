#pragma once

#include <Common/AtomicSharedPtr.hpp>

#include <UI/ImguiUI.hpp>
#include <UI/Config.hpp>
#include <UI/Event.hpp>
#include <UI/Module.hpp>

#include <VCLG/Graph.hpp>


namespace Grog {

    class Application : public ImguiUI {
    public:
        Application();
        ~Application();

        void Initialize();

        void AddModule(std::unique_ptr<Module> module);
        template<typename T, typename... Args>
        void AddModule(Args&&... args) {
            AddModule(std::make_unique<T>(std::forward<Args>(args)...));
        }

        Config& GetConfig();
        EventBus& GetEventBus();
        VCLG::Graph& GetGraph();

    protected:
        void parameterChanged(uint32_t index, float value) override;
        
        void OnImguiDisplay() override;

    private:
        Config config{};
        EventBus eventBus{};
        AtomicSharedPtr<VCLG::Graph> graph{};

        std::vector<std::unique_ptr<Module>> modules{};

        DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Application)
    };

}