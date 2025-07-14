#pragma once

#include <DistrhoUI.hpp>
#include <imgui.h>


namespace Grog {

    class ImguiUI : public DISTRHO::UI {
    public:
        ImguiUI(uint width = 0, uint height = 0, bool automaticallyScaleAndSetAsMinimumSize = false);
        ~ImguiUI() override;

    protected:
        virtual void OnImguiDisplay() = 0;
        
        void onDisplay() override;
        void uiIdle() override;

        bool onKeyboard(const KeyboardEvent& event) override;
        bool onCharacterInput(const CharacterInputEvent& event) override;
        bool onMouse(const MouseEvent& event) override;
        bool onMotion(const MotionEvent& event) override;
        bool onScroll(const ScrollEvent& event) override;
        
        void onResize(const ResizeEvent& event) override;

    private:
        ImGuiContext* context;
        double lastFrameTime;

        DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImguiUI)
    };
    
}