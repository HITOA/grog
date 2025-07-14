#pragma once

#include <UI/ImguiUI.hpp>

namespace Grog {

    class GrogUI : public ImguiUI {
    public:
        GrogUI();

    protected:
        void parameterChanged(uint32_t index, float value) override;

        void OnImguiDisplay() override;

    private:
        void DrawMainMenuBar();

    private:
        DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrogUI)
    };

}