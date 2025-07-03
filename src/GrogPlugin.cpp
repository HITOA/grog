#include "GrogPlugin.hpp"


GrogPlugin::GrogPlugin() : Plugin{ 0, 0, 0 } {}

const char* GrogPlugin::getLabel() const {
    return "Grog";
}

const char* GrogPlugin::getDescription() const {
    return "Modular node based synthesizer.";
}

const char* GrogPlugin::getMaker() const {
    return "HITO";
}

const char* GrogPlugin::getHomePage() const {
    return "https://github.com/HITOA/grog";
}

const char* GrogPlugin::getLicense() const {
    return "GPLv3";
}

uint32_t GrogPlugin::getVersion() const {
    return d_version(0, 0, 1);
}

void GrogPlugin::initAudioPort(bool input, uint32_t index, AudioPort& port) {
    port.groupId = kPortGroupStereo;
    Plugin::initAudioPort(input, index, port);
}

void GrogPlugin::initParameter(uint32_t index, Parameter& parameter) {

}

float GrogPlugin::getParameterValue(uint32_t index) const {
    return 0.0f;
}

void GrogPlugin::setParameterValue(uint32_t index, float value) {
    
}

void GrogPlugin::run(const float** inputs, float** outputs, uint32_t frames,
                    const MidiEvent* midiEvents, uint32_t midiEventCount) {
    for (uint32_t i = 0; i < frames; ++i) {
        outputs[0][i] = 0;
        outputs[1][i] = 0;
    }
}

START_NAMESPACE_DISTRHO

Plugin* createPlugin() {
    return new GrogPlugin{};
}

END_NAMESPACE_DISTRHO