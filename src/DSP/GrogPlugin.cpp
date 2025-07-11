#include "GrogPlugin.hpp"


Grog::GrogPlugin::GrogPlugin() : Plugin{ 0, 0, 0 }, graph{ MakeAtomicSharedPtr<VCLG::Graph>(nullptr) } {}

const char* Grog::GrogPlugin::getLabel() const {
    return "Grog";
}

const char* Grog::GrogPlugin::getDescription() const {
    return "Modular node based synthesizer.";
}

const char* Grog::GrogPlugin::getMaker() const {
    return "HITO";
}

const char* Grog::GrogPlugin::getHomePage() const {
    return "https://github.com/HITOA/grog";
}

const char* Grog::GrogPlugin::getLicense() const {
    return "GPLv3";
}

uint32_t Grog::GrogPlugin::getVersion() const {
    return d_version(0, 0, 1);
}

void Grog::GrogPlugin::initAudioPort(bool input, uint32_t index, AudioPort& port) {
    port.groupId = kPortGroupStereo;
    Plugin::initAudioPort(input, index, port);
}

void Grog::GrogPlugin::initParameter(uint32_t index, Parameter& parameter) {

}

float Grog::GrogPlugin::getParameterValue(uint32_t index) const {
    return 0.0f;
}

void Grog::GrogPlugin::setParameterValue(uint32_t index, float value) {
    
}

void Grog::GrogPlugin::run(const float** inputs, float** outputs, uint32_t frames,
                    const MidiEvent* midiEvents, uint32_t midiEventCount) {
    for (uint32_t i = 0; i < frames; ++i) {
        outputs[0][i] = 0;
        outputs[1][i] = 0;
    }

    VCLG::Graph::ExecutionContextHandle context = graph->GetExecutionContext();

    if (!context)
        return;

    void* userData = context->GetUserData();
    if (!userData)
        return;
    
    
}

Grog::AtomicSharedPtr<VCLG::Graph> Grog::GrogPlugin::GetGraph() {
    return graph;
}

START_NAMESPACE_DISTRHO

Plugin* createPlugin() {
    return new Grog::GrogPlugin{};
}

END_NAMESPACE_DISTRHO