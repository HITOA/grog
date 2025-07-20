#include "Processor.hpp"


Grog::Processor::Processor() : Plugin{ 0, 0, 0 }, graph{ MakeAtomicSharedPtr<VCLG::Graph>(nullptr) } {}

const char* Grog::Processor::getLabel() const {
    return "Grog";
}

const char* Grog::Processor::getDescription() const {
    return "Modular node based synthesizer.";
}

const char* Grog::Processor::getMaker() const {
    return "HITO";
}

const char* Grog::Processor::getHomePage() const {
    return "https://github.com/HITOA/grog";
}

const char* Grog::Processor::getLicense() const {
    return "GPLv3";
}

uint32_t Grog::Processor::getVersion() const {
    return d_version(0, 0, 1);
}

void Grog::Processor::initAudioPort(bool input, uint32_t index, AudioPort& port) {
    port.groupId = kPortGroupStereo;
    Plugin::initAudioPort(input, index, port);
}

void Grog::Processor::initParameter(uint32_t index, Parameter& parameter) {

}

float Grog::Processor::getParameterValue(uint32_t index) const {
    return 0.0f;
}

void Grog::Processor::setParameterValue(uint32_t index, float value) {
    
}

void Grog::Processor::run(const float** inputs, float** outputs, uint32_t frames,
                    const MidiEvent* midiEvents, uint32_t midiEventCount) {
    for (uint32_t i = 0; i < frames; ++i) {
        outputs[0][i] = 0;
        outputs[1][i] = 0;
    }
    
}

Grog::AtomicSharedPtr<VCLG::Graph> Grog::Processor::GetGraph() {
    return graph;
}

START_NAMESPACE_DISTRHO

Plugin* createPlugin() {
    return new Grog::Processor{};
}

END_NAMESPACE_DISTRHO