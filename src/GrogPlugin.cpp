#include <DistrhoPlugin.hpp>


START_NAMESPACE_DISTRHO

class GrogPlugin : public Plugin {
public:
    GrogPlugin() : Plugin{ 0, 0, 0 } {}

protected:
    const char* getLabel() const override {
        return "Grog";
    }

    const char* getDescription() const override {
        return "Modular node based synthesizer.";
    }

    const char* getMaker() const override {
        return "HITO";
    }

    const char* getHomePage() const override {
        return "https://github.com/HITOA/grog";
    }

    const char* getLicense() const override {
        return "GPLv3";
    }

    uint32_t getVersion() const override {
        return d_version(0, 0, 1);
    }

    void initAudioPort(bool input, uint32_t index, AudioPort& port) override {
        port.groupId = kPortGroupStereo;
        Plugin::initAudioPort(input, index, port);
    }
    
    void initParameter(uint32_t index, Parameter& parameter) override {

    }

    float getParameterValue(uint32_t index) const override {
        return 0.0f;
    }

    void setParameterValue(uint32_t index, float value) override {

    }


    void run(const float** inputs, float** outputs, uint32_t frames,
                     const MidiEvent* midiEvents, uint32_t midiEventCount) override {
        
        for (uint32_t i = 0; i < frames; ++i) {
            outputs[0][i] = 0;
            outputs[1][i] = 0;
        }
    }


private:
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrogPlugin)
};

Plugin* createPlugin() {
    return new GrogPlugin{};
}

END_NAMESPACE_DISTRHO