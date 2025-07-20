#pragma once

#include <DistrhoPlugin.hpp>

#include <VCLG/Graph.hpp>

#include <Common/AtomicSharedPtr.hpp>


namespace Grog {

    class Processor : public DISTRHO::Plugin {
    public:
        Processor();

        AtomicSharedPtr<VCLG::Graph> GetGraph();
        
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

        void run(const float** inputs, float** outputs, uint32_t frames,
                        const MidiEvent* midiEvents, uint32_t midiEventCount) override;

    private:
        AtomicSharedPtr<VCLG::Graph> graph;

        DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Processor)
    };

}