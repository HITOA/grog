#include <DistrhoUI.hpp>


START_NAMESPACE_DISTRHO

class GrogUI : public UI {
public:
    GrogUI() : UI{ DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT } {

    }

protected:
    void parameterChanged(uint32_t index, float value) override {

    }

    void onDisplay() override {
        
    }

    bool onMouse(const MouseEvent& event) override {
        return false;
    }

private:
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrogUI)
};

UI* createUI() {
    return new GrogUI{};
}

END_NAMESPACE_DISTRHO