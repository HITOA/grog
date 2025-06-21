#pragma once

#include <DistrhoPlugin.hpp>


class GrogPlugin : public DISTRHO::Plugin {
public:
    GrogPlugin();

protected:
    const char* getLabel() const override;

    const char* getDescription() const override;

    const char* getMaker() const override;

    const char* getHomePage() const override;

    const char* getLicense() const override;

    uint32_t getVersion() const override;

    void initAudioPort(bool input, uint32_t index, AudioPort& port) override;
    
    void initParameter(uint32_t index, Parameter& parameter) override;

    float getParameterValue(uint32_t index) const override;

    void setParameterValue(uint32_t index, float value) override;

    #ifdef GROG_IS_SYNTH
    void run(const float** inputs, float** outputs, uint32_t frames,
                     const MidiEvent* midiEvents, uint32_t midiEventCount) override;
    #else
    void run(const float** inputs, float** outputs, uint32_t frames) override;
    #endif


private:
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrogPlugin)
};